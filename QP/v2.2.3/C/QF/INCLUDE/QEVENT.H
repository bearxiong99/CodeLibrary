/******************************************************************** 
 * Quantum Event declarations ("C+" version)
 * Copyright (c) 2002 Miro Samek, Palo Alto, CA.
 * All Rights Reserved.
 *******************************************************************/
#ifndef qevent_h
#define qevent_h

#ifndef cplus_h
#include "cplus.h"
#endif

typedef unsigned short QSignal;

CLASS(QEvent)
   QSignal sig;                    /* signal of the event instance */
   unsigned char poolId;    /* pool this event instance comes from */
   unsigned char useNum;     /* # of times it has been used so far */
METHODS
END_CLASS

enum {                                         /* standard signals */
   Q_INIT_SIG = 1,
   Q_ENTRY_SIG,
   Q_EXIT_SIG,
   Q_USER_SIG
};

#endif                                                   /* qevt_h */
