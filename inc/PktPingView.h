/*$Id: PktPingView.h 340 2008-12-23 16:11:32Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PKTPINGCONTAINERVIEW_H
#define PKTPINGCONTAINERVIEW_H

#include <aknview.h>
#include "ApConnectStateObserver.h"
class CPktPingViewContainer;

/**
 * Avkon view class for PktPingContainerView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class    CPktPingView PktPingContainerView.h
 */                        
            
class CPktPingView : public CAknView, public MApConnectStateObserver
    {
public:
    // constructors and destructor
    CPktPingView();
    static CPktPingView* NewL();
    static CPktPingView* NewLC();        
    void ConstructL();
    virtual ~CPktPingView();
                        
    // from base class CAknView
    TUid Id() const;
    void HandleCommandL( TInt aCommand );
    CPktPingViewContainer* CreateContainerL();
    
    void ClearTextL();
	void AppendTextL(const TDesC& aText);
	
	// from MApConnectStateObserver
	void ChangeStateL(TInt aState);
	void SetTitleL(const TDesC& aTitle);
	//void SetCbaL(TInt aResId);

protected:
    // from base class CAknView
    void DoActivateL(
        const TVwsViewId& aPrevViewId,
        TUid aCustomMessageId,
        const TDesC8& aCustomMessage );
    void DoDeactivate();

private:
    void SetupStatusPaneL();
    void CleanupStatusPane();

    CPktPingViewContainer* iPktPingViewContainer;
    TUid iPreviousView;
    };

#endif // PKTPINGCONTAINERVIEW_H
