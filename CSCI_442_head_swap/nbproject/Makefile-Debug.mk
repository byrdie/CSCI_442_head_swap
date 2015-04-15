#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs opencv` NiTE-Linux-x64-2.2/Redist/libNiTE2.jni.so NiTE-Linux-x64-2.2/Redist/libNiTE2.so OpenNI2/Bin/x64-Release/libDepthUtils.a OpenNI2/Bin/x64-Release/libMWClosestPoint.so OpenNI2/Bin/x64-Release/libOpenNI2.jni.so OpenNI2/Bin/x64-Release/libOpenNI2.so OpenNI2/Bin/x64-Release/OpenNI2/Drivers/libDummyDevice.so OpenNI2/Bin/x64-Release/OpenNI2/Drivers/libOniFile.so OpenNI2/Bin/x64-Release/OpenNI2/Drivers/libPS1080.so OpenNI2/Bin/x64-Release/OpenNI2/Drivers/libPSLink.so -lOpenNI2  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: NiTE-Linux-x64-2.2/Redist/libNiTE2.jni.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: NiTE-Linux-x64-2.2/Redist/libNiTE2.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: OpenNI2/Bin/x64-Release/libDepthUtils.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: OpenNI2/Bin/x64-Release/libMWClosestPoint.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: OpenNI2/Bin/x64-Release/libOpenNI2.jni.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: OpenNI2/Bin/x64-Release/libOpenNI2.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: OpenNI2/Bin/x64-Release/OpenNI2/Drivers/libDummyDevice.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: OpenNI2/Bin/x64-Release/OpenNI2/Drivers/libOniFile.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: OpenNI2/Bin/x64-Release/OpenNI2/Drivers/libPS1080.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: OpenNI2/Bin/x64-Release/OpenNI2/Drivers/libPSLink.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -INiTE-Linux-x64-2.2/Include -IOpenNI2/Include `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/csci_442_head_swap

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
