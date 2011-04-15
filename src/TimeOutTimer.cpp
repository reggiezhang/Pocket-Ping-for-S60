/*$Id: TimeOutTimer.cpp 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include "TimeOutTimer.h"

// ================= MEMBER FUNCTIONS =======================

// Constructor
CTimeOutTimer::CTimeOutTimer( const TInt aPriority, MTimeoutNotifier& aNotify) :
	CTimer( aPriority), iNotify( aNotify)
{

}

// Destructor
CTimeOutTimer::~CTimeOutTimer()
{
}

// ----------------------------------------------------
// CTimeOutTimer::ConstructL()
// Second-phase constructor
// ----------------------------------------------------
//
void CTimeOutTimer::ConstructL()
{
	CTimer::ConstructL();
	CActiveScheduler::Add( this);
}

// ----------------------------------------------------
// CTimeOutTimer::NewL()
// Two-phased construction.
// ----------------------------------------------------
//
CTimeOutTimer* CTimeOutTimer::NewL( const TInt aPriority,
		MTimeoutNotifier& aNotify)
{
	CTimeOutTimer* self = CTimeOutTimer::NewLC( aPriority, aNotify);
	CleanupStack::Pop( self);
	return self;
}

// ----------------------------------------------------
// CTimeOutTimer::NewLC()
// Two-phased construction.
// ----------------------------------------------------
//    
CTimeOutTimer* CTimeOutTimer::NewLC( const TInt aPriority,
		MTimeoutNotifier& aNotify)
{
	CTimeOutTimer* self = new (ELeave) CTimeOutTimer( aPriority, aNotify );
	CleanupStack::PushL( self);
	self->ConstructL();
	return self;
}

// ----------------------------------------------------
// CTimeOutTimer::RunL()
// Handles an active object’s request completion event.
// ----------------------------------------------------
//    
void CTimeOutTimer::RunL()
{
	if ( iStatus == KErrNone)
	{
		iNotify.TimerExpiredL();
	}
	else
	{
		User::Leave(iStatus.Int());
	}
}

TInt CTimeOutTimer::RunError( TInt /*aError*/)
{
	return KErrNone;
}

// End of file
