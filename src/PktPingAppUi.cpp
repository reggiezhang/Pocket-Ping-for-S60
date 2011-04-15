/*$Id: PktPingAppUi.cpp 346 2009-07-29 08:04:23Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <aknmessagequerydialog.h> 
#include <sysutil.h>
#include <hal.h>
#include <aknnavide.h> 
#include <apsettingshandlerui.h> 
#include <aknglobalnote.h>
#include <TextResolver.h>
#include <sys\reent.h>
#include <apgtask.h>
#include <PktPing.rsg>
#include "imeireader.h"
#include "PktPingAppUi.h"
#include "PktPing.hrh"
#include "PktPingView.h"
#include "PktPingSettingView.h"
#include "PingController.h"
#include "PingSettings.h"
#include "IcmpPacket.h"
#include "util.h"

_LIT8(KAboutTextFormat, 
"Built on:\n"__DATE__" "__TIME__"\n\
Software version:\n%S\n\
OS version:\n%S\n\
Model id: 0x%x\n\
Machine id:0x%x\n\
IMEI: %S\n\
Copyright (C) Reggie Zhang 2009\nAll rights reserved.\n\
Email:\n\
reggie.zhang@mobiledev.org.cn\n\
Homepage:\n\
www.mobiledev.org.cn");

const TInt KPhoneSwVersionTextLength = 64;
const TInt KMaxHexDigitsInInteger = 10;
const TInt KResolverMaxErrorMsgLength = 256;
#ifdef __WINSCW__
_LIT(KDefaultHost, "wap.sina.com");
#else
_LIT(KDefaultHost, "mobiledev.org.cn");
#endif
/**
 * Construct the CPktPingAppUi instance
 */ 
CPktPingAppUi::CPktPingAppUi()
    {

    }

/** 
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CPktPingAppUi::~CPktPingAppUi()
    {   
	if (iDecorator)
	{
		iNaviPane->Pop(iDecorator);
		delete iDecorator;
	}
	delete iTextResolver;
	delete iAsynch;
	delete iHostResolver;
	delete iPingController;
	delete iApConnectEngine;
		
	DisposeWaitingDialogL();
	CloseSTDLIB(); 
    }

void CPktPingAppUi::InitializeContainersL()
    {
    iPktPingView = CPktPingView::NewL();
    AddViewL( iPktPingView );
    //SetDefaultViewL( *iPktPingView );
    iPktPingSettingView = CPktPingSettingView::NewL();
    AddViewL(iPktPingSettingView);
    SetDefaultViewL( *iPktPingSettingView );
    // Access status pane
//    CEikStatusPane* sp = StatusPane();
//    // Get a pointer to navi pane
//    iNaviPane = (CAknNavigationControlContainer*)sp->ControlL(
//        TUid::Uid(EEikStatusPaneUidNavi));
//    
//    iDecorator = iNaviPane->ResourceDecorator();
//    //iNaviPane->PushL( *iDecorator);
//
//    iTabGroup = static_cast<CAknTabGroup*>(iDecorator->DecoratedControl());
    
    //iPktPingSettingView->SetTabDecorator(iDecorator);
    }

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CPktPingAppUi::HandleCommandL( TInt aCommand )
{
    switch ( aCommand )
    { 
	    case EAknSoftkeyExit:
	    case EEikCmdExit:
	    	Exit();
	    	break;
	    case EPktPingCmdPing:
	    {	
	    	iWaitingForPing = EFalse;
	    	if (iConnectState != EApConnectStateConnected)
	    	{
	    		iWaitingForPing = ETrue;
	    		HandleCommandL(EPktPingCmdChangeAp);
	    		//ShowInfoNoteL(R_PKTPING_DISCONNECTED);
	    		break;
	    	}
	    	if (InquireTargetHostL(iTargetHost))
	    	{
	    		Util::SetCbaL(R_AVKON_SOFTKEYS_EMPTY);
	    		iPingSettings = iPktPingSettingView->GetPingSettings();
		    	ActivateLocalViewL(iPktPingView->Id()); 
	    		TCallBack callback(StaticStartPingCallBack,this);
		        iAsynch->Set(callback);
		        iAsynch->CallBack();
	    	}
	    }
	    	break;
	    case EPktPingCmdCancelPing:
	    	iPingController->CancelPingL();
	    	break;
	    case EPktPingCmdShowStat:
	    	iPingController->ShowIntermediateStat();
	    	break;
	    case EPktPingCmdResume:
	    	iPingController->ResumePingL();
	    	break;
	    case EPktPingCmdChangeAp:
	    {
	    	//ChangeAccessPointL();
	    	DisplayWaitingDialogL();
	    	SelectAccessPointL();
	    	TApConnectionInfo connectInfo = iApConnectEngine->GetApConnectionInfo();
			if (iPktPingSettingView)
			{
				iPktPingSettingView->UpdateConnectionInfoL(connectInfo);
			}
	    }
	    	break;
	    case EPktPingCmdAbout:
	    	ShowAboutNoteL();
	    	break;
        default:
            break;
    }  
}
void CPktPingAppUi::HandleWsEventL( const TWsEvent& aEvent, CCoeControl* aDestination )
{
	switch (aEvent.Type())
	{
	case KAknUidValueEndKeyCloseEvent:
	{
		TApaTask task( iEikonEnv->WsSession() );
		task.SetWgId( CEikonEnv::Static()->RootWin().Identifier() );
		task.SendToBackground();
		break;
	}
	default:
		CAknAppUi::HandleWsEventL(aEvent, aDestination);
	}
}
/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CPktPingAppUi::HandleResourceChangeL( TInt aType )
    {
    CAknViewAppUi::HandleResourceChangeL( aType ); 
    }
                
/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CPktPingAppUi::HandleKeyEventL(
        const TKeyEvent& aKeyEvent,
        TEventCode aType )
    {
    
    return EKeyWasNotConsumed;
    }

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CPktPingAppUi::HandleViewDeactivation( 
        const TVwsViewId& aViewIdToBeDeactivated, 
        const TVwsViewId& aNewlyActivatedViewId )
    {
    CAknViewAppUi::HandleViewDeactivation( 
            aViewIdToBeDeactivated, 
            aNewlyActivatedViewId );
    
    }

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */ 
void CPktPingAppUi::ConstructL()
    {   
    BaseConstructL( EAknEnableSkin ); 
    iAsynch = new (ELeave) CAsyncCallBack(CActive::EPriorityStandard);
    InitializeContainersL();
    iTextResolver = CTextResolver::NewL();
    iApConnectEngine = CApConnectEngine::NewL();
    iApConnectEngine->AddObserverL(this);
    iApConnectEngine->AddObserverL(iPktPingView);
	iHostResolver = CHostResolver::NewL(iApConnectEngine->GetRConnection()
					, iApConnectEngine->GetRSocketServ(), *this);
	
    iPingController = CPingController::NewL(iApConnectEngine->GetRConnection()
    		, iApConnectEngine->GetRSocketServ(), this);
    iTargetHost = KDefaultHost;
    TCallBack callback(StaticConstructCallBack,this);
    iAsynch->Set(callback);
    iAsynch->CallBack();
    //iApConnectEngine->ConnectL();
    }

void CPktPingAppUi::PostConstructL()
{
	DisplayWaitingDialogL();
	iApConnectEngine->ConnectL();
}
void CPktPingAppUi::PingL()
{
	if (CHostResolver::IsIpAddr(iTargetHost))
	{
		iTargetIp = iTargetHost;		
		HBufC* buf = StringLoader::LoadLC(R_PKTPING_PINGING_IP_FORMAT, iTargetIp);
		iPktPingView->AppendTextL(*buf);
		CleanupStack::PopAndDestroy(buf);
		iPingController->PingL(iTargetIp, *iPingSettings);
		Util::SetCbaL(R_PKT_PING_ONGOING_CBA);
	}
	else
	{
		HBufC* buf = StringLoader::LoadLC(R_PKTPING_RESOLVING_HOST);
		iPktPingView->AppendTextL(*buf);
		CleanupStack::PopAndDestroy(buf);
		iHostResolver->GetByNameL(iTargetHost);
	}	    		 	
}
TInt CPktPingAppUi::StaticConstructCallBack(TAny* aVoid)
	{
	TRAPD(err, static_cast<CPktPingAppUi*>(aVoid)->PostConstructL());
	return err;
	}
TInt CPktPingAppUi::StaticStartPingCallBack(TAny* aVoid)
{
	TRAPD(err, static_cast<CPktPingAppUi*>(aVoid)->PingL());
	return err;
}

void CPktPingAppUi::ShowAboutNoteL()
{
	//HBufC* buffer = iEikonEnv->AllocReadResourceLC(R_PKTPING_ABOUT_TEXT_FORMAT);
	HBufC* buffer = HBufC::NewLC(KAboutTextFormat().Length());
	TPtr ptrBuffer(buffer->Des());
	ptrBuffer.Copy(KAboutTextFormat);

    HBufC* swVerBuf = HBufC::NewLC(KPhoneSwVersionTextLength);
    TPtr swVerPtr(swVerBuf->Des());
    User::LeaveIfError(SysUtil::GetSWVersion(swVerPtr));

    TVersion osVer = User::Version();
    TVersionName osVerName = osVer.Name();

    TInt model,machine;
    HAL::Get(HAL::EModel, model);
    HAL::Get(HAL::EMachineUid, machine);
    
    TBuf<25> imei;
    CImeiReader::GetImeiL(imei);

    HBufC* buffer2 = HBufC::NewLC(buffer->Length() + 
        swVerBuf->Length() + osVerName.Length() + 2*KMaxHexDigitsInInteger +
        imei.Length());
    buffer2->Des().Format(*buffer, &*swVerBuf, &osVerName, model, machine, &imei);

    HBufC* title = iEikonEnv->AllocReadResourceLC(R_PKTPING_ABOUT_TITLE);
    CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL (*buffer2, CAknQueryDialog::ENoTone);
    dlg->SetHeaderText(*title);
    dlg->ExecuteLD(R_PKT_PING_LARGE_INFO_MESSAGE); 
    CleanupStack::PopAndDestroy(4); //buffer, buffer2, title, swVerBuf
}
void CPktPingAppUi::ShowInfoNoteL(TInt aResId)
{/*
	HBufC* textResource = StringLoader::LoadLC(aResId);
	CAknInformationNote* informationNote = new ( ELeave ) CAknInformationNote;
	informationNote->ExecuteLD( *textResource);
	CleanupStack::PopAndDestroy(textResource);
	*/
	
	HBufC* textResource = StringLoader::LoadLC(aResId);
	CAknGlobalNote* globalNote = CAknGlobalNote::NewLC();
	globalNote->ShowNoteL(EAknGlobalInformationNote, *textResource); 
	CleanupStack::PopAndDestroy(2); // textResource, globalNote
}
void CPktPingAppUi::ChangeAccessPointL()
{
	CApSettingsHandler *settings =
      CApSettingsHandler::NewLC(
        ETrue, // aStartsWithSelection
        EApSettingsSelListIsPopUp, //EApSettingsSelListIsListPane
        EApSettingsSelMenuSelectOnly,  // aSelMenuType
        KEApIspTypeAll,  // aIspFilter
        EApBearerTypeAll,  // aBearerFilter
        KEApSortNameAscending); // aSortType
    // id (TUint32) is the AP Id to launch for editing
    // RunSettingsL returns after completion
    settings->RunSettingsL(iAp, iAp);
    CleanupStack::PopAndDestroy(); // settings 
}
void CPktPingAppUi::SelectAccessPointL()
{
	iApConnectEngine->TerminateConnectionL();
	iApConnectEngine->ConnectL();
}
void CPktPingAppUi::ChangeStateL(TInt aState)
{
	iConnectState = aState;
	TApConnectionInfo connectInfo = iApConnectEngine->GetApConnectionInfo();
	if (aState == EApConnectStateConnected/* ||
		aState == EApConnectStateNotConnected*/)
	{	
		DisposeWaitingDialogL();
	}
	else if (aState == EApConnectStateNotConnected)
	{
		DisposeWaitingDialogL();
	}
	else
	{
		UpdateWaitingDialogL(connectInfo.ipAddress);
	}
	if (iPktPingSettingView)
	{	
		iPktPingSettingView->UpdateConnectionInfoL(connectInfo);
	}
	if ( iWaitingForPing && (iConnectState == EApConnectStateConnected) )
	{
		HandleCommandL(EPktPingCmdPing);
	}
}
void CPktPingAppUi::DialogDismissedL( TInt aButtonId )
{
	delete iWaitDialog;
	iWaitDialog = NULL;
}
void CPktPingAppUi::DisplayWaitingDialogL()
{
	delete iWaitDialog;
	iWaitDialog = new(ELeave)CAknWaitDialog( (REINTERPRET_CAST(CEikDialog**,&iWaitDialog)));
    iWaitDialog->SetCallback(this);
	iWaitDialog->ExecuteLD(R_PKT_PING_CONNECT_WAIT_NOTE_NO_CANCEL);
}
void CPktPingAppUi::DisposeWaitingDialogL()
{
	if (iWaitDialog)
	{
		iWaitDialog->SetCallback(NULL);
		iWaitDialog->ProcessFinishedL();
		delete iWaitDialog;
		iWaitDialog = NULL;
	}	
}
void CPktPingAppUi::UpdateWaitingDialogL(const TDesC& aTxt)
{
	if (iWaitDialog)
	{
		iWaitDialog->SetTextL(aTxt);
	}
}
void CPktPingAppUi::HostResolvedL(TInt aError, TNameEntry& aResult)
{
	if (aError == KErrNone)
	{
		iPktPingView->ClearTextL();
		TInetAddr localAddr = aResult().iAddr;
		iTargetIp.Zero();
		localAddr.Output( iTargetIp );
		CDesCArrayFlat* strings = new (ELeave) CDesCArrayFlat( 2 );
		CleanupStack::PushL( strings );
		strings->AppendL(iTargetHost);
		strings->AppendL(iTargetIp);
		HBufC* buf = StringLoader::LoadLC(R_PKTPING_PINGING_FORMAT, *strings);
		iPktPingView->AppendTextL(*buf);
		CleanupStack::PopAndDestroy(buf);
		iPingController->PingL(iTargetIp, *iPingSettings);
		Util::SetCbaL(R_PKT_PING_ONGOING_CBA);
	}
	else
	{		
		HBufC* buf = StringLoader::LoadLC(R_PKTPING_UNKNOWN_HOST_NAME);
		iPktPingView->AppendTextL(*buf);
		CleanupStack::PopAndDestroy(buf);
		iPktPingView->HandleCommandL(EPktPingCmdCancelPing);
	}
}
void CPktPingAppUi::NotifyPingProgressL(const TDesC& aStatus)
{
	iPktPingView->AppendTextL(aStatus);
}
void CPktPingAppUi::NotifyErrorL(TInt aError)
{
	TBuf<KResolverMaxErrorMsgLength> errorText;
	errorText = iTextResolver->ResolveErrorString(aError);
	HBufC* buf = StringLoader::LoadLC(R_PKTPING_ERROR_CODE_FORMAT, errorText, aError);
	iPktPingView->AppendTextL(*buf);
	CleanupStack::PopAndDestroy(buf);
}
void CPktPingAppUi::NotifiyEndL()
{
	iPktPingView->HandleCommandL(EPktPingcmdEnd);
}
TBool CPktPingAppUi::InquireTargetHostL(TDes& aAddr)
{
	// create dialog instance 
	CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL(aAddr);

	// Prepares the dialog, constructing it from the specified resource
	dlg->PrepareLC( R_TARGET_HOST_QUERY_DIALOG);
	// Sets the maximum length of the text editor
	//dlg->SetMaxLength(KMaxInputLength);
	// Launch the dialog
	return dlg->RunLD();
}

