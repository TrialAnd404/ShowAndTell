/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "hamster.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

hmstr_collect_out *
hmstr_collect_rpc_1(hmstr_collect_in *argp, CLIENT *clnt)
{
	static hmstr_collect_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, HMSTR_COLLECT_RPC,
		(xdrproc_t) xdr_hmstr_collect_in, (caddr_t) argp,
		(xdrproc_t) xdr_hmstr_collect_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

hmstr_directory_out *
hmst_directory_rpc_1(hmstr_directory_in *argp, CLIENT *clnt)
{
	static hmstr_directory_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, HMST_DIRECTORY_RPC,
		(xdrproc_t) xdr_hmstr_directory_in, (caddr_t) argp,
		(xdrproc_t) xdr_hmstr_directory_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

hmstr_givetreats_out *
hmstr_givetreats_rpc_1(hmstr_givetreats_in *argp, CLIENT *clnt)
{
	static hmstr_givetreats_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, HMSTR_GIVETREATS_RPC,
		(xdrproc_t) xdr_hmstr_givetreats_in, (caddr_t) argp,
		(xdrproc_t) xdr_hmstr_givetreats_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

hmstr_howsdoing_out *
hmstr_howsdoing_rpc_1(hmstr_howsdoing_in *argp, CLIENT *clnt)
{
	static hmstr_howsdoing_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, HMSTR_HOWSDOING_RPC,
		(xdrproc_t) xdr_hmstr_howsdoing_in, (caddr_t) argp,
		(xdrproc_t) xdr_hmstr_howsdoing_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

hmstr_lookup_out *
hmstr_lookup_rpc_1(hmstr_lookup_in *argp, CLIENT *clnt)
{
	static hmstr_lookup_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, HMSTR_LOOKUP_RPC,
		(xdrproc_t) xdr_hmstr_lookup_in, (caddr_t) argp,
		(xdrproc_t) xdr_hmstr_lookup_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

hmstr_readentry_out *
hmstr_readentry_rpc_1(hmstr_readentry_in *argp, CLIENT *clnt)
{
	static hmstr_readentry_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, HMSTR_READENTRY_RPC,
		(xdrproc_t) xdr_hmstr_readentry_in, (caddr_t) argp,
		(xdrproc_t) xdr_hmstr_readentry_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

hmstr_new_out *
hmstr_new_rpc_1(hmstr_new_in *argp, CLIENT *clnt)
{
	static hmstr_new_out clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, HMSTR_NEW_RPC,
		(xdrproc_t) xdr_hmstr_new_in, (caddr_t) argp,
		(xdrproc_t) xdr_hmstr_new_out, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
