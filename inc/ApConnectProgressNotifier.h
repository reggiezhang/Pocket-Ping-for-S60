/*$Id: ApConnectProgressNotifier.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef APCONNECT_PROGRESSNOTIFIER_H
#define APCONNECT_PROGRESSNOTIFIER_H

#include "ApConnectStateObserver.h"

#include <e32base.h>
#include <Es_sock.h>

class CApConnectProgressNotifier : public CActive
    {
public:

    static CApConnectProgressNotifier* 
        NewL(MApConnectStateObserver* aObserver, RConnection& aConnection);

    static CApConnectProgressNotifier* 
        NewLC(MApConnectStateObserver* aObserver, RConnection& aConnection);

    ~CApConnectProgressNotifier();

    void SetState(TInt aState);

    TInt GetState() const;

protected: // from CActive

    void DoCancel();

    void RunL();
    
private:
    
    CApConnectProgressNotifier(MApConnectStateObserver* aObserver, 
        RConnection& aConnection);

    void ConstructL();

private:

    // Member variables
    
    TNifProgressBuf iProgress;
    TInt iState;
    
    RConnection& iConnection;

    MApConnectStateObserver* iObserver;    
    
    };

#endif // APCONNECT_PROGRESSNOTIFIER_H

