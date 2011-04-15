/*$Id: pingpack.h 318 2008-12-15 17:25:17Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef PING_PACK_H
#define PING_PACK_H

// borrowed from ip_icmp.h
// ICMP packet type
/**
Definition of type and code field values.
*/
#define	ICMP_ECHOREPLY		0		/* echo reply */
#define	ICMP_UNREACH		3		/* dest unreachable, codes: */
#define		ICMP_UNREACH_NET	0		/* bad net */
#define		ICMP_UNREACH_HOST	1		/* bad host */
#define		ICMP_UNREACH_PROTOCOL	2		/* bad protocol */
#define		ICMP_UNREACH_PORT	3		/* bad port */
#define		ICMP_UNREACH_NEEDFRAG	4		/* IP_DF caused drop */
#define		ICMP_UNREACH_SRCFAIL	5		/* src route failed */
#define	ICMP_SOURCEQUENCH	4		/* packet lost, slow down */
#define	ICMP_REDIRECT		5		/* shorter route, codes: */
#define		ICMP_REDIRECT_NET	0		/* for network */
#define		ICMP_REDIRECT_HOST	1		/* for host */
#define		ICMP_REDIRECT_TOSNET	2		/* for tos and net */
#define		ICMP_REDIRECT_TOSHOST	3		/* for tos and host */
#define	ICMP_ECHO		8		/* echo service */
#define	ICMP_TIMXCEED		11		/* time exceeded, code: */
#define		ICMP_TIMXCEED_INTRANS	0		/* ttl==0 in transit */
#define		ICMP_TIMXCEED_REASS	1		/* ttl==0 in reass */
#define	ICMP_PARAMPROB		12		/* ip header bad */
#define	ICMP_TSTAMP		13		/* timestamp request */
#define	ICMP_TSTAMPREPLY	14		/* timestamp reply */
#define	ICMP_IREQ		15		/* information request */
#define	ICMP_IREQREPLY		16		/* information reply */
#define	ICMP_MASKREQ		17		/* address mask request */
#define	ICMP_MASKREPLY		18		/* address mask reply */
#define	ICMP_MAXTYPE		18

#define	ICMP_INFOTYPE(type) \
	((type) == ICMP_ECHOREPLY || (type) == ICMP_ECHO || \
	(type) == ICMP_TSTAMP || (type) == ICMP_TSTAMPREPLY || \
	(type) == ICMP_IREQ || (type) == ICMP_IREQREPLY || \
	(type) == ICMP_MASKREQ || (type) == ICMP_MASKREPLY)

struct TIpHdr
{
	// LITTLE_ENDIAN
	TInt8 ip_hl :4; // header length
	TInt8 ip_v :4; // version
	TInt8 ip_tos; // type of service
	TUint16 ip_len; // total length
	TUint16 ip_id; // identification
	TUint16 ip_off; // fragment offset field
	TUint8 ip_ttl; // time to live
	TUint8 ip_p; // protocol (TCP, UDP etc)
	TUint16 ip_sum; // checksum
	TUint32 ip_src; // struct in_addr
	TUint32 ip_dst;   // struct in_addr
};
#define IP_HDR_LEN sizeof(TIpHdr)

// ICMP packet header
struct TIcmpHdr {
	TUint8 icmp_type; // type of message, see below
	TUint8 icmp_code; // type sub code
	TUint16 icmp_cksum; // ones complement checksum of struct
	TUint16 icd_id;         /* echo datagram */
	TUint16 icd_seq;
	//TUint32 timestamp; /* This is not the std header, but we reserve space for time */
};

#define ICMP_HDR_LEN sizeof(TIcmpHdr)

#endif // PING_PACK_H

