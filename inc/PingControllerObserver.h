/*$Id: PingControllerObserver.h 329 2008-12-20 20:03:54Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PING_CONTROLLER_OBSERVER
#define PING_CONTROLLER_OBSERVER
#include <e32std.h>
#include <e32base.h> 
class MPingControllerObserver
{
public:
	virtual void NotifyPingProgressL(const TDesC& aStatus) = 0;
	virtual void NotifyErrorL(TInt aError) = 0;
	virtual void NotifiyEndL() = 0;
};

#endif // PING_CONTROLLER_OBSERVER
