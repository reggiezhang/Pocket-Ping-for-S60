/*$Id: IntegerSettingItem.cpp 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include <akntextsettingpage.h> 
#include "IntegerSettingItem.h"

CIntegerSettingItem::CIntegerSettingItem(TInt aIdentifier, TInt& aValue)
:CAknIntegerEdwinSettingItem(aIdentifier, aValue)
{
	// No implementation required
}

CIntegerSettingItem::~CIntegerSettingItem()
{
}
CAknIntegerEdwin* CIntegerSettingItem::IntegerEditorControl()
{
	if ( CAknIntegerSettingPage* page = (CAknIntegerSettingPage*)SettingPage() )
	{
		return page->IntegerEditorControl();
	}
	return NULL;
}
void CIntegerSettingItem::SetMaximumIntegerValue(TInt aMaximumValue)
{
	if ( CAknIntegerEdwin* edwin = IntegerEditorControl() )
	{
		edwin->SetMaximumIntegerValue(aMaximumValue);
	}
}
void CIntegerSettingItem::SetMinimumIntegerValue(TInt aMinimumValue)
{
	if ( CAknIntegerEdwin* edwin = IntegerEditorControl() )
	{
		edwin->SetMinimumIntegerValue(aMinimumValue);
	}
}


