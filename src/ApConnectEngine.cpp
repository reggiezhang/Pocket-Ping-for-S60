/*$Id: ApConnectEngine.cpp 343 2009-05-14 15:56:18Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include <in_sock.h>
#include <es_enum.h>
#include <stringloader.h>
#include <aknglobalnote.h>
#include "ApConnectEngine.h"
#include "ApConnectProgressNotifier.h"
#include "PktPing.rsg"
_LIT( KTextNotReady, "Active request already ongoing" );
_LIT( KZeroIP, "0.0.0.0" );
_LIT( KLocalIP, "127.0.0.1" );

CApConnectEngine::CApConnectEngine() :
	CActive(EPriorityStandard) // Standard priority
{
}

CApConnectEngine* CApConnectEngine::NewLC()
{
	CApConnectEngine* self = new ( ELeave ) CApConnectEngine();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CApConnectEngine* CApConnectEngine::NewL()
{
	CApConnectEngine* self = CApConnectEngine::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CApConnectEngine::ConstructL()
{
	iConnInfo.apNo = KUndefinedApId;
	iConnInfo.apName.Zero();
	iConnInfo.ipAddress.Zero();
	iPref.SetDirection(ECommDbConnectionDirectionOutgoing);
	iPref.SetDialogPreference(ECommDbDialogPrefPrompt);
	iPref.SetIapId(KUndefinedApId);
	iPref.SetBearerSet(KCommDbBearerUnknown);
	User::LeaveIfError(iSocketServ.Connect());
    User::LeaveIfError(iConnect.Open(iSocketServ));
    iProgressNotifier = CApConnectProgressNotifier::NewL(this, iConnect);
	CActiveScheduler::Add( this); // Add to scheduler
}

CApConnectEngine::~CApConnectEngine()
{
	Cancel(); // Cancel any request, if outstanding
	if (iProgressNotifier)
	{
		delete iProgressNotifier;
		iProgressNotifier = NULL;		
	}
	iConnect.Close();
    iSocketServ.Close();
    iObservers.Close();
}
void CApConnectEngine::ConnectL()
{
	if (iProgressNotifier->GetState() == EApConnectStateConnected)
	{
		return;  // Already connected, do nothing
	}
	if (!IsActive())
	{ 
		iConnInfo.apNo = KUndefinedApId;
		iConnInfo.apName.Zero();
		//iConnInfo.ipAddress.Zero();
		SetStatusTextL(R_PKTPING_PLEASE_WAIT);
	    iConnect.Start(iPref, iStatus);
		SetActive();
	}
	else
	{
		//
	}
}
void CApConnectEngine::DoCancel()
{
}


void CApConnectEngine::RunL()
{
	TInt statusCode = iStatus.Int();	
	if (statusCode != KErrNone)
	{
		ChangeStateL(EApConnectStateNotConnected);
	}
}

void CApConnectEngine::FetchConnectInfoL()
{
	TUint connCount = 0;
    // Count active connections
    iConnect.EnumerateConnections(connCount);
    if (!connCount) return;
    // Get connection info
    TPckgBuf<TConnectionInfo> connInfo;
    User::LeaveIfError(iConnect.GetConnectionInfo(1, connInfo));
    iConnInfo.apNo = connInfo().iIapId;
}
void CApConnectEngine::FetchIpAddressL()
{
	RSocket socket;
	CleanupClosePushL( socket);
	while (socket.Open(iSocketServ, KAfInet, KSockDatagram, KProtocolInetUdp,
			iConnect)!= KErrNone)
	{
	};
	TSoInetInterfaceInfo inf;
	TPckg<TSoInetInterfaceInfo> opt( inf);
	TRAPD(err, socket.SetOpt (KSoInetEnumInterfaces, KSolInetIfCtrl ))
	;
	if (err != KErrNone)
	{
		return;
	}

	while ( socket.GetOpt(KSoInetNextInterface, KSolInetIfCtrl, opt)== KErrNone)
	{
		if ( inf.iAddress.Address()==0) // if the stored address is not an IPv4 address
		{
		}
		else
		{
			if ( inf.iAddress.IsUnspecified())
			{
				// Interface inf.iName has no address        	
			}
			else
			{
				// have to do this, strange
				TInetAddr addr;
				addr.SetAddress( inf.iAddress.Address());
				addr.Output(iConnInfo.ipAddress);
				// have to do this, strange

				if ( iConnInfo.ipAddress.Compare(KZeroIP)!= 0 && iConnInfo.ipAddress.Compare(KLocalIP)!= 0)
				{
					if ( iConnInfo.ipAddress.Left(3).Compare(_L("169"))!= 0)
					{
						CleanupStack::PopAndDestroy(); // socket
						return;
					}
				}
			}
		}
	}
	CleanupStack::PopAndDestroy(); // socket	
}

void CApConnectEngine::NotifyObserverL(TInt aStatusCode)
{
	for (TInt i = 0; i < iObservers.Count(); ++i)
	{
		iObservers[i]->ChangeStateL(aStatusCode);
	}
}

TInt CApConnectEngine::RunError(TInt aError)
{
	return aError;
}

void CApConnectEngine::ChangeStateL(TInt aState)
{
	TInt resId = KErrNone;
	switch (aState)
	{
	case EApConnectStateNotConnected:
		resId = R_PKTPING_DISCONNECTED;
		break;
	case EApConnectStateSelecting:
		resId = R_PKTPING_PLEASE_WAIT;
		break;
	case EApConnectStateConnecting:
		resId = R_PKTPING_CONNECTING;
		break;
	case EApConnectStateConnected:
		//resId = R_PKTPING_CONNECTED;
		FetchConnectInfoL();
		FetchIpAddressL();
		break;
	case EApConnectStateDisconnecting:
		resId = R_PKTPING_DISCONNECTED;
		break;
	case EApConnectStateUnknown:
		resId = R_PKTPING_UNKNOWN_STATE;
	default:
		break;
	}
	SetStatusTextL(resId);
	NotifyObserverL(aState);
}
void CApConnectEngine::SetStatusTextL(TInt aResId)
{
	if (aResId == KErrNone) return;
	HBufC* text = StringLoader::LoadLC(aResId);
	iConnInfo.ipAddress = *text;
	CleanupStack::PopAndDestroy(text);
}
void CApConnectEngine::TerminateConnectionL()
{
	if (iProgressNotifier->GetState() == EApConnectStateConnected)
	{
		Cancel();
		if (iProgressNotifier)
        {
			delete iProgressNotifier;
	        iProgressNotifier = NULL;
        }
		iConnect.Close();
		// reopen the connection
	    User::LeaveIfError(iConnect.Open(iSocketServ));
	    iProgressNotifier = CApConnectProgressNotifier::NewL(this, iConnect);
	}
}
