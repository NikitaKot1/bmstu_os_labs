/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "calculator.h"

bool_t
xdr_CALCULATOR (XDR *xdrs, CALCULATOR *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->op))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->arg1))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->arg2))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->result))
		 return FALSE;
	return TRUE;
}