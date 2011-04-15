/*$Id: HostResolver.h 312 2008-12-15 06:05:46Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef HOSTRESOLVER_H
#define HOSTRESOLVER_H 

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib

#include <es_sock.h>
#include <in_sock.h>


class CHostResolver : public CActive
{
public:
	class MClient
	{
	public:
		virtual void HostResolvedL(TInt aError, TNameEntry& aResult) = 0;
	};
	// Cancel and destroy
	~CHostResolver();

	// Two-phased constructor.
	static CHostResolver* NewL(RConnection& aConnect, RSocketServ& aSockSrv, MClient& aClient);

	// Two-phased constructor.
	static CHostResolver* NewLC(RConnection& aConnect, RSocketServ& aSockSrv, MClient& aClient);

public:
	// New functions
	// Function for making the initial request
	void GetByNameL(const TDesC& aName);
	
	static TBool IsIpAddr(const TDesC& aName); 

private:
	// C++ constructor
	CHostResolver(RConnection& aConnect, RSocketServ& aSockSrv, MClient& aClient);

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

private:
	RConnection& iConnect; // not own
    RSocketServ& iSocketServ; // not own
    MClient& iClient;
    RHostResolver iHostResolver; // own
    TNameEntry iNameEntry;
};

#endif // HOSTRESOLVER_H
