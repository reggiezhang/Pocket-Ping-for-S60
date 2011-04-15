/*$Id: PktPingApplication.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PKTPINGAPPLICATION_H
#define PKTPINGAPPLICATION_H

#include <aknapp.h>

const TUid KUidPktPingApplication = { 0x20020D05 };

/**
 *
 * @class    CPktPingApplication PktPingApplication.h
 * @brief    A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CPktPingApplication : public CAknApplication
    {
private:
    TUid AppDllUid() const;
    CApaDocument* CreateDocumentL();
    
    };
            
#endif // PKTPINGAPPLICATION_H        
