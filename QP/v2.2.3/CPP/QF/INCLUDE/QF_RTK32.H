/////////////////////////////////////////////////////////////////////
// Quantum Framework RTK-32-specific declarations (C++ version)
// Copyright (c) 2002 Miro Samek, Palo Alto, CA. 
// All Rights Reserved.
/////////////////////////////////////////////////////////////////////
#ifndef qf_rtk32_h
#define qf_rtk32_h

#include <rtk32.h>

// RTK-32-specific event queue and thread types
#define QF_EQUEUE(x_) RTKMailbox    x_;
#define QF_THREAD(x_) RTKTaskHandle x_;
#define Q_STATIC_CAST(type_, expr_) static_cast<type_>(expr_)

#include "qevent.h"
#include "qhsm.h"
#include "qactive.h"
#include "qtimer.h"
#include "qf.h"

#endif                                                  // qf_rtk32_h
