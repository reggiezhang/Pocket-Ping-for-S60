/*$Id: ApConnectProgressNotifier.cpp 338 2008-12-22 14:29:31Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include "ApConnectProgressNotifier.h"
#include <in_iface.h>
#include <aknglobalnote.h>

CApConnectProgressNotifier* CApConnectProgressNotifier::NewL(
		MApConnectStateObserver* aObserver, RConnection& aConnection)
{
	CApConnectProgressNotifier* self = NewLC(aObserver, aConnection);
	CleanupStack::Pop(self);
	return self;
}

CApConnectProgressNotifier* CApConnectProgressNotifier::NewLC(
		MApConnectStateObserver* aObserver, RConnection& aConnection)
{
	CApConnectProgressNotifier* self = new (ELeave) CApConnectProgressNotifier(aObserver, aConnection);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CApConnectProgressNotifier::CApConnectProgressNotifier(
		MApConnectStateObserver* aObserver, RConnection& aConnection) :
	CActive(CActive::EPriorityStandard), iConnection(aConnection),
			iObserver(aObserver)
{
	CActiveScheduler::Add(this);
}

CApConnectProgressNotifier::~CApConnectProgressNotifier()
{
	Cancel();
}

void CApConnectProgressNotifier::ConstructL()
{
//	iObserver->ChangeStateL(iPingControllerState);
	iConnection.ProgressNotification(iProgress, iStatus);
	SetActive();
}

void CApConnectProgressNotifier::DoCancel()
{
	iConnection.CancelProgressNotification();
}

void CApConnectProgressNotifier::RunL()
{
	switch (iProgress().iStage)
	{
	case KConnectionUninitialised:
		if (iState == EApConnectStateNotConnected) 
		{
			break;
		}
		else
		{
			iState = EApConnectStateNotConnected;
			iObserver->ChangeStateL(iState);
			break;
		}

	// Starting connetion selection
	case KStartingSelection:
		iState = EApConnectStateSelecting;
		iObserver->ChangeStateL(iState);
		break;
	// Selection finished
	case KFinishedSelection:
		if (iProgress().iError == KErrNone)
		{
			// The user successfully selected an IAP to be used
			iState = EApConnectStateConnecting;
			iObserver->ChangeStateL(iState);
		}
		else
		{
			iState = EApConnectStateNotConnected;
			iObserver->ChangeStateL(iState);
		}
		break;

	case KConnectionFailure:
		if (iState == EApConnectStateNotConnected) 
		{
			break;
		}
		else
		{
			iState = EApConnectStateNotConnected;
			iObserver->ChangeStateL(iState);
			break;
		}
	case KPsdStartingConfiguration:
	case KPsdFinishedConfiguration:
		iState = EApConnectStateConnecting;
		iObserver->ChangeStateL(iState);
		break;

	case KConnectionOpen:
	case KLinkLayerOpen:

		iState = EApConnectStateConnected;
		iObserver->ChangeStateL(iState);
		break;

	case KDataTransferTemporarilyBlocked:

		iState = EApConnectStateConnected;
		iObserver->ChangeStateL(iState);
		break;

	case KConnectionStartingClose:

		iState = EApConnectStateDisconnecting;
		iObserver->ChangeStateL(iState);
		break;

	case KConnectionClosed:
	case KLinkLayerClosed:
		if (iState == EApConnectStateNotConnected) 
		{
			break;
		}
		else
		{
			iState = EApConnectStateNotConnected;
			iObserver->ChangeStateL(iState);
			break;
		}
	default:
//		iPingControllerState = EApConnectStateUnknown;
//		iObserver->ChangeStateL(iPingControllerState);
		break;
	}

	iConnection.ProgressNotification(iProgress, iStatus);
	SetActive();
}

void CApConnectProgressNotifier::SetState(TInt aState)
{
	iState=aState;
}

TInt CApConnectProgressNotifier::GetState() const
{
	return iState;
}
