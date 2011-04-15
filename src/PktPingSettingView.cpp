/*$Id: PktPingSettingView.cpp 340 2008-12-23 16:11:32Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include <aknViewAppUi.h> 
#include <akntitle.h>
#include <akncontext.h>
#include <barsread.h>
#include <PktPing.rsg>
#include "PktPing.hrh"
#include "PktPingSettingView.h"
#include "PktPingSettingViewContainer.h"
#include "PingSettings.h"
#include "util.h"
CPktPingSettingView::CPktPingSettingView()
{
	// No implementation required
}

CPktPingSettingView::~CPktPingSettingView()
{
	delete iSettingListBox;
	delete iSettings;
}

CPktPingSettingView* CPktPingSettingView::NewLC()
{
	CPktPingSettingView* self = new (ELeave) CPktPingSettingView();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CPktPingSettingView* CPktPingSettingView::NewL()
{
	CPktPingSettingView* self = CPktPingSettingView::NewLC();
	CleanupStack::Pop(); // self;
	return self;
}

void CPktPingSettingView::ConstructL()
{
	iSettings = TPingSettings::NewL();
	BaseConstructL(R_PKT_PING_SETTING_VIEW);
}

void CPktPingSettingView::BaseConstructL(TInt aResId)
{
	CEikStatusPane* sp = StatusPane();
	// Fetch pointer to the default navi pane control
	iNaviPane = (CAknNavigationControlContainer*) sp->ControlL(TUid::Uid(
			EEikStatusPaneUidNavi));
	CAknView::BaseConstructL(aResId);
}

TUid CPktPingSettingView::Id() const
{
	return TUid::Uid(EPktPingSettingViewId);
}

void CPktPingSettingView::DoActivateL(const TVwsViewId& aPrevViewId,
		TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/)
{
	iPreviousView = aPrevViewId.iViewUid;
	SetupStatusPaneL();
	DisplaySettingsL();
	Util::SetCbaL(R_AVKON_SOFTKEYS_OPTIONS_EXIT);
}
void CPktPingSettingView::DoDeactivate()
{
	if (iSettingListBox)
	{
		AppUi()->RemoveFromStack(iSettingListBox);
		delete iSettingListBox;
		iSettingListBox = NULL;
	}
}
void CPktPingSettingView::DisplaySettingsL()
{
	if (iSettingListBox == NULL)
	{
		iSettingListBox = CPktPingSettingViewContainer::NewL(*iSettings);
		iSettingListBox->SetMopParent(this);
		iSettingListBox->ConstructFromResourceL(R_PKT_PING_SETTING_LIST);
		AppUi()->AddToStackL(*this, iSettingListBox);
		iSettingListBox->ActivateL();
	}

}
void CPktPingSettingView::HandleCommandL(TInt aCommand)
{
	AppUi()->HandleCommandL(aCommand);
}
void CPktPingSettingView::DisplayInitOutlineL()
{
	if (iDecorator)
	{
		iNaviPane->PushL(*iDecorator);
	}
}
void CPktPingSettingView::SetTabDecorator(CAknNavigationDecorator* aDecorator)
{
	iDecorator = aDecorator;
}
void CPktPingSettingView::UpdateConnectionInfoL(TApConnectionInfo& aInfo)
{
	if (iSettings)
	{
		if (iSettingListBox)
		{
			TRAP_IGNORE(iSettingListBox->StoreSettingsL());
		}
		iSettings->SetMyAddress(aInfo.ipAddress);
		if (!aInfo.ipAddress.Length())
		{
			iSettings->ResetMyAddressL();
		}
		if (iSettingListBox)
		{
			iSettingListBox->LoadSettingsL();
			iSettingListBox->DrawNow();
		}
	}
}
TPingSettings* CPktPingSettingView::GetPingSettings()
{
	TRAP_IGNORE(iSettingListBox->StoreSettingsL());
	return iSettings;
}
void CPktPingSettingView::SetupStatusPaneL()
{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid(EEikStatusPaneUidContext);
	CEikStatusPaneBase::TPaneCapabilities subPaneContext =
			StatusPane()->PaneCapabilities(contextPaneUid);
	if (subPaneContext.IsPresent() && subPaneContext.IsAppOwned())
	{
		CAknContextPane* context =
				static_cast<CAknContextPane*> (StatusPane()->ControlL(
						contextPaneUid));
		context->SetPictureToDefaultL();
	}

	// setup the title pane
	TUid titlePaneUid = TUid::Uid(EEikStatusPaneUidTitle);
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle =
			StatusPane()->PaneCapabilities(titlePaneUid);
	if (subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned())
	{
		CAknTitlePane* title =
				static_cast<CAknTitlePane*> (StatusPane()->ControlL(
						titlePaneUid));
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC(reader,
				R_PKT_PING_CONTAINER_TITLE_RESOURCE );
		title->SetFromResourceL(reader);
		CleanupStack::PopAndDestroy(); // reader internal state
	}
	StatusPane()->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL);
}

