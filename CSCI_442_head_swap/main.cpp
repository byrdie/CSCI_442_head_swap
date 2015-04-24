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
    cv::namedWindow("swap", CV_WINDOW_KEEPRATIO);



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
        cv::cvtColor(frame, frame, CV_BGR2RGB);

        Mat swap = frame.clone();

        const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
        nite::SkeletonJoint heads[10];
        Point head_point[2];
        for (int i = 0; i < users.getSize(); ++i) {
            const nite::UserData& user = users[i];
            updateUserState(user, userTrackerFrame.getTimestamp());
            if (user.isNew()) {
                userTracker.startSkeletonTracking(user.getId());
            } else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED) {
                heads[i] = user.getSkeleton().getJoint(nite::JOINT_HEAD);
                if (heads[i].getPositionConfidence() > .5) {
                    //                    printf("%d. (%5.2f, %5.2f, %5.2f)\n", user.getId(), heads[i].getPosition().x, heads[i].getPosition().y, heads[i].getPosition().z);

                    int X, Y;
                    openni::DepthPixel Z;
                    openni::CoordinateConverter::convertWorldToDepth(depth, heads[i].getPosition().x, heads[i].getPosition().y, heads[i].getPosition().z, &X, &Y, &Z);
                    head_point[i] = cv::Point(X, Y);




                }

            }
        }
        //        cv::Mat swap = cv::Mat::zeros(cv::Size(320, 240), CV_8UC3);
        if (users.getSize() > 1) {
            Point upperCorner[2];
            Point lowerCorner[2];
            for (int i = 0; i < 2; i++) {
                openni::DepthPixel Z;
                double upperCornerX = heads[i].getPosition().x + 75.0;
                double upperCornerY = heads[i].getPosition().y + 112.5;
                double lowerCornerX = heads[i].getPosition().x - 75.0;
                double lowerCornerY = heads[i].getPosition().y - 112.5;


                openni::CoordinateConverter::convertWorldToDepth(depth, upperCornerX, upperCornerY, heads[i].getPosition().z, &upperCorner[i].x, &upperCorner[i].y, &Z);
                openni::CoordinateConverter::convertWorldToDepth(depth, lowerCornerX, lowerCornerY, heads[i].getPosition().z, &lowerCorner[i].x, &lowerCorner[i].y, &Z);
                printf("%d: (%d, %d); (%d, %d)\n", i, lowerCorner[i].x, lowerCorner[i].y, upperCorner[i].x, upperCorner[i].y);
            }



            Point length_1;
            length_1.x = upperCorner[0].x - lowerCorner[0].x;
            length_1.y = lowerCorner[0].y - upperCorner[0].y;

            //            Point length_2 = upperCorner[2] - lowerCorner[2];
            //            printf("%d, %d\n", length_1.x, length_1.y);

            for (int i = 0; i < length_1.x; i++) {
                for (int j = 0; j < length_1.y; j++) {

                    if (i + lowerCorner[0].x < 320 && i + lowerCorner[1].x < 320 && j + upperCorner[0].y < 240 && j + upperCorner[1].y < 240) {
                        if (i + lowerCorner[0].x >= 0 && i + lowerCorner[1].x >= 0 && j + upperCorner[0].y >= 0 && j + upperCorner[1].y >= 0) {
                            swap.at<cv::Vec3b>(j + upperCorner[1].y, i + lowerCorner[1].x) = frame.at<cv::Vec3b>(j + upperCorner[0].y, i + lowerCorner[0].x);

                            swap.at<cv::Vec3b>(j + upperCorner[0].y, i + lowerCorner[0].x) = frame.at<cv::Vec3b>(j + upperCorner[1].y, i + lowerCorner[1].x);

                        }
                    }

                }
            }
        }

        circle(frame, head_point[0], 10, Scalar(0, 255, 0));
        circle(frame, head_point[1], 10, Scalar(0, 255, 0));



        cv::imshow("OpenCV", frame);
        cv::imshow("swap", swap);


        c = cv::waitKey(10);
        if (c == 27)
            break;

    }

    nite::NiTE::shutdown();

}