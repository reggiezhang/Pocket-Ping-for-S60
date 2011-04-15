/*$Id: ApConnectEngine.h 329 2008-12-20 20:03:54Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */


#ifndef IAPSELECTOR_H
#define IAPSELECTOR_H 

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <e32cmn.h>
#include <CommDbConnPref.h>
#include <es_sock.h>
#include <cdbcols.h>
#include <commdb.h>
#include <cdbtemp.h>
#include <commdbconnpref.h>
#include "ApConnectStateObserver.h"

// CONSTANTS
const TUint32 KUndefinedApId = 0x00;
struct TApConnectionInfo
{
	TUint32 apNo;
	TBuf<255> apName;
	TBuf<255> ipAddress;
};

class CApConnectProgressNotifier;

class CApConnectEngine : public CActive, MApConnectStateObserver
{
public:
	// Cancel and destroy
	~CApConnectEngine();

	// Two-phased constructor.
	static CApConnectEngine* NewL();

	// Two-phased constructor.
	static CApConnectEngine* NewLC();

	RConnection& GetRConnection()
	{
		return iConnect;
	}
	RSocketServ& GetRSocketServ()
	{
		return iSocketServ;
	}
	TApConnectionInfo& GetApConnectionInfo()
	{
		return iConnInfo;
	}
	
	// from MApConnectStateObserver
	void ChangeStateL(TInt aState);
	
	void TerminateConnectionL();
	
	void AddObserverL(MApConnectStateObserver* aObserver)
	{
		iObservers.AppendL(aObserver);
	}

public:
	// New functions
	void ConnectL();
private:
	// C++ constructor
	CApConnectEngine();

	// Second-phase constructor
	void ConstructL();

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);
	
	void NotifyObserverL(TInt aStatusCode);
	void FetchConnectInfoL();
	void FetchIpAddressL();
	void SetStatusTextL(TInt aResId);

private:
	TCommDbConnPref iPref;
	RConnection iConnect;
    RSocketServ iSocketServ;
    TApConnectionInfo iConnInfo;
    CApConnectProgressNotifier* iProgressNotifier;
    RPointerArray<MApConnectStateObserver> iObservers;
};

#endif // IAPSELECTOR_H
