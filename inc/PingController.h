/*$Id: PingController.h 329 2008-12-20 20:03:54Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PINGUTILITY_H
#define PINGUTILITY_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <es_sock.h>
#include <in_sock.h>
#include "PingSettings.h"
#include "TimeOutTimer.h"
class MPingControllerObserver;
class CEchoPacket;
class CPingController : public CActive, MTimeoutNotifier
{
public:
	// Cancel and destroy
	~CPingController();

	// Two-phased constructor.
	static CPingController* NewL(RConnection& aConnect, RSocketServ& aSockSrv, MPingControllerObserver* aObserver);

	// Two-phased constructor.
	static CPingController* NewLC(RConnection& aConnect, RSocketServ& aSockSrv, MPingControllerObserver* aObserver);

public:
	// New functions
	// Function for making the initial request
	void PingL(const TDesC& aRemote, const TPingSettings& aSettings);
	void CancelPingL();
	void ShowIntermediateStat(TBool aPause = ETrue);
	void ResumePingL();

private:
	// C++ constructor
	CPingController(RConnection& aConnect, RSocketServ& aSockSrv, MPingControllerObserver* aObserver);

	// Second-phase constructor
	void ConstructL();
	
	// from MTimeoutNotifier
	void TimerExpiredL();

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);
		
	TInt64 GetTime();
	
	void WaitingForReplyL();

private:
	enum TPingControllerState
	{
		ESendTo, 
		ERecvFrom,
		EError // Error condition
	};
	
private:
	enum TPingState
	{
		EIdle,
		EWaitingForReply,
		EPinging,
		EPause
	};
	enum TPingEvent
	{
		EPing,
		EReplyReceived,
		ELastReplyReceived,
		ETimeOut,
		ELastReplyTimeOut,
		EShowIntermediateStat,
		EShowIntermediateStatAndPause
	};
	typedef void (CPingController::*Action)();
	class TTransition
	{
	public:
		TTransition(TInt aCurrentState, TInt aEvent, TInt aNewState, Action aAction)
		: currentState(aCurrentState)
		, event(aEvent)
		, newState(aNewState)
		, action(aAction) {}
		
		TInt currentState;
		TInt event;
		TInt newState;
		Action action;
	};
	struct TPingStat
	{
	  TInt sent;
	  TInt received;
	  TInt min;
	  TInt max;
	  TInt sum;
	};
	void InitTransitionsL();
	// actions
	void SendFirstRequestL();
	void NotifyResultL();
	void NotifyLastResultL();
	void SendNextRequestL();
	void NotifyIntermediateStatL();
	void NotifyIntermediateStatAndPauseL();
	
	void EventL(TInt aEvent);
	
	void InitSocket();
	/*
	 *  return TBool, true means decode successful, false means need continue wait for reply
	 */
	TBool DecodeRespL();
	void DoSend();
	void StatL();
	void ClearReplyText();
	void NotifyStatL();

private:
	RConnection& iConnect; // not own
    RSocketServ& iSocketServ; // not own
    RSocket iSocket; // own
    MPingControllerObserver* iObserver;
    TInt iNrOfPing;
    TInt iPingCount;
	TInt iDataSize;
	TInt iTtl;
	TBool iDontFragment;
	TUint iSendFlag;
	TInt iPingControllerState; // State of the active object
	TInt iPingState;
	RBuf iRemote; // own
    TInetAddr iRemoteAddr;
    TBuf<255> iRecv;
    TInetAddr iRecvAddr;
    RArray<TTransition> iTransitions;
    CEchoPacket* iIcmpPacket;
    RBuf8 iReplyBuf;
    CTimeOutTimer* iTimer;
    TInt iPingIntervalTime;
    HBufC* iReplyText;
    TPingStat iStat;
    TInt iTimestamp; // calc the roundtrip time
    TInt iShowIntermediateStat;
};

#endif // PINGUTILITY_H
