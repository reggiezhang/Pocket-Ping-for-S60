/*$Id: PktPingDocument.cpp 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include "PktPingDocument.h"
#include "PktPingAppUi.h"

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CPktPingDocument::CPktPingDocument( CEikApplication& anApplication )
    : CAknDocument( anApplication )
    {
    }

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */ 
void CPktPingDocument::ConstructL()
    {
    }
    
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CPktPingDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CPktPingDocument
 */
CPktPingDocument* CPktPingDocument::NewL( CEikApplication& aApp )
    {
    CPktPingDocument* self = new ( ELeave ) CPktPingDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */    
CEikAppUi* CPktPingDocument::CreateAppUiL()
    {
    return new ( ELeave ) CPktPingAppUi;
    }
                
