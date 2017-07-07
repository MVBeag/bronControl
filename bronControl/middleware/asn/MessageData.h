/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "OTACommProtocolModule"
 * 	found in "../BronComm.asn"
 */

#ifndef	_MessageData_H_
#define	_MessageData_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include "MessageSetParameter.h"
#include "MessageResult.h"
#include "MessageError.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MessageData_PR {
	MessageData_PR_NOTHING,	/* No components present */
	MessageData_PR_mgi,
	MessageData_PR_mgs,
	MessageData_PR_ms,
	MessageData_PR_mr,
	MessageData_PR_me
} MessageData_PR;

/* MessageData */
typedef struct MessageData {
	MessageData_PR present;
	union MessageData_u {
		NULL_t	 mgi;
		NULL_t	 mgs;
		MessageSetParameter_t	 ms;
		MessageResult_t	 mr;
		MessageError_t	 me;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MessageData_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MessageData;

#ifdef __cplusplus
}
#endif

#endif	/* _MessageData_H_ */
#include <asn_internal.h>
