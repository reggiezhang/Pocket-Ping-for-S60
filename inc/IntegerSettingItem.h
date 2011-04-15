/*$Id: IntegerSettingItem.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef INTEGERSETTINGITEM_H
#define INTEGERSETTINGITEM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <aknsettingitemlist.h>
// CLASS DECLARATION
class CAknIntegerEdwin;
/**
 *  CIntegerSettingItem
 * 
 */
class CIntegerSettingItem : public CAknIntegerEdwinSettingItem
{
public:
	// Constructors and destructor
	/**
	 * Constructor for performing 1st stage construction
	 */
	CIntegerSettingItem(TInt aIdentifier, TInt& aValue);
	/**
	 * Destructor.
	 */
	~CIntegerSettingItem();
	
	void SetMaximumIntegerValue(TInt aMaximumValue);
	void SetMinimumIntegerValue(TInt aMinimumValue);   

private:
	CAknIntegerEdwin* IntegerEditorControl();

};

#endif // INTEGERSETTINGITEM_H
