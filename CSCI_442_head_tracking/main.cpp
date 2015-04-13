/*******************************************************************************
 *                                                                              *
 *   PrimeSense NiTE 2.0 - Simple Skeleton Sample                               *
 *   Copyright (C) 2012 PrimeSense Ltd.                                         *
 *                                                                              *
 *******************************************************************************/

#include "main.h"

#define MAX_USERS 10
bool g_visibleUsers[MAX_USERS] = {false};
nite::SkeletonState g_skeletonStates[MAX_USERS] = {nite::SKELETON_NONE};
int c = 100;



#define USER_MESSAGE(msg) \
	{printf("[%08llu] User #%d:\t%s\n",ts, user.getId(),msg);}

void updateUserState(const nite::UserData& user, unsigned long long ts) {
    if (user.isNew())
        USER_MESSAGE("New")
    else if (user.isVisible() && !g_visibleUsers[user.getId()])
        USER_MESSAGE("Visible")
    else if (!user.isVisible() && g_visibleUsers[user.getId()])
        USER_MESSAGE("Out of Scene")
    else if (user.isLost())
        USER_MESSAGE("Lost")

        g_visibleUsers[user.getId()] = user.isVisible();


    if (g_skeletonStates[user.getId()] != user.getSkeleton().getState()) {
        switch (g_skeletonStates[user.getId()] = user.getSkeleton().getState()) {
            case nite::SKELETON_NONE:
                USER_MESSAGE("Stopped tracking.")
                break;
            case nite::SKELETON_CALIBRATING:
                USER_MESSAGE("Calibrating...")
                break;
            case nite::SKELETON_TRACKED:
                USER_MESSAGE("Tracking!")
                break;
            case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
            case nite::SKELETON_CALIBRATION_ERROR_HANDS:
            case nite::SKELETON_CALIBRATION_ERROR_LEGS:
            case nite::SKELETON_CALIBRATION_ERROR_HEAD:
            case nite::SKELETON_CALIBRATION_ERROR_TORSO:
                USER_MESSAGE("Calibration Failed... :-|")
                break;
        }
    }
}

int main(int argc, char** argv) {
    int rc;
    openni::Device device;
    const char* deviceURI = openni::ANY_DEVICE;
        openni::VideoStream depth, color;
    openni::VideoFrameRef pFrame, dep;
    cv::Mat frame = cv::Mat::zeros(cv::Size(320, 240), CV_8UC3);

        cv::namedWindow("OpenCV", CV_WINDOW_KEEPRATIO);

        

    nite::UserTracker userTracker;
    nite::Status niteRc;

    nite::NiTE::initialize();
    
    rc = openni::OpenNI::initialize();
    rc = device.open(deviceURI);
    if (rc != openni::STATUS_OK) {
        printf("SimpleViewer: Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
        return 1;

    }
    rc = depth.create(device, openni::SENSOR_DEPTH);

    if (rc == openni::STATUS_OK) {

        rc = depth.start();
        if (rc != openni::STATUS_OK) {
            printf("SimpleViewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
            depth.destroy();
        }
    } else {
        printf("SimpleViewer: Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
    }
    rc = color.create(device, openni::SENSOR_COLOR);
    if (rc == openni::STATUS_OK) {
        rc = color.start();
        if (rc != openni::STATUS_OK) {
            printf("SimpleViewer: Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
            color.destroy();
        }
    } else {
        printf("SimpleViewer: Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
    }

    
    
    
    niteRc = userTracker.create();
    if (niteRc != nite::STATUS_OK) {
        printf("Couldn't create user tracker\n");
        return 3;
    }
    printf("\nStart moving around to get detected...\n(PSI pose may be required for skeleton calibration, depending on the configuration)\n");

    nite::UserTrackerFrameRef userTrackerFrame;
    while (!wasKeyboardHit()) {
        
        
        niteRc = userTracker.readFrame(&userTrackerFrame);

        if (niteRc != nite::STATUS_OK) {
            printf("Get next frame failed\n");
            continue;
        }

        const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
        nite::SkeletonJoint head;
        for (int i = 0; i < users.getSize(); ++i) {
            const nite::UserData& user = users[i];
            updateUserState(user, userTrackerFrame.getTimestamp());
            if (user.isNew()) {
                userTracker.startSkeletonTracking(user.getId());
            } else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED) {
                head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
                if (head.getPositionConfidence() > .5)
                    printf("%d. (%5.2f, %5.2f, %5.2f)\n", user.getId(), head.getPosition().x, head.getPosition().y, head.getPosition().z);
            }
        }
        
        

        /*display image data*/
        color.readFrame(&pFrame);
        depth.readFrame(&dep);
        openni::RGB888Pixel *pColor = (openni::RGB888Pixel *) pFrame.getData();
        openni::DepthPixel* pDepth = (openni::DepthPixel *) dep.getData();
        
        for (int i = 0; i < frame.rows; i++) {
            for (int j = 0; j < frame.cols; j++) {
                openni::RGB888Pixel pix = pColor[frame.cols * i + j];
                frame.at<cv::Vec3b>(i, j) = cv::Vec3b(pix.r, pix.g, pix.b);
            }
        }
        
        int X, Y;
        openni::DepthPixel Z;
        openni::CoordinateConverter::convertWorldToDepth(depth, head.getPosition().x, head.getPosition().y, head.getPosition().z, &X, &Y, &Z);

        
        Point head_point = cv::Point(X, Y);
        cv::cvtColor(frame, frame, CV_BGR2RGB);
        circle(frame, head_point, 10, Scalar(0,255,0));
        
        cv::imshow("OpenCV", frame);
        
         c = cv::waitKey(10);
        if (c == 27)
            break;

    }

    nite::NiTE::shutdown();

}