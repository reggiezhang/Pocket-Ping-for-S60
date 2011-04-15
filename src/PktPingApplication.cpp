/*$Id: PktPingApplication.cpp 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#include "PktPingApplication.h"
#include "PktPingDocument.h"
#ifdef EKA2
#include <eikstart.h>
#endif


/**
 * @brief Returns the application's UID (override from CApaApplication::AppDllUid())
 * @return UID for this application (KUidPktPingApplication)
 */
TUid CPktPingApplication::AppDllUid() const
    {
    return KUidPktPingApplication;
    }

/**
 * @brief Creates the application's document (override from CApaApplication::CreateDocumentL())
 * @return Pointer to the created document object (CPktPingDocument)
 */
CApaDocument* CPktPingApplication::CreateDocumentL()
    {
    return CPktPingDocument::NewL( *this );
    }

#ifdef EKA2

/**
 *    @brief Called by the application framework to construct the application object
 *  @return The application (CPktPingApplication)
 */    
LOCAL_C CApaApplication* NewApplication()
    {
    return new CPktPingApplication;
    }

/**
* @brief This standard export is the entry point for all Series 60 applications
* @return error code
 */    
GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }
    
#else     // Series 60 2.x main DLL program code

/**
* @brief This standard export constructs the application object.
* @return The application (CPktPingApplication)
*/
EXPORT_C CApaApplication* NewApplication()
    {
    return new CPktPingApplication;
    }

/**
* @brief This standard export is the entry point for all Series 60 applications
* @return error code
*/
GLDEF_C TInt E32Dll(TDllReason /*reason*/)
    {
    return KErrNone;
    }

#endif // EKA2
