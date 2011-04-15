/*$Id: PktPingView.cpp 340 2008-12-23 16:11:32Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <akncontext.h>
#include <akntitle.h>
#include <stringloader.h>
#include <barsread.h>
#include <eikbtgpc.h>
#include <avkon.rsg>
#include <PktPing.rsg>
#include "PktPing.hrh"
#include "PktPingView.h"
#include "PktPing.hrh"
#include "PktPingViewContainer.h"
#include "util.h"

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CPktPingView::CPktPingView()
{
	iPktPingViewContainer = NULL;
}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CPktPingView::~CPktPingView()
{
	delete iPktPingViewContainer;
	iPktPingViewContainer = NULL;
}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CPktPingView
 */
CPktPingView* CPktPingView::NewL()
{
	CPktPingView* self = CPktPingView::NewLC();
	CleanupStack::Pop(self);
	return self;
}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CPktPingView
 */
CPktPingView* CPktPingView::NewLC()
{
	CPktPingView* self = new (ELeave) CPktPingView();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */
void CPktPingView::ConstructL()
{
	BaseConstructL(R_PKT_PING_CONTAINER_PKT_PING_CONTAINER_VIEW);
}

/**
 * @return The UID for this view
 */
TUid CPktPingView::Id() const
{
	return TUid::Uid(EPktPingContainerViewId);
}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CPktPingView::HandleCommandL(TInt aCommand)
{
	switch (aCommand)
	{ // code to dispatch to the AknView's menu and CBA commands is generated here
	case EAknSoftkeyClose:
		AppUi()->ActivateLocalViewL(iPreviousView);
		//AppUi()->HandleCommandL(EPktPingCmdCancelPing);
		break;
	case EPktPingCmdCancelPing:
		// change cba
		Util::SetCbaL(R_AVKON_SOFTKEYS_CLOSE);
		AppUi()->HandleCommandL(aCommand);
		break;
	case EPktPingCmdShowStat:
		Util::SetCbaL(R_PKT_PING_RESUME_CBA);
		AppUi()->HandleCommandL(aCommand);
		break;
	case EPktPingCmdResume:
		Util::SetCbaL(R_PKT_PING_ONGOING_CBA);
		AppUi()->HandleCommandL(aCommand);
		break;
	case EPktPingcmdEnd:
		Util::SetCbaL(R_AVKON_SOFTKEYS_CLOSE);
		break;
	default:
		AppUi()->HandleCommandL(aCommand);
		break;
	}
}

/**
 *    Handles user actions during activation of the view, 
 *    such as initializing the content.
 */
void CPktPingView::DoActivateL(const TVwsViewId& aPrevViewId,
		TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/)
{
	iPreviousView = aPrevViewId.iViewUid;
	// Fetches pointer to the default title pane control.
	CAknTitlePane* titlePane =
			static_cast<CAknTitlePane*> (StatusPane()->ControlL(TUid::Uid(
					EEikStatusPaneUidTitle)));
	HBufC* title = StringLoader::LoadLC(R_PKTPING_TITLE);
	titlePane->SetTextL(*title);
	SetupStatusPaneL();
	if (iPktPingViewContainer == NULL)
	{
		iPktPingViewContainer = CreateContainerL();
		iPktPingViewContainer->SetMopParent(this);
		iPktPingViewContainer->ClearTextL();
		AppUi()->AddToStackL(*this, iPktPingViewContainer);
	}
	Util::SetCbaL(R_AVKON_SOFTKEYS_EMPTY/*R_AVKON_SOFTKEYS_CLOSE*//*R_PKT_PING_ONGOING_CBA*/);
	CleanupStack::PopAndDestroy(title);
}
void CPktPingView::SetTitleL(const TDesC& aTitle)
{
	CAknTitlePane* titlePane =
			static_cast<CAknTitlePane*> (StatusPane()->ControlL(TUid::Uid(
					EEikStatusPaneUidTitle)));
	titlePane->SetTextL(aTitle);
}

/**
 */
void CPktPingView::DoDeactivate()
{
	CleanupStatusPane();
	if (iPktPingViewContainer != NULL)
	{
		AppUi()->RemoveFromViewStack(*this, iPktPingViewContainer);
		delete iPktPingViewContainer;
		iPktPingViewContainer = NULL;
	}
}

void CPktPingView::SetupStatusPaneL()
{
	StatusPane()->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL_FLAT);
}

void CPktPingView::CleanupStatusPane()
{
	StatusPane()->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL);
}

/**
 *    Creates the top-level container for the view.  You may modify this method's
 *    contents and the CPktPingViewContainer::NewL() signature as needed to initialize the
 *    container, but the signature for this method is fixed.
 *    @return new initialized instance of CPktPingViewContainer
 */
CPktPingViewContainer* CPktPingView::CreateContainerL()
{
	return CPktPingViewContainer::NewL(ClientRect(), NULL, this);
}

void CPktPingView::AppendTextL(const TDesC& aText)
{
	if (iPktPingViewContainer)
	{
		iPktPingViewContainer->AppendTextL(aText);
	}
}

void CPktPingView::ClearTextL()
{
	if (iPktPingViewContainer)
	{
		iPktPingViewContainer->ClearTextL();
	}
}
void CPktPingView::ChangeStateL(TInt aState)
{
	if ((aState == EApConnectStateNotConnected) && iPktPingViewContainer)
	{
		HBufC* errorText = StringLoader::LoadLC(R_PKTPING_CONNECTION_LOST);
		AppendTextL(*errorText);
		CleanupStack::PopAndDestroy(errorText);
		HandleCommandL(EPktPingCmdCancelPing);
	}
}
//void CPktPingView::SetCbaL(TInt aResId)
//{
//	CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
//	cba->SetCommandSetL(aResId);
//	cba->DrawNow();
//}
