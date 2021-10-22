package de.hsrm.cs.wwwvs.hamster.rpc.client;

import java.nio.ByteBuffer;

public class RPCMessageHeader {
	
	public int payloadSize;
	public int msgID;
	public boolean isResonse;
	public boolean isError;
	public byte version;
	public short rpcCallId;
	
	
	public RPCMessageHeader(ByteBuffer buf) {
	
		// parse header
		
		version = buf.get();
		byte tmp = buf.get();
		
		if ((tmp & HamsterRPCConnection.protocolFlagResponse) > 0) {
			isResonse =  true;
		}
		
		if ((tmp & HamsterRPCConnection.protocolFlagError) > 0) {
			isError = true;
		}
		
		msgID = buf.getShort();
		
		payloadSize = buf.getShort();
		
		rpcCallId = buf.getShort();
		
		
	}
}
