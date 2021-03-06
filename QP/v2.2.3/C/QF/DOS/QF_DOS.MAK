# Microsoft Visual C++ generated build script - Do not modify

PROJ = QF_DOS
DEBUG = 0
PROGTYPE = 4
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\QPCODE\C\QF\DOS\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP = DOS.CPP     
RC = rc
CFLAGS_D_LIB = /nologo /G2 /FPi87 /W3 /Z7 /AL /Od /D "_DEBUG" /I "." /I "..\include" /I "..\source" /FR /GA 
CFLAGS_R_LIB = /nologo /Gs /G2 /FPi87 /W3 /AL /Ox /D "NDEBUG" /I "." /I "..\include" /I "..\source" /FR /GA 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_LIB)
LFLAGS = 
LIBS = 
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_LIB)
LFLAGS = 
LIBS = 
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = DOS.SBR \
		CPLUS.SBR \
		QACTIVE.SBR \
		QEPOOL.SBR \
		QEQUEUE.SBR \
		QF.SBR \
		QFSM.SBR \
		QHSM.SBR \
		QHSMTRAN.SBR \
		QTIMER.SBR


DOS_DEP = d:\qpcode\c\qf\dos\port.h


all:	$(PROJ).LIB $(PROJ).BSC

DOS.OBJ:	DOS.C $(DOS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DOS.C

CPLUS.OBJ:	..\SOURCE\CPLUS.C $(CPLUS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\CPLUS.C

QACTIVE.OBJ:	..\SOURCE\QACTIVE.C $(QACTIVE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QACTIVE.C

QEPOOL.OBJ:	..\SOURCE\QEPOOL.C $(QEPOOL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QEPOOL.C

QEQUEUE.OBJ:	..\SOURCE\QEQUEUE.C $(QEQUEUE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QEQUEUE.C

QF.OBJ:	..\SOURCE\QF.C $(QF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QF.C

QFSM.OBJ:	..\SOURCE\QFSM.C $(QFSM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QFSM.C

QHSM.OBJ:	..\SOURCE\QHSM.C $(QHSM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QHSM.C

QHSMTRAN.OBJ:	..\SOURCE\QHSMTRAN.C $(QHSMTRAN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QHSMTRAN.C

QTIMER.OBJ:	..\SOURCE\QTIMER.C $(QTIMER_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\SOURCE\QTIMER.C

$(PROJ).LIB::	DOS.OBJ CPLUS.OBJ QACTIVE.OBJ QEPOOL.OBJ QEQUEUE.OBJ QF.OBJ QFSM.OBJ \
	QHSM.OBJ QHSMTRAN.OBJ QTIMER.OBJ $(OBJS_EXT)
	echo >NUL @<<$(PROJ).CRF
$@ /PAGESIZE:64
y
+DOS.OBJ &
+CPLUS.OBJ &
+QACTIVE.OBJ &
+QEPOOL.OBJ &
+QEQUEUE.OBJ &
+QF.OBJ &
+QFSM.OBJ &
+QHSM.OBJ &
+QHSMTRAN.OBJ &
+QTIMER.OBJ &
;
<<
	if exist $@ del $@
	lib @$(PROJ).CRF

$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
