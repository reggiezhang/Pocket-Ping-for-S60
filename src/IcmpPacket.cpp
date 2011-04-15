/*$Id: IcmpPacket.cpp 315 2008-12-15 17:21:50Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */
#include <e32std.h>
#include <e32def.h>
#include <sys\types.h>
#include <netinet\in.h>
#include "pktping.hrh"
#include "IcmpPacket.h"

const TUint8 KAscii_a = 97;
const TUint8 KAscii_z = 122;

CEchoPacket::CEchoPacket(TUint16 aSeqNo, TInt aDataSize) : iSeqNo(aSeqNo), iDataSize(aDataSize)
{
}

CEchoPacket::~CEchoPacket()
{
	delete iBuf;
}

CEchoPacket* CEchoPacket::NewLC(TUint16 aSeqNo, TInt aDataSize)
{
	CEchoPacket* self = new (ELeave)CEchoPacket(aSeqNo, aDataSize);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

CEchoPacket* CEchoPacket::NewL(TUint16 aSeqNo, TInt aDataSize)
{
	CEchoPacket* self=CEchoPacket::NewLC(aSeqNo, aDataSize);
	CleanupStack::Pop(); // self;
	return self;
}

void CEchoPacket::ConstructL()
{
	TInt len = sizeof(TIcmpHdr) + iDataSize;
	iBuf = new (ELeave) TUint8[len];
	iPtr.Set(iBuf, len);
	
	TUint8 ch = KAscii_a;
	for (TInt i = sizeof (TIcmpHdr); i < len; i++)
	{
		if (ch > KAscii_z)
		{
			ch = KAscii_a;
		}
		iBuf[i] = ch++;
	}
	Calc();
}
void CEchoPacket::Calc()
{
	TIcmpHdr* hdr = (TIcmpHdr*)iBuf;
	hdr->icmp_type = ICMP_ECHO;
	hdr->icmp_code = 0;
	hdr->icmp_cksum = 0;
	hdr->icd_id = htons(ICMP_ID);
	hdr->icd_seq = htons(++iSeqNo);
	
	//hdr->timestamp = User::NTickCount();
	hdr->icmp_cksum = CheckSum((TUint16*)iBuf, iPtr.Length());
}
TUint16 CEchoPacket::CheckSum(TUint16* buffer, TInt size)
{
	unsigned long cksum = 0;

	// Sum all the words together, adding the final byte if size is odd
	while (size > 1) {
	cksum += *buffer++;
	size -= sizeof(TUint16);
	}
	if (size) {
	cksum += *(TUint8*)buffer;
	}

	// Do a little shuffling
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);

	// Return the bitwise complement of the resulting mishmash
	return (TUint16)(~cksum);
}
const TDesC8& CEchoPacket::Des()
{
	return iPtr;
}

void CEchoPacket::Next()
{
	Calc();
}
