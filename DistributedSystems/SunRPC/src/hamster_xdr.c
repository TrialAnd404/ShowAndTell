/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "hamster.h"

bool_t
xdr_nameString (XDR *xdrs, nameString *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, 32))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstrState (XDR *xdrs, hmstrState *objp)
{
	register int32_t *buf;

	 if (!xdr_uint16_t (xdrs, &objp->treats_left))
		 return FALSE;
	 if (!xdr_uint32_t (xdrs, &objp->rounds))
		 return FALSE;
	 if (!xdr_int16_t (xdrs, &objp->cost))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_new_in (XDR *xdrs, hmstr_new_in *objp)
{
	register int32_t *buf;

	 if (!xdr_nameString (xdrs, &objp->owner))
		 return FALSE;
	 if (!xdr_nameString (xdrs, &objp->hamster))
		 return FALSE;
	 if (!xdr_uint16_t (xdrs, &objp->treats))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_new_out (XDR *xdrs, hmstr_new_out *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->id))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_lookup_in (XDR *xdrs, hmstr_lookup_in *objp)
{
	register int32_t *buf;

	 if (!xdr_nameString (xdrs, &objp->owner))
		 return FALSE;
	 if (!xdr_nameString (xdrs, &objp->hamster))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_lookup_out (XDR *xdrs, hmstr_lookup_out *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->id))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_directory_in (XDR *xdrs, hmstr_directory_in *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->fdptr))
		 return FALSE;
	 if (!xdr_nameString (xdrs, &objp->owner))
		 return FALSE;
	 if (!xdr_nameString (xdrs, &objp->hamster))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_directory_out (XDR *xdrs, hmstr_directory_out *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->id))
		 return FALSE;
	 if (!xdr_int32_t (xdrs, &objp->fdptr))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_howsdoing_in (XDR *xdrs, hmstr_howsdoing_in *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->id))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_howsdoing_out (XDR *xdrs, hmstr_howsdoing_out *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->retVal))
		 return FALSE;
	 if (!xdr_hmstrState (xdrs, &objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_readentry_in (XDR *xdrs, hmstr_readentry_in *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->id))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_readentry_out (XDR *xdrs, hmstr_readentry_out *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->treats))
		 return FALSE;
	 if (!xdr_nameString (xdrs, &objp->owner))
		 return FALSE;
	 if (!xdr_nameString (xdrs, &objp->name))
		 return FALSE;
	 if (!xdr_int16_t (xdrs, &objp->price))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_givetreats_in (XDR *xdrs, hmstr_givetreats_in *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->id))
		 return FALSE;
	 if (!xdr_uint16_t (xdrs, &objp->treats))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_givetreats_out (XDR *xdrs, hmstr_givetreats_out *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->treats))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_collect_in (XDR *xdrs, hmstr_collect_in *objp)
{
	register int32_t *buf;

	 if (!xdr_nameString (xdrs, &objp->owner))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_hmstr_collect_out (XDR *xdrs, hmstr_collect_out *objp)
{
	register int32_t *buf;

	 if (!xdr_int32_t (xdrs, &objp->price))
		 return FALSE;
	return TRUE;
}