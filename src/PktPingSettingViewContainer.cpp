/*$Id: PktPingSettingViewContainer.cpp 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include <aknsettingitemlist.h>
#include "PktPing.hrh"
#include "PktPingSettingViewContainer.h"
#include "IntegerSettingItem.h"
#include "LabelSettingItem.h"

_LIT(KDefaultAddress, "Disconnected");
CPktPingSettingViewContainer::CPktPingSettingViewContainer(TPingSettings& aSettings)
: iSettings(aSettings)
{
	// No implementation required
}

CPktPingSettingViewContainer::~CPktPingSettingViewContainer()
{
}

CPktPingSettingViewContainer* CPktPingSettingViewContainer::NewLC(TPingSettings& aSettings)
{
	CPktPingSettingViewContainer* self = new (ELeave)CPktPingSettingViewContainer(aSettings);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CPktPingSettingViewContainer* CPktPingSettingViewContainer::NewL(TPingSettings& aSettings)
{
	CPktPingSettingViewContainer* self=CPktPingSettingViewContainer::NewLC(aSettings);
	CleanupStack::Pop(); // self;
	return self;
}

void CPktPingSettingViewContainer::ConstructL()
{

}
CAknSettingItem* CPktPingSettingViewContainer::CreateSettingItemL( TInt aSettingId )
{
	// method is used to create specific setting item as required at run-time.
	// aIdentifier is used to determine what kind of setting item should be 
	// created
	CAknSettingItem* settingItem = NULL;
	switch ( aSettingId )
	{
	case EPktPingMyAddress:
		settingItem = new ( ELeave ) CLabelSettingItem(aSettingId, iSettings.MyAddress());
		break;
	case EPktPingNrOfPing:
		settingItem = new ( ELeave ) CIntegerSettingItem(aSettingId, iSettings.NrOfPing());
		break;
	case EPktPingDataSize:
		settingItem = new ( ELeave ) CIntegerSettingItem(aSettingId, iSettings.DataSize());
		((CIntegerSettingItem*)settingItem)->SetMaximumIntegerValue(1472);
		break;
	case EPktPingFragment:
		settingItem = new ( ELeave ) CAknBinaryPopupSettingItem(aSettingId, iSettings.DontFragment());
		break;
	case EPktPingTtl:
		settingItem = new ( ELeave ) CIntegerSettingItem(aSettingId, iSettings.Ttl());
		break;
	default:
		break;
	}
	return settingItem;
}
