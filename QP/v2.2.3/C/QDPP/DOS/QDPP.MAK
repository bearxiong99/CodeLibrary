# Microsoft Visual C++ generated build script - Do not modify

PROJ = QDPP
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\QPCODE\C\QDPP\DOS\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = MAIN.C      
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /Gs /G2 /FPi87 /W3 /Zi /AL /Od /D "_DEBUG" /D "_DOS" /D "DOS" /I "." /I "..\Include" /I "..\..\Qf\include" /FR /Fd"QDPP.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /FPi87 /W3 /AL /Ox /D "NDEBUG" /D "_DOS" /D "DOS" /I "." /I "..\Include" /I "..\..\Qf\include" /FR 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE 
LIBS_D_DEXE = ..\..\Qf\DOS\Qf_dos.lib oldnames llibc7 
LIBS_R_DEXE = ..\..\Qf\DOS\Qf_dos.lib oldnames llibc7 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = MAIN.SBR \
		QDPP.SBR


MAIN_DEP = d:\qpcode\c\qdpp\dos\port.h


all:	$(PROJ).EXE $(PROJ).BSC

MAIN.OBJ:	MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c MAIN.C

QDPP.OBJ:	..\SOURCE\QDPP.C $(QDPP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QDPP.C

$(PROJ).EXE::	MAIN.OBJ QDPP.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
MAIN.OBJ +
QDPP.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
d:\tools\vc1.52\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
