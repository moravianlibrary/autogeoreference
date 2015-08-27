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
	${OBJECTDIR}/Affine2DEstimator.o \
	${OBJECTDIR}/AutoGeoreference.o \
	${OBJECTDIR}/AwareImageDownloader.o \
	${OBJECTDIR}/Controller.o \
	${OBJECTDIR}/GeoreferencerMetadataProvider.o \
	${OBJECTDIR}/ImageDownloaderManager.o \
	${OBJECTDIR}/ZoomifyImageDownloader.o \
	${OBJECTDIR}/ZoomifyPyramid.o \
	${OBJECTDIR}/appio.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11
CXXFLAGS=-std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lgdal -lgdal_correlator -lPocoJSON -lPocoNet -lPocoFoundation -lPocoNetSSL -lfcgi++ -lfcgi

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fcgi

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fcgi: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fcgi ${OBJECTFILES} ${LDLIBSOPTIONS} `pkg-config opencv --libs`

${OBJECTDIR}/Affine2DEstimator.o: Affine2DEstimator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Affine2DEstimator.o Affine2DEstimator.cpp

${OBJECTDIR}/AutoGeoreference.o: AutoGeoreference.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AutoGeoreference.o AutoGeoreference.cpp

${OBJECTDIR}/AwareImageDownloader.o: AwareImageDownloader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AwareImageDownloader.o AwareImageDownloader.cpp

${OBJECTDIR}/Controller.o: Controller.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Controller.o Controller.cpp

${OBJECTDIR}/GeoreferencerMetadataProvider.o: GeoreferencerMetadataProvider.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GeoreferencerMetadataProvider.o GeoreferencerMetadataProvider.cpp

${OBJECTDIR}/ImageDownloaderManager.o: ImageDownloaderManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ImageDownloaderManager.o ImageDownloaderManager.cpp

${OBJECTDIR}/ZoomifyImageDownloader.o: ZoomifyImageDownloader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ZoomifyImageDownloader.o ZoomifyImageDownloader.cpp

${OBJECTDIR}/ZoomifyPyramid.o: ZoomifyPyramid.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ZoomifyPyramid.o ZoomifyPyramid.cpp

${OBJECTDIR}/appio.o: appio.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/appio.o appio.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/gdal -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/fcgi

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
