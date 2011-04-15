/*$Id: ImeiReader.h 312 2008-12-15 06:05:46Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef IMEIREADER_H
#define IMEIREADER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <Etel3rdParty.h>

#include <e32base.h>
#include <Etel3rdParty.h>

class CImeiReader : public CActive
{
public:
	static void GetImeiL(TDes& aImei);
private:
	CImeiReader(TDes& aImei);
	~CImeiReader();
	void ConstructL();
	CTelephony* iTelephony;
	CTelephony::TPhoneIdV1 iPhoneIdV1;
	CTelephony::TPhoneIdV1Pckg iPhoneIdV1Pckg;
	void RunL();
	void DoCancel();
private:
	TDes& iImei;
};

#endif // IMEIREADER_H
