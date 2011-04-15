/*$Id: IcmpPacket.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef ICMPPACKET_H
#define ICMPPACKET_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "pingpack.h"

// CLASS DECLARATION

/**
 *  CEchoPacket
 * 
 */
class CEchoPacket : public CBase
{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CEchoPacket();

	/**
	 * Two-phased constructor.
	 */
	static CEchoPacket* NewL(TUint16 aSeqNo, TInt aDataSize);

	/**
	 * Two-phased constructor.
	 */
	static CEchoPacket* NewLC(TUint16 aSeqNo, TInt aDataSize);
	
	const TDesC8& Des();
	TInt SeqNo()
	{
		return iSeqNo;
	}

	// change icmp header for next icmp echo packet
	void Next();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CEchoPacket(TUint16 aSeqNo, TInt aDataSize);

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	TUint16 CheckSum(TUint16* buffer, TInt size);
	
	void Calc();
private:
	TUint16 iSeqNo;
	TInt iDataSize;
	TUint8* iBuf;
	TPtrC8 iPtr;
};

#endif // ICMPPACKET_H
