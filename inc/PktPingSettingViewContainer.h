/*$Id: PktPingSettingViewContainer.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PKTPINGSETTINGVIEWCONTAINER_H
#define PKTPINGSETTINGVIEWCONTAINER_H 

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <aknsettingitemlist.h>
#include "PingSettings.h"

// CLASS DECLARATION

/**
 *  CPktPingSettingViewContainer
 * 
 */
class CPktPingSettingViewContainer : public CAknSettingItemList
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CPktPingSettingViewContainer();

	/**
	 * Two-phased constructor.
	 */
	static CPktPingSettingViewContainer* NewL(TPingSettings& aSettings);

	/**
	 * Two-phased constructor.
	 */
	static CPktPingSettingViewContainer* NewLC(TPingSettings& aSettings);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CPktPingSettingViewContainer(TPingSettings& aSettings);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	/**
	 * Framework method to create a setting item based upon the user id aSettingId. The 
	 * client code decides what type to contruct.  new (ELeave) must then be used and the resulting 
	 * pointer returned. Ownership is thereafter base class's responsiblity.
	 *
	 * @param aSettingId	ID to use to determine the type of the setting item
	 * @return a constructed (not 2nd-stage constructed) setting item.
	 */
	virtual CAknSettingItem* CreateSettingItemL( TInt aSettingId );
	
private:
	TPingSettings& iSettings;

};

#endif // PKTPINGSETTINGVIEWCONTAINER_H
