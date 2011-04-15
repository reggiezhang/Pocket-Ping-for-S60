/*$Id: LabelSettingItem.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef LABELSETTINGITEM_H
#define LABELSETTINGITEM_H 

// INCLUDES
#include <AknSettingItemList.h>

// CLASS DECLARATION

/**
 *  CLabelSettingItem
 * 
 */
/** 
 * Dummy settings item to block editing
 */
class CLabelSettingItem:public CAknSettingItem
{
private:
	enum TSettingsType
	{
		EString,
		EInt
	};
public:
	CLabelSettingItem(TInt aIdentifier, TDesC& aString):CAknSettingItem(aIdentifier),iType(EString),iLabelString(aString)
	{
	}

	CLabelSettingItem(TInt aIdentifier, TInt aInt):CAknSettingItem(aIdentifier),iType(EInt),iLabelString(iIntBuf), iLabelInt(aInt)
	{
		iIntBuf.Zero();
		iIntBuf.AppendNum(iLabelInt);
	}

	void ConstructL()
	{
	}

	~CLabelSettingItem()
	{
	}

	/*
	* This method obtains the current value's setting text
	*
	* @return	Reference to descriptor with the text 
	*/
	virtual const TDesC& SettingTextL()
	{
		return iLabelString;
	}

	/**
	 * Override to block editing
	 */
	void EditItemL( TBool /*aCalledFromMenu*/)
	{
	}

private:

	TSettingsType iType;
	TDesC& iLabelString;
	TInt iLabelInt;
	TBuf<16> iIntBuf;
};

#endif // LABELSETTINGITEM_H
