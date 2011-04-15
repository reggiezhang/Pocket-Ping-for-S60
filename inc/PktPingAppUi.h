/*$Id: PktPingAppUi.h 338 2008-12-22 14:29:31Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PKTPINGAPPUI_H
#define PKTPINGAPPUI_H

#include <aknviewappui.h>
#include <aknnavi.h> 
#include <akntabgrp.h> 
#include <akntabobserver.h> 
#include <aknwaitdialog.h> 
#include "ApConnectStateObserver.h"
#include "HostResolver.h"
#include "PingControllerObserver.h"
class CPktPingView;
class CPktPingSettingView;
class CApConnectEngine;
class CPingController;
class TPingSettings;
class CTextResolver;
/**
 * @class    CPktPingAppUi PktPingAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CPktPingAppUi : public CAknViewAppUi, MApConnectStateObserver
	, MProgressDialogCallback, CHostResolver::MClient, MPingControllerObserver
    {
public: 
    // constructor and destructor
    CPktPingAppUi();
    virtual ~CPktPingAppUi();
    void ConstructL();
    void PostConstructL();
    void PingL();

public:
    // from CCoeAppUi
    TKeyResponse HandleKeyEventL(
                const TKeyEvent& aKeyEvent,
                TEventCode aType );

    // from CEikAppUi
    void HandleCommandL( TInt aCommand );
    void HandleResourceChangeL( TInt aType );

    // from CAknAppUi
    void HandleViewDeactivation( 
            const TVwsViewId& aViewIdToBeDeactivated, 
            const TVwsViewId& aNewlyActivatedViewId );
    
    // from MProgressDialogCallback
    void DialogDismissedL( TInt aButtonId );
    
    // from CHostResolver::MClient
    void HostResolvedL(TInt aError, TNameEntry& aResult);
    
    void ChangeAccessPointL();
    void SelectAccessPointL();
    
    // from MApConnectStateObserver
    void ChangeStateL(TInt aState);
    
    // from MPingControllerObserver
	void NotifyPingProgressL(const TDesC& aStatus);
	void NotifyErrorL(TInt aError);
	void NotifiyEndL();
	/**
	 * StaticConstructCallBack
	 * Static callback for creating the application
	 * @param a pointer to the this class.
	 */
	static TInt StaticConstructCallBack(TAny* aVoid);
	static TInt StaticStartPingCallBack(TAny* aVoid);
protected:
    /**
    * From AknAppUi. Handles pointer-initiated view switch. Currently this
    * function does nothing but calls base class function.
    * @since Series 60 3.0
    * @param aEvent Window server event.
    * @param aDestination Pointer to the control which the event is targeted to.
    */
    void HandleWsEventL( const TWsEvent& aEvent, CCoeControl* aDestination );

private:
    void InitializeContainersL();
    void ShowAboutNoteL();
    void ShowInfoNoteL(TInt aResId);
    void DisplayWaitingDialogL();
    void DisposeWaitingDialogL();
    void UpdateWaitingDialogL(const TDesC& aTxt);
    TBool InquireTargetHostL(TDes& aAddr);
public: 

private: 
    CPktPingView* iPktPingView;
    CPktPingSettingView* iPktPingSettingView;
    /**
     * iNaviPane
	 * Does not own.
	 */
	CAknNavigationControlContainer* iNaviPane;
	/**
	* iDecoratedTabGroup
	*/
	CAknNavigationDecorator* iDecorator;
	/**
	* iTabGroup
	* Does not own.
	*/
	CAknTabGroup* iTabGroup;
	
	CAsyncCallBack* iAsynch;
	TUint32 iAp;
	CApConnectEngine* iApConnectEngine;
	CAknWaitDialog* iWaitDialog;
	CHostResolver* iHostResolver;
	CPingController* iPingController;
	TPingSettings* iPingSettings;
	TBuf<128> iTargetHost;
	TBuf<128> iTargetIp;
	TInt iConnectState;
	CTextResolver* iTextResolver;
	TBool iWaitingForPing;
    };

#endif // PKTPINGAPPUI_H            
