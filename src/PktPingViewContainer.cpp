/*$Id: PktPingViewContainer.cpp 346 2009-07-29 08:04:23Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */
#include <aknviewappui.h>
#include <eikappui.h>
#include <eikgted.h>
#include <txtrich.h>	//Richtext
#include <eikrted.h>  	//RichText Editor
#include <txtglobl.h>
#include <aknsbasicbackgroundcontrolcontext.h> 
#include <aknsdrawutils.h> 
#include <avkon.hrh>
#include <PktPing.rsg>
#include "PktPingViewContainer.h"
#include "PktPingView.h"
#include "PktPing.hrh"
_LIT(KEndOfLine, "\f" );
// CEikEdwin uses '\f' as EOF mark.
const TInt KEdwinWidth = 7;
const TInt KEdwinTextLimit = 50000;//0;  // Number of editable chars
const TInt KEdwinNumberOfLines = 11;
const TInt KFontInUse(EAknLogicalFontSecondaryFont);
const TInt KFontColorIndexBlue(200);
const TInt KMargin(0);
/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CPktPingViewContainer::CPktPingViewContainer()
{

}
/** 
 * Destroy child controls.
 */
CPktPingViewContainer::~CPktPingViewContainer()
{
	delete iBgContext;
	delete iPingHistory;
}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CPktPingViewContainer
 */
CPktPingViewContainer* CPktPingViewContainer::NewL(const TRect& aRect,
		const CCoeControl* aParent, MEikCommandObserver* aCommandObserver)
{
	CPktPingViewContainer* self = CPktPingViewContainer::NewLC(aRect, aParent,
			aCommandObserver);
	CleanupStack::Pop(self);
	return self;
}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CPktPingViewContainer
 */
CPktPingViewContainer* CPktPingViewContainer::NewLC(const TRect& aRect,
		const CCoeControl* aParent, MEikCommandObserver* aCommandObserver)
{
	CPktPingViewContainer* self = new (ELeave) CPktPingViewContainer();
	CleanupStack::PushL(self);
	self->ConstructL(aRect, aParent, aCommandObserver);
	return self;
}

/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */
void CPktPingViewContainer::ConstructL(const TRect& aRect,
		const CCoeControl* aParent, MEikCommandObserver* aCommandObserver)
{
	if (aParent == NULL)
	{
		CreateWindowL();
	}
	else
	{
		SetContainerWindowL(*aParent);
	}
	iFocusControl = NULL;
	iCommandObserver = aCommandObserver;
	//iRect = aRect;
	InitializeControlsL(aRect);
	SetRect(aRect);
	ActivateL();
}

/**
 * Return the number of controls in the container (override)
 * @return count
 */
TInt CPktPingViewContainer::CountComponentControls() const
{
	return (int) ELastControl;
}

/**
 * Get the control with the given index (override)
 * @param aIndex Control index [0...n) (limited by #CountComponentControls)
 * @return Pointer to control
 */
CCoeControl* CPktPingViewContainer::ComponentControl(TInt aIndex) const
{
	switch (aIndex)
	{
	case EEikEdwin:
		return iPingHistory;
	}
	return NULL;
}

/**
 *    Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */
void CPktPingViewContainer::SizeChanged()
{
	if (iBgContext)
	{
		iBgContext->SetParentPos(TPoint(0, 0));
		iBgContext->SetRect(Rect());
		/*if ( &Window() )
		 {
		 iBgContext->SetParentPos(PositionRelativeToScreen() );
		 }*/
	}
	CCoeControl::SizeChanged();
	LayoutControls();
}

/**
 * Layout components as specified in the UI Designer
 */
void CPktPingViewContainer::LayoutControls()
{
	if (iPingHistory)
	{
		TSize size = Rect().Size();
		size.iWidth = size.iWidth - CEikScrollBar::DefaultScrollBarBreadth();
		iPingHistory->SetExtent(TPoint(0, 0), size);
	}
	DrawNow();
}

/**
 *    Handle key events.
 */
TKeyResponse CPktPingViewContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,
		TEventCode aType)
{
	if (iPingHistory)
	{
		switch (aKeyEvent.iCode)
		{
		case EKeyDownArrow:
		{
			iPingHistory->MoveDisplayL(TCursorPosition::EFLineDown);
			return EKeyWasConsumed;
		}
		case EKeyUpArrow:
		{
			iPingHistory->MoveDisplayL(TCursorPosition::EFLineUp);
			return EKeyWasConsumed;
		}
		case EKeyLeftArrow:
		{
			iPingHistory->MoveDisplayL(TCursorPosition::EFPageUp);
			return EKeyWasConsumed;
		}
		case EKeyRightArrow:
		{
			iPingHistory->MoveDisplayL(TCursorPosition::EFPageDown);
			return EKeyWasConsumed;
		}
		default:
			return iPingHistory->OfferKeyEventL(aKeyEvent, aType);
		}
	}
	return CCoeControl::OfferKeyEventL(aKeyEvent, aType);
}

/**
 *    Initialize each control upon creation.
 */
void CPktPingViewContainer::InitializeControlsL(const TRect& aRect)
{
	iBgContext = CAknsBasicBackgroundControlContext::NewL(
			KAknsIIDQsnBgAreaMain/*KAknsIIDQsnBgAreaMainAppsGrid*/, aRect,
			EFalse);
	// Create editable editor control
	iPingHistory = new (ELeave) CEikRichTextEditor();
	TInt pingHistoryFlags(EEikEdwinNoAutoSelection| EEikEdwinJustAutoCurEnd
			| EEikEdwinLineCursor | EEikEdwinNoHorizScrolling);
	((CEikGlobalTextEditor*)iPingHistory)->ConstructL(this, NumOfFittingLines(aRect), KEdwinTextLimit,
			pingHistoryFlags, EGulFontControlAll, EGulAllFonts);

	iPingHistory->SetContainerWindowL(*this);
	iPingHistory->SetMopParent(this);
	iPingHistory->SetAknEditorFlags(EAknEditorFlagEnableScrollBars);
	// Creates scroll bar.
	iPingHistory->CreateScrollBarFrameL();
	iPingHistory->ScrollBarFrame()->SetScrollBarVisibilityL(
			CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto);
	iPingHistory->SetReadOnly(ETrue);
	
	TRect rect = aRect;
	iPingHistory->SetRect(rect);
	
	CGlobalText* globalText = iPingHistory->GlobalText();
	globalText->Reset();

	TRgb color(0, 0, 0);
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	AknsUtils::GetCachedColor(skin, color, KAknsIIDQsnTextColors,
			EAknsCIQsnTextColorsCG6);

	/* set properties of editor and history control */
	CCharFormatLayer* formatLayer = CEikonEnv::NewDefaultCharFormatLayerL();

	TCharFormat charFormat;
	TCharFormatMask charFormatMask;
	formatLayer->Sense(charFormat, charFormatMask);
	charFormat.iFontPresentation.iTextColor = color;
	charFormatMask.SetAttrib(EAttColor);
	formatLayer->SetL(charFormat, charFormatMask);

	iPingHistory->SetCharFormatLayer(formatLayer);
	
	iPingHistory->TextLayout()->RestrictScrollToTopsOfLines( EFalse );
	iPingHistory->ActivateL();

}

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CPktPingViewContainer::HandleResourceChange(TInt aType)
{
	CCoeControl::HandleResourceChange(aType);
	SetRect(
			iAvkonViewAppUi->View(TUid::Uid(EPktPingContainerViewId))->ClientRect());
}

/**
 *    Draw container contents.
 */
void CPktPingViewContainer::Draw(const TRect& aRect) const
{
	TRect tmp = aRect;
	CWindowGc& gc = SystemGc();
	// Redraw the background using the default skin
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
	AknsDrawUtils::Background(skin, cc, this, gc, aRect);
	gc.DrawRect(aRect);
}
// -----------------------------------------------------------------------------
// Sets the data shown in editor and sets the focus on editor.
// -----------------------------------------------------------------------------
void CPktPingViewContainer::AppendTextL(const TDesC& aText,
		TBool aInsertNewline)
{
	if (!iPingHistory)
		return;
	HBufC* currText = iPingHistory->GetTextInHBufL();
	CleanupStack::PushL(currText);
	TPtrC keepText(0, 0);
	TBuf<1> startingNewline;

	if (currText)
	{
		keepText.Set(*currText);
		if (aInsertNewline)
		{
			startingNewline.SetLength(1);
			startingNewline[0] = CEditableText::EParagraphDelimiter;
		}
	}

	HBufC* newText = HBufC::NewLC(keepText.Length() + aText.Length()
			+ startingNewline.Length());

	newText->Des().Append(keepText);
	newText->Des().Append(startingNewline);
	newText->Des().Append(aText);
	TPtrC newTextPtr = newText->Right(KEdwinTextLimit);

	iPingHistory->SetTextL(&newTextPtr);
	iPingHistory->DrawDeferred();
	iPingHistory->ScrollBarFrame()->DrawScrollBarsNow();

	CleanupStack::PopAndDestroy(newText);
	CleanupStack::PopAndDestroy(currText);
}

void CPktPingViewContainer::ClearTextL()
{
	if (iPingHistory)
	{
		iPingHistory->SetCursorPosL(0, EFalse);
		CGlobalText* histText = iPingHistory->GlobalText();
		histText->Reset();
		iPingHistory->HandleTextChangedL();
		iPingHistory->DrawDeferred();
		iPingHistory->ScrollBarFrame()->DrawScrollBarsNow();
	}
}
TTypeUid::Ptr CPktPingViewContainer::MopSupplyObject(TTypeUid aId)
{
	if (aId.iUid == MAknsControlContext::ETypeId)
	{
		return MAknsControlContext::SupplyMopObject(aId, iBgContext);
	}
	return CCoeControl::MopSupplyObject(aId);
}

TInt CPktPingViewContainer::NumOfFittingLines(const TRect& aRect) const
{
	TInt lines = (aRect.Height()) / FontLineGap();
	return 11;
}
TInt CPktPingViewContainer::FontLineGap() const
{
	const CFont* font = AknLayoutUtils::FontFromId(KFontInUse);
	_LIT( KPanicCntx, "CPktPingViewContainer::FontHeightInPixels(): Font not found" );
	__ASSERT_ALWAYS( font, User::Panic( KPanicCntx(), KErrNotFound ) );

	return font->FontLineGap();
}
