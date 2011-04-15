/*$Id: HostResolver.cpp 312 2008-12-15 06:05:46Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include "HostResolver.h"

#include <arpa\inet.h>
#include <netinet\in.h>

CHostResolver::CHostResolver(RConnection& aConnect, RSocketServ& aSockSrv, MClient& aClient) :
	CActive(EPriorityStandard) // Standard priority
	, iConnect(aConnect), iSocketServ(aSockSrv), iClient(aClient)
{
}

CHostResolver* CHostResolver::NewLC(RConnection& aConnect, RSocketServ& aSockSrv, MClient& aClient)
{
	CHostResolver* self = new ( ELeave ) CHostResolver(aConnect, aSockSrv, aClient);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CHostResolver* CHostResolver::NewL(RConnection& aConnect, RSocketServ& aSockSrv, MClient& aClient)
{
	CHostResolver* self = CHostResolver::NewLC(aConnect, aSockSrv, aClient);
	CleanupStack::Pop(); // self;
	return self;
}

void CHostResolver::ConstructL()
{
	
	CActiveScheduler::Add( this); // Add to scheduler
}

CHostResolver::~CHostResolver()
{
	Cancel(); // Cancel any request, if outstanding
	iHostResolver.Close(); // Destroy the RTimer object
	// Delete instance variables if any
}

void CHostResolver::DoCancel()
{
	iHostResolver.Cancel();
}

void CHostResolver::GetByNameL(const TDesC &aName)
{
	if (IsActive())
	{
		Cancel(); // Cancel any request, just to be sure
		iHostResolver.Close();
	}	
	User::LeaveIfError( iHostResolver.Open(iSocketServ, KAfInet, KProtocolInetTcp, iConnect) );
	iHostResolver.GetByName(aName, iNameEntry, iStatus);
	SetActive(); // Tell scheduler a request is active
}

void CHostResolver::RunL()
{
	iClient.HostResolvedL(iStatus.Int(), iNameEntry);
	iHostResolver.Close(); 
}

TInt CHostResolver::RunError(TInt aError)
{
	return aError;
}
TBool CHostResolver::IsIpAddr(const TDesC& aName)
{
	RBuf8 buf;
	buf.Create(aName.Length() + 1);
	buf.Copy(aName);
	buf.PtrZ();
	TBool ret = !(inet_addr((const char*)buf.PtrZ()) == INADDR_NONE);
	buf.Close();
	return ret;
}
