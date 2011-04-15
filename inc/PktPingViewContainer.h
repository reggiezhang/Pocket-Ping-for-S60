/*$Id: PktPingViewContainer.h 327 2008-12-20 08:51:22Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PKTPINGCONTAINER_H
#define PKTPINGCONTAINER_H

#include <coecntrl.h>        
#include <eikedwin.h>
#include <aknsutils.h>
#include <aknutils.h>
class MEikCommandObserver;        
class CEikRichTextEditor;
class CAknsBasicBackgroundControlContext;
/**
 * Container class for PktPingContainer
 * 
 * @class    CPktPingViewContainer PktPingContainer.h
 */
class CPktPingViewContainer : public CCoeControl
    {
public:
    // constructors and destructor
    CPktPingViewContainer();
    static CPktPingViewContainer* NewL( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver );
    static CPktPingViewContainer* NewLC( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver );
    void ConstructL( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver );
    virtual ~CPktPingViewContainer();

public:
    // from base class CCoeControl
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl( TInt aIndex ) const;
    TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent, 
            TEventCode aType );
    void HandleResourceChange( TInt aType );
	TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
    
    void ClearTextL();
	void AppendTextL(const TDesC& aText, TBool aInsertNewline = ETrue);
    
protected:
    // from base class CCoeControl
    void SizeChanged();

private:
    // from base class CCoeControl
    void Draw( const TRect& aRect ) const;

private:
    void InitializeControlsL(const TRect& aRect);
    void LayoutControls();
    TInt FontLineGap() const;
    TInt NumOfFittingLines( const TRect& aRect ) const;
private:
    CCoeControl* iFocusControl;
    CEikRichTextEditor* iPingHistory;
    MEikCommandObserver* iCommandObserver;
    CAknsBasicBackgroundControlContext* iBgContext;
    //TRect iRect;
    
public: 
    enum TControls
        {
    	EEikEdwin,
        ELastControl
        };
    };
                
#endif // PKTPINGCONTAINER_H
