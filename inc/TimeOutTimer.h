/*$Id: TimeOutTimer.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef TIME_OUT_TIMER_H
#define TIME_OUT_TIMER_H
#include <e32base.h>


class MTimeoutNotifier
    {
public:
    virtual void TimerExpiredL() = 0;
    };

class CTimeOutTimer : public CTimer
    {
public:
    static CTimeOutTimer* NewL( const TInt aPriority,
                                MTimeoutNotifier& aNotify );

    static CTimeOutTimer* NewLC( const TInt aPriority,
                                 MTimeoutNotifier& aNotify );

    virtual ~CTimeOutTimer();

protected: // from CTimer

    void RunL();

    TInt RunError( TInt aError );

private:

    CTimeOutTimer( const TInt aPriority,
                   MTimeoutNotifier& aNotify);

    void ConstructL();

private:

    MTimeoutNotifier&               iNotify;
    };

#endif // TIME_OUT_TIMER_H
