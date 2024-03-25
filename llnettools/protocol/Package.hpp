/*
 * HTTP.hpp
 *
 *  Created on: Mar 16, 2023
 *      Author: llanyro
 */

#ifndef LLCPP_NET_PROTOCOL_PACKAGE_HPP_
#define LLCPP_NET_PROTOCOL_PACKAGE_HPP_

#include <llanytypeslib.hpp>
#include <lllibbits.hpp>

#include "../../list/vector/VectorList.hpp"

namespace llcpp {
namespace net {
namespace package {

enum class DNSHeaderOptions {
	QR_NOT	= 0x000,
	QR		= 0x001,
	QR2		= 0x00000000,


};

typedef ll_uchar_t bytes;

struct DNSHeaderStruct {
	/*
	                                1  1  1  1  1  1
	  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|					   ID						|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|                    QDCOUNT                    |
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|                    ANCOUNT                    |
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|                   NSCOUNT                     |
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|                   ARCOUNT                     |
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*/
	/*
	 *	ID A 16 bit identifier assigned by the program that generates any kind of query.
	 *		This identifier is copied the corresponding reply and can be used by the requester 
	 *		to match up replies to outstanding queries. You should select a new, 
	 *		random 16 bit number for each request.
	 */
	ui16 ID;
	/*
	 *	QR A one bit field that specifies whether this message is a query (0), or a response (1). Obviously,
	 *		you should use 0 for your requests, and expect to see a 1 in the response you receive.
	 * 
	 *	OPCODE A four bit field that specifies kind of query in this message.
	 *		You should use 0, representing a standard query.
	 * 
	 *	AA Authoritative Answer - this bit is only meaningful in responses,
	 *		and specifies that the responding name server is an authority for the domain
	 *		name in question section. You should use this bit to report whether or not
	 *		the response you receive is authoritative.
	 * 
	 *	TC TrunCation - specifies that this message was truncated.
	 *		For this project, you must exit and return an error if you receive a response
	 *		that is truncated.
	 * 
	 * 	RD Recursion Desired - this bit directs the name server to pursue the query recursively.
	 *		You should use 1, representing that you desire recursion.
	 * 
	 * 	RA Recursion Available - this be is set or cleared in a response,
	 *		and denotes whether recursive query support is available in the name server.
	 *		Recursive query support is optional.
	 *		You must exit and return an error if you receive a response that
	 *		indicates the server does not support recursion.
	 * 
	 * Z Reserved for future use. You must set this field to 0.
	 * 
	 *	RCODE Response code - this 4 bit field is set as part of responses.
	 *		The values have the following interpretation:
	 *		0 No error condition
	 *		1 Format error - The name server was unable to interpret the query.
	 *		2 Server failure - The name server was unable to process this query due to a
	 *			problem with the name server.
	 *		3 Name Error - Meaningful only for responses from an authoritative name server,
	 *			this code signifies that the domain name referenced in the query does not exist.
	 *		4 Not Implemented - The name server does not support the requested kind of query.
	 *		5 Refused - The name server refuses to perform the specified operation for
	 *			policy reasons.
	 *	You should set this field to 0, and should assert an error if you receive a response
	 *		indicating an error condition. You should treat 3 differently, as this represents
	 *		the case where a requested name doesn’t exist.
	 */
	ui16 OPTIONS; // QR, Opcode, AA, TC, RD, RA, Z, RCODE
	/*
	 * QDCOUNT an unsigned 16 bit integer specifying the number of entries in
	 *		the question section. You should set this field to 1, indicating you have one question.
	 */
	ui16 QDCOUNT;
	/*
	 * ANCOUNT an unsigned 16 bit integer specifying the number of resource records in the answer
	 *		section. You should set this field to 0, indicating you are not providing any answers.
	 */
	ui16 ANCOUNT;
	/*
	 * NSCOUNT an unsigned 16 bit integer specifying the number of name server resource records
	 *		in the authority records section.
	 *		You should set this field to 0, and should ignore any response entries in this section.
	 */
	ui16 NSCOUNT;
	/*
	 * ARCOUNT an unsigned 16 bit integer specifying the number of resource records in the
	 *		additional records section. You should set this field to 0,
	 *		and should ignore any response entries in this section.
	 */
	ui16 ARCOUNT;
};

struct DNSQuestionsStruct {
	/*
	                                1  1  1  1  1  1
	  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|												|
	|					   QNAME					|
	|												|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|                      QTYPE					|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|					  QCLASS					|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*/

	/*
	 *	QNAME A domain name represented as a sequence of labels, where each label 
	 *		consists of a length octet followed by that number of octets. 
	 *		The domain name terminates with the zero length octet for the null label of the root. 
	 */
	ui16 name;
	/*
	 *	QTYPE A two octet code which specifies the type of the query.
	 *		You should use 0x0001 for this project, representing A records (host addresses).
	 *		If you are completing the graduate version of this project, you will also need
	 *		to use 0x000f for mail server (MX) records and 0x0002 for name servers (NS) records.
	 */
	ui16 type;
	/*
	 *	QCLASS A two octet code that specifies the class of the query.
	 *		You should always use 0x0001 for this project, representing Internet addresses
	 */
	ui16 qclass;
};

struct DNSAnswersStruct {
	/*
									1  1  1  1  1  1
	  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|												|
	|					    NAME					|
	|												|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|                       TYPE					|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|					   CLASS					|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|					    TTL 					|
	|												|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|					  RDLENGHT					|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	|					  RDDATA					|
	+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
	*/

	/*
		NAME The domain name that was queried, in the same format as the QNAME in the questions.

		TYPE Two octets containing one of th type codes. This field specifies the meaning 
			of the data in the RDATA field. You should be prepared to interpret 
			type 0x0001 (A record) and type 0x0005 (CNAME). If you are completing the graduate 
			version of this project, you should also be prepared to accept type 
			0x0002 (name servers) and 0x000f (mail servers).

		CLASS Two octets which specify the class of the data in the RDATA field. 
			You should expect 0x0001 for this project, representing Internet addresses.

		TTL The number of seconds the results can be cached.

		RDLENGTH The length of the RDATA field.

		RDATA The data of the response. The format is dependent on the TYPE field: 
			if the TYPE is 0x0001 for A records, then this is the IP address 
			(4 octets). If the type is 0x0005 for CNAMEs, then this is the name of the alias.
			If the type is 0x0002 for name servers, then this is the name of the server.
			Finally if the type is 0x000f for mail servers, the format is

											1  1  1  1  1  1
			  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
			+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			|				    PREFERENCE					|
			+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			|					EXCHANGE					|
			|												|
			+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
			where PREFERENCE is a 16 bit integer which specifies the preference of 
			this mail server, and EXCHANGE is a domain name stored in the same format as QNAMEs.
			The latter two types are only relevant for the graduate version of this project.
	*/

	/*
	 *	QNAME A domain name represented as a sequence of labels, where each label 
	 *		consists of a length octet followed by that number of octets. 
	 *		The domain name terminates with the zero length octet for the null label of the root. 
	 */
	ll_uchar_t name[2];
	/*
	 *	QTYPE A two octet code which specifies the type of the query.
	 *		You should use 0x0001 for this project, representing A records (host addresses).
	 *		If you are completing the graduate version of this project, you will also need
	 *		to use 0x000f for mail server (MX) records and 0x0002 for name servers (NS) records.
	 */
	ll_uchar_t type[2];
	/*
	 *	QCLASS A two octet code that specifies the class of the query.
	 *		You should always use 0x0001 for this project, representing Internet addresses
	 */
	ll_uchar_t qclass[2];
};



/********************************************************/

enum class DNSQuestionType {
	HOST_ADDRESS = 0x0001,
	MAIL_SERVER = 0x000f,
	NAME_SERVER = 0x0002,
};
enum class DNSQuestionClass {
	CLASS_1 = 0x0001,
};

class DNSPacket {
	protected:
		ui8* name; len_t nameLen;
		DNSQuestionType type;
		DNSQuestionClass qclass;
	public:
		DNSPacket()
			: name(LL_NULLPTR), nameLen(0ull)
			, type(DNSQuestionType::HOST_ADDRESS), qclass(DNSQuestionClass::CLASS_1)
		{}

		llcpp::list::vector::VectorList<ui8>* formatPackage() {
			llcpp::list::vector::VectorList<ui8>* data =
				new llcpp::list::vector::VectorList<ui8>();


		}
};



} /* namespace package */
} /* namespace net */
} /* namespace llcpp */

#endif /* LLCPP_NET_PROTOCOL_PACKAGE_HPP_ */
