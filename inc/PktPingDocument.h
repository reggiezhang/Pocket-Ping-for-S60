/*$Id: PktPingDocument.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PKTPINGDOCUMENT_H
#define PKTPINGDOCUMENT_H

#include <akndoc.h>
        
class CEikAppUi;

/**
* @class    CPktPingDocument PktPingDocument.h
* @brief    A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CPktPingDocument : public CAknDocument
    {
public: 
    // constructor
    static CPktPingDocument* NewL( CEikApplication& aApp );

private: 
    // constructors
    CPktPingDocument( CEikApplication& aApp );
    void ConstructL();
    
public: 
    // from base class CEikDocument
    CEikAppUi* CreateAppUiL();
    };
#endif // PKTPINGDOCUMENT_H
