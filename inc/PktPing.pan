/*$Id: PktPing.pan 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PKTPING_PAN_H
#define PKTPING_PAN_H

/** PktPing application panic codes */
enum TPktPingPanics
	{
	EPktPingUi = 1
	// add further panics here
	};

inline void Panic(TPktPingPanics aReason)
	{
	_LIT(applicationName,"PktPing");
	User::Panic(applicationName, aReason);
	}

#endif // PKTPING_PAN_H
