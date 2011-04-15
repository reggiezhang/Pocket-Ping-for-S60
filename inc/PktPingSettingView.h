/*$Id: PktPingSettingView.h 312 2008-12-15 06:05:46Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PKTPINGSETTINGVIEW_H
#define PKTPINGSETTINGVIEW_H

// INCLUDES
#include <e32std.h>
#include <e32base.h> 
#include <aknview.h>
#include <aknnavi.h> 
#include "ApConnectEngine.h"
// CLASS DECLARATION
class CPktPingSettingViewContainer;
class TPingSettings;
/**
 *  CPktPingSettingView
 * 
 */
class CPktPingSettingView : public CAknView
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CPktPingSettingView();

	/**
	 * Two-phased constructor.
	 */
	static CPktPingSettingView* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CPktPingSettingView* NewLC();
	
	/** 
     * This is a second stage constructor initializing this view with standard 
     * values. Should be called from derived classes second stage construction
     * functions.  
     * @param aResId The numeric ID of the resource (@c iViewInfo) to be read. 
     *        If aResId=0, resource file is not accessed.
     */
	void BaseConstructL(TInt aResId=0);
	
	/**
    * Returns views id, intended for overriding by sub classes.
    * @return id for this view.
    */
	virtual TUid Id() const;
	
	/** 
     * Command handling function intended for overriding by sub classes. 
     * Default implementation is empty.  
     * @param aCommand ID of the command to respond to. 
     */
	virtual void HandleCommandL(TInt aCommand);
	
	void SetTabDecorator(CAknNavigationDecorator* aDecorator);
	void UpdateConnectionInfoL(TApConnectionInfo& aInfo);
	TPingSettings* GetPingSettings();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CPktPingSettingView();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	/** 
     * Handles a view activation and passes the message of type 
     * @c aCustomMessageId. This function is intended for overriding by 
     * sub classes. This function is called by @c AknViewActivatedL().
     * @param aPrevViewId Specifies the view previously active.
     * @param aCustomMessageId Specifies the message type.
     * @param aCustomMessage The activation message.
     */
	virtual void DoActivateL(const TVwsViewId& aPrevViewId,
	                                  TUid aCustomMessageId,
	                                  const TDesC8& aCustomMessage);

    /** 
     * View deactivation function intended for overriding by sub classes. 
     * This function is called by @c AknViewDeactivated().
     */
	virtual void DoDeactivate();
	
	/**
     * DisplaySettingsL
	 * creates the settinglist
	 */	
	void DisplaySettingsL();

	/**
	* DisplayInitOutlineL
	* Displays initial outline when view changed.
	*/
	void DisplayInitOutlineL();

	void SetupStatusPaneL();
	
	CPktPingSettingViewContainer* iSettingListBox;
	TPingSettings* iSettings;
	TUid iPreviousView;
	/** Navigation pane. Does not own. */
	CAknNavigationControlContainer* iNaviPane;
	CAknNavigationDecorator* iDecorator;

};

#endif // PKTPINGSETTINGVIEW_H
