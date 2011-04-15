/*$Id: PingController.cpp 338 2008-12-22 14:29:31Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */
  
#include <e32std.h>
#include <sys\types.h>
#include <netinet\in.h>
#include <stringloader.h> 
#include "PingControllerObserver.h"
#include "PingController.h"
#include "IcmpPacket.h"
#include "pktping.rsg"
#include "pktping.hrh"

const TInt KTimeOut = 2000000; // 1 sec
const TInt KDefaultPingIntervalTime = 1000000; // 1 sec
const TInt KMaxDigitsInInteger = 10;

const TInt KShowStatAndPause = 1;
const TInt KShowStatNoPause = 2;

CPingController::CPingController(RConnection& aConnect, RSocketServ& aSockSrv, MPingControllerObserver* aObserver) :
	CActive(EPriorityStandard) // Standard priority
	, iConnect(aConnect), iSocketServ(aSockSrv), iObserver(aObserver), iPingIntervalTime(KDefaultPingIntervalTime)
{
}

CPingController* CPingController::NewLC(RConnection& aConnect, RSocketServ& aSockSrv, MPingControllerObserver* aObserver)
{
	CPingController* self = new ( ELeave ) CPingController(aConnect, aSockSrv, aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CPingController* CPingController::NewL(RConnection& aConnect, RSocketServ& aSockSrv, MPingControllerObserver* aObserver)
{
	CPingController* self = CPingController::NewLC(aConnect, aSockSrv, aObserver);
	CleanupStack::Pop(); // self;
	return self;
}

void CPingController::ConstructL()
{
	InitTransitionsL();
	iTimer = CTimeOutTimer::NewL( EPriorityHigh, *this);
	CActiveScheduler::Add( this); // Add to scheduler
}
void CPingController::InitTransitionsL()
{
	//                              State             Event                          New State         Action
	iTransitions.Append(TTransition(EIdle,            EPing,                         EWaitingForReply, &CPingController::SendFirstRequestL));
	iTransitions.Append(TTransition(EWaitingForReply, EReplyReceived,                EPinging,         &CPingController::NotifyResultL));
	iTransitions.Append(TTransition(EPinging,         EPing,                         EWaitingForReply, &CPingController::SendNextRequestL));
	iTransitions.Append(TTransition(EWaitingForReply, ELastReplyReceived,            EIdle,            &CPingController::NotifyLastResultL));
	iTransitions.Append(TTransition(EWaitingForReply, ETimeOut,                      EPinging,         &CPingController::NotifyResultL));
	iTransitions.Append(TTransition(EWaitingForReply, ELastReplyTimeOut,             EIdle,            &CPingController::NotifyLastResultL));	
	iTransitions.Append(TTransition(EPinging,         EShowIntermediateStat,         EPinging,         &CPingController::NotifyIntermediateStatL));
	iTransitions.Append(TTransition(EPinging,         EShowIntermediateStatAndPause, EPause,           &CPingController::NotifyIntermediateStatAndPauseL));
	iTransitions.Append(TTransition(EPause,           EPing,                         EWaitingForReply, &CPingController::SendNextRequestL));
}
void CPingController::InitSocket()
{
	iSocket.Close();
	while (iSocket.Open(iSocketServ, KAfInet, KSockDatagram, KProtocolInetIcmp,
				iConnect)!= KErrNone)
	{
		User::After(1000);
	};
	iSocket.SetOpt(KSoIpTTL, KSolInetIp, iTtl);
}
// actions
void CPingController::SendFirstRequestL()
{
	iPingCount = 0;
	iPingControllerState = ESendTo;
	InitSocket();

	if (iRemoteAddr.Input(iRemote)!= KErrNone)
	{
		iObserver->NotifyErrorL(KErrArgument);
		return; //?
	}
	delete iIcmpPacket;
	iIcmpPacket = CEchoPacket::NewL(GetTime(), iDataSize);
	
	DoSend();
}
void CPingController::SendNextRequestL()
{
	iPingControllerState = ESendTo;
	iIcmpPacket->Next();
	DoSend();	
}

void CPingController::DoSend()
{
	iTimestamp = User::NTickCount();
	iSocket.SendTo(iIcmpPacket->Des()
			, iRemoteAddr
			, iDontFragment ? KIpDontFragment : 0
			, iStatus);
	SetActive(); // Tell scheduler a request is active
}
void CPingController::ClearReplyText()
{
	delete iReplyText;
	iReplyText = NULL;
}
void CPingController::NotifyStatL()
{
	StatL();
	iObserver->NotifyPingProgressL(*iReplyText);
	ClearReplyText();
}
void CPingController::NotifyIntermediateStatL()
{
	NotifyStatL();
	User::After(iPingIntervalTime); //interval 1sec
	EventL(EPing);
}
void CPingController::NotifyIntermediateStatAndPauseL()
{
	NotifyStatL();
}
void CPingController::NotifyResultL()
{
	iObserver->NotifyPingProgressL(*iReplyText);
	ClearReplyText();
	
	if (iShowIntermediateStat == KShowStatAndPause)
	{
		iShowIntermediateStat = 0;
		EventL(EShowIntermediateStatAndPause);
	}
	else if (iShowIntermediateStat == KShowStatNoPause)
	{
		iShowIntermediateStat = 0;
		EventL(EShowIntermediateStat);
	}
	else
	{
		User::After(iPingIntervalTime); //interval 1sec
		EventL(EPing);
	}
}
void CPingController::NotifyLastResultL()
{
	iObserver->NotifyPingProgressL(*iReplyText);
	ClearReplyText();
	NotifyStatL();
	iObserver->NotifiyEndL();
}

void CPingController::EventL(TInt aEvent)
{
	for(TInt i = 0; i < iTransitions.Count(); i++)
	{
		if (iPingState == iTransitions[i].currentState && aEvent == iTransitions[i].event)
		{
			iPingState = iTransitions[i].newState;
			Action func = iTransitions[i].action;
			(this->*func)();
		}
	}
}
CPingController::~CPingController()
{
	Cancel(); // Cancel any request, if outstanding
	// Delete instance variables if any
	delete iTimer;
	delete iIcmpPacket;
	ClearReplyText();
	iReplyBuf.Close();
	iRemote.Close();
	iTransitions.Close();
	iSocket.Close();
}

void CPingController::DoCancel()
{
	iSocket.CancelAll();
	iTimer->Cancel();
}

void CPingController::PingL(const TDesC& aRemote, const TPingSettings& aSettings)
{
	if (IsActive())
	{
		// notify 
		iObserver->NotifyErrorL(KErrNotReady);
		return;
	}
	iShowIntermediateStat = 0;
	iPingState = EIdle;
	iRemote.Close();
	iRemote.CreateL( aRemote.Length());
	iRemote.Copy( aRemote);
	iNrOfPing = aSettings.NrOfPing();
	iDataSize = aSettings.DataSize();
	iTtl = aSettings.Ttl();
	iDontFragment = aSettings.DontFragment();
	if (iDontFragment)
	{
		iSendFlag |= KIpDontFragment;
	}
	iStat.sent = 0;
	iStat.received = 0;
	iStat.min = KMaxTInt;
	iStat.max = 0;
	iStat.sum = 0;
	EventL(EPing);
}

void CPingController::RunL()
{
	iTimer->Cancel();
	if (iPingControllerState == ESendTo)
	{
		if (iStatus.Int() == KErrNone)
		{
			++(iStat.sent);
			WaitingForReplyL();
		}
		else
		{
			iObserver->NotifyErrorL(iStatus.Int());
//			TInt err = iStatus.Int();
//			TPckgBuf<TSoInetLastErr> pckgBuf;
//			if (iSocket.GetOpt(KSolInetIp, KSoInetLastError, pckgBuf) == KErrNone)
//			{
//				TSoInetLastErr p = pckgBuf();
//				if (p.iErrAddr.Family() == KAfInet || p.iErrAddr.IsV4Mapped())
//				{
//					TInt type = p.iErrType;
//					TInt code = p.iErrCode;
//				}
//				
//				iObserver->NotifyError(err);
//			}
//			else
//			{
//				iObserver->NotifyError(err);
//			}
		}
	}
	else if (iPingControllerState == ERecvFrom)
	{
		if (iStatus.Int() == KErrNone)
		{	
			if (!DecodeRespL())
			{
				WaitingForReplyL();
				return;
			}
			if (++iPingCount < iNrOfPing)
			{
				EventL(EReplyReceived);
			}
			else
			{
				EventL(ELastReplyReceived);
			}
		}
		else
		{
			iObserver->NotifyErrorL(iStatus.Int());
		}
		
	}
}

TInt CPingController::RunError(TInt aError)
{
	return aError;
}

void CPingController::WaitingForReplyL()
{
	iPingControllerState = ERecvFrom;
	iReplyBuf.Close();
	iReplyBuf.CreateL(iIcmpPacket->Des().Size() + sizeof(TIpHdr));
	iSocket.RecvFrom(iReplyBuf, iRecvAddr, KIpHeaderIncluded, iStatus);
	SetActive();
	iTimer->After( KTimeOut); // Set for 1 sec later
}
TInt64 CPingController::GetTime()
{
	TTime currentTime;
	currentTime.HomeTime();
	return currentTime.Int64() / 1000;
}
void CPingController::TimerExpiredL()
{
	Cancel();
	iReplyText = StringLoader::LoadL(R_PKTPING_REQUEST_TIMED_OUT);	
	if (++iPingCount < iNrOfPing)
	{
		EventL(ETimeOut);
	}
	else
	{
		EventL(ELastReplyTimeOut);
	}
}
void CPingController::CancelPingL()
{
	Cancel();
	if (iPingState != EIdle && iPingState != EPause)
	{
		NotifyStatL();
	}
	iPingState = EIdle;
}
void CPingController::ResumePingL()
{
	iShowIntermediateStat = 0;
	if (iPingState == EPause)
	{	
		EventL(EPing);
	}
}
TBool CPingController::DecodeRespL()
{
	// Reply from %S: bytes=%D time=%Dms TTL=%D
	// time
	TUint32 triptime = User::NTickCount() - iTimestamp;//icmpHdr->timestamp;
	
	const TUint8* reply = iReplyBuf.Ptr();
	const TIpHdr* ipHdr = (const TIpHdr*)reply;
	const TIcmpHdr* icmpHdr = (const TIcmpHdr*)(reply + ipHdr->ip_hl * 4);
	if (ntohs(icmpHdr->icd_id) != ICMP_ID)
	{
		iReplyText = StringLoader::LoadL(R_PKTPING_SOMEONE_ELSE_PACKET_RECEIVED);
		return EFalse;
	}
	if (icmpHdr->icmp_type != ICMP_ECHOREPLY)
	{
		iReplyText = StringLoader::LoadL(R_PKTPING_NONE_ECHO_TYPE_RECEIVED_FORMAT, icmpHdr->icmp_type);
		return ETrue;
	}
	if (ntohs(icmpHdr->icd_seq) != iIcmpPacket->SeqNo())
	{
		iReplyText = StringLoader::LoadL(R_PKTPING_OUT_OF_ORDER_PACKET_RECEIVED);
		return EFalse;
	}
	// from
	iRecv.Zero();
	iRecvAddr.Output(iRecv);	
	// bytes
	TUint16 bytes = ntohs(ipHdr->ip_len) - ipHdr->ip_hl * 4 - ICMP_HDR_LEN;
	// TTL
	TUint8 ttl = ipHdr->ip_ttl;
	
	CDesCArrayFlat* strings = new (ELeave) CDesCArrayFlat( 1 );
	CleanupStack::PushL( strings );
	strings->AppendL(iRecv);
	CArrayFix<TInt>* vals = new (ELeave) CArrayFixFlat<TInt>(3);
	CleanupStack::PushL( vals );
	vals->AppendL(bytes);
	vals->AppendL(triptime);
	vals->AppendL(ttl);

	iReplyText = StringLoader::LoadL(R_PKTPING_REPLY_TEXT_FORMAT, *strings, *vals);
	CleanupStack::PopAndDestroy(2); // strings, vals
	
	//stat
	iStat.sum += triptime;
	++(iStat.received);
	iStat.min = Min(iStat.min, triptime);
	iStat.max = Max(iStat.max, triptime);
	return ETrue;
}
void CPingController::StatL()
{
	CArrayFix<TInt>* vals = new (ELeave) CArrayFixFlat<TInt>(7);
	CleanupStack::PushL( vals );
	vals->AppendL(iStat.sent); 
	vals->AppendL(iStat.received);
	vals->AppendL(iStat.sent - iStat.received);
	vals->AppendL((iStat.sent - iStat.received) * 100 / iStat.sent);
	if (iStat.received)
	{
		vals->AppendL(iStat.min);
		vals->AppendL(iStat.max);
		vals->AppendL(iStat.sum / iStat.received);
		iReplyText = StringLoader::LoadL(R_PKTPING_STAT_FORMAT, *vals);
	}
	else
	{
		iReplyText = StringLoader::LoadL(R_PKTPING_STAT_SHORT_FORMAT, *vals);
	}
}
void CPingController::ShowIntermediateStat(TBool aPause)
{
	iShowIntermediateStat = aPause ? KShowStatAndPause : KShowStatNoPause;
}
