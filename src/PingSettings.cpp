/*$Id: PingSettings.cpp 312 2008-12-15 06:05:46Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include <stringloader.h>
#include "PktPing.rsg"
#include "PingSettings.h"


TPingSettings::TPingSettings() : iNrOfPing(4), iDataSize(32), iDontFragment(EFalse), iTtl(32)
{
	// No implementation required
}

TPingSettings::~TPingSettings()
{
}

TPingSettings* TPingSettings::NewL()
{
	TPingSettings* self = new (ELeave)TPingSettings();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self;
	return self;
}

void TPingSettings::ConstructL()
{
	HBufC* text = StringLoader::LoadLC( R_PKTPING_DISCONNECTED );
	SetMyAddress( text->Des() );
    CleanupStack::PopAndDestroy( text );
}
void TPingSettings::ResetMyAddressL()
{
//	need trap later
	HBufC* text = StringLoader::LoadLC( R_PKTPING_DISCONNECTED );
	SetMyAddress( text->Des() );
    CleanupStack::PopAndDestroy( text );	 
}
TDes& TPingSettings::MyAddress()
{
	return iMyAddress;
}

void TPingSettings::SetMyAddress(const TDesC& aValue)
{
	if (aValue.Length() < KMyAddressMaxLength)
		iMyAddress.Copy(aValue);
	else
		iMyAddress.Copy(aValue.Ptr(), KMyAddressMaxLength);
}

TInt& TPingSettings::NrOfPing()
{
	return iNrOfPing;
}

TInt TPingSettings::NrOfPing() const
{
	return iNrOfPing;
}

//void TPingSettings::SetNrOfPing(const TInt& aValue)
//{
//	iNrOfPing = aValue;
//}

TInt& TPingSettings::DataSize()
{
	return iDataSize;
}
TInt TPingSettings::DataSize() const
{
	return iDataSize;
}

//void TPingSettings::SetDataSize(const TInt& aValue)
//{
//	iDataSize = aValue;
//}

TBool& TPingSettings::DontFragment()
{
	return iDontFragment;
}
TBool TPingSettings::DontFragment() const
{
	return iDontFragment;
}

//void TPingSettings::SetDontFragment(const TBool& aValue)
//{
//	iDontFragment = aValue;
//}

TInt& TPingSettings::Ttl()
{
	return iTtl;
}

TInt TPingSettings::Ttl() const
{
	return iTtl;
}

//void TPingSettings::SetTtl(const TInt& aValue)
//{
//	iTtl = aValue;
//}
