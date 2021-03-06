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
CC=gcc.exe
CCC=g++.exe
CXX=g++.exe
FC=
AS=as.exe

# Macros
CND_PLATFORM=MinGW_QT-Windows
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cmd_line.o \
	${OBJECTDIR}/format.o \
	${OBJECTDIR}/calculator.o \
	${OBJECTDIR}/localedef.o \
	${OBJECTDIR}/plot.o \
	${OBJECTDIR}/functions.o \
	${OBJECTDIR}/viewport2d.o


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
LDLIBSOPTIONS=-Linclude lwtk.dll

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/graphic_plot.exe

dist/Release/graphic_plot.exe: lwtk.dll

dist/Release/graphic_plot.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Release
	${LINK.cc} -mwindows -o ${CND_DISTDIR}/${CND_CONF}/graphic_plot -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/cmd_line.o: cmd_line.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -Iinclude -o ${OBJECTDIR}/cmd_line.o cmd_line.cpp

${OBJECTDIR}/format.o: format.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -Iinclude -o ${OBJECTDIR}/format.o format.cpp

${OBJECTDIR}/calculator.o: calculator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -Iinclude -o ${OBJECTDIR}/calculator.o calculator.cpp

${OBJECTDIR}/localedef.o: localedef.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -Iinclude -o ${OBJECTDIR}/localedef.o localedef.cpp

${OBJECTDIR}/plot.o: plot.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -Iinclude -o ${OBJECTDIR}/plot.o plot.cpp

${OBJECTDIR}/functions.o: functions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -Iinclude -o ${OBJECTDIR}/functions.o functions.cpp

${OBJECTDIR}/viewport2d.o: viewport2d.cpp 
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.cc) -O2 -s -Iinclude -o ${OBJECTDIR}/viewport2d.o viewport2d.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/graphic_plot.exe

# Subprojects
.clean-subprojects:
