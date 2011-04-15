/*$Id: PingSettings.h 312 2008-12-15 06:05:46Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PINGSETTINGS_H
#define PINGSETTINGS_H 

// INCLUDES
#include <e32std.h>
#include <e32base.h>
const int KMyAddressMaxLength = 255;
// CLASS DECLARATION

/**
 *  TPingSettings
 * 
 */
class TPingSettings
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~TPingSettings();

	/**
	 * Two-phased constructor.
	 */
	static TPingSettings* NewL();
	
	TDes& MyAddress();
	void SetMyAddress(const TDesC& aValue);
	
	TInt& NrOfPing();
	TInt NrOfPing() const;
    //void SetNrOfPing(const TInt& aValue);
    
    TInt& DataSize();
    TInt DataSize() const;
    //void SetDataSize(const TInt& aValue);
    
    TBool& DontFragment();
    TBool DontFragment() const;
    //void SetDontFragment(const TBool& aValue);
    
    TInt& Ttl();
    TInt Ttl() const;
    //void SetTtl(const TInt& aValue);
    void ResetMyAddressL();


private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	TPingSettings();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
private:
	TBuf<KMyAddressMaxLength> iMyAddress;
	TInt iNrOfPing;
	TInt iDataSize;
	TBool iDontFragment;
	TInt iTtl;

};

#endif // PINGSETTINGS_H
