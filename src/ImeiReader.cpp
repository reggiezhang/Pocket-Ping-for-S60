/*$Id: ImeiReader.cpp 312 2008-12-15 06:05:46Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include "ImeiReader.h"

void CImeiReader::GetImeiL(TDes& aImei)
{
	CImeiReader* self= new (ELeave) CImeiReader(aImei);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::PopAndDestroy(self);
}
 
void CImeiReader::ConstructL()
{
	iTelephony = CTelephony::NewL();
	CActiveScheduler::Add(this);
	
	iTelephony->GetPhoneId(iStatus, iPhoneIdV1Pckg);
	SetActive();
	CActiveScheduler::Start();
}
 
CImeiReader:: CImeiReader(TDes& aImei): CActive(EPriorityStandard),iImei(aImei),iPhoneIdV1Pckg(iPhoneIdV1)
{
    //default constructor
}
 
CImeiReader::~CImeiReader()
{
    delete iTelephony;
    iTelephony = NULL;
}
 
void CImeiReader::RunL()
{
    if(iStatus==KErrNone)
    {
	iImei= iPhoneIdV1.iSerialNumber;
	CActiveScheduler::Stop();
    }
}
 
void CImeiReader::DoCancel()
{
    iTelephony->CancelAsync(CTelephony::EGetPhoneIdCancel);
}
