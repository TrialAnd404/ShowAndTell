package de.hsrm.cs.wwwvs.hamster.rpc;

import java.io.IOException;

public interface Hmstr {
	
	public class State {
		public int treatsLeft;
		public int rounds;
		public int cost;
	}
	
	public class HamsterHandle {
		public int fdptr;
		public HamsterHandle() {
			fdptr = -1;
		}
	}
	
	public class HamsterString {
		public String str;
	}
	
	public class HamsterInteger {
		public int i;
	}
	
	public int new_(String owner_name, String hamster_name, Integer treats) throws HamsterRPCException_NameTooLong, HamsterRPCException_Extists, HamsterRPCException_StorageError, HamsterRPCException_DatabaseCorrupt, IOException, HamsterRPCException;
	public int lookup(String owner_name, String hamster_name) throws HamsterRPCException_NameTooLong, HamsterRPCException_NotFound, HamsterRPCException_StorageError, HamsterRPCException_DatabaseCorrupt, IOException, HamsterRPCException;
	public int directory(HamsterHandle fdptr, String owner_name, String hamster_name) throws HamsterRPCException, HamsterRPCException_NameTooLong, HamsterRPCException_NotFound, HamsterRPCException_StorageError, HamsterRPCException_DatabaseCorrupt, IOException;
	public int howsdoing(Integer ID, State st) throws HamsterRPCException_NotFound, HamsterRPCException_StorageError, HamsterRPCException_DatabaseCorrupt, IOException, HamsterRPCException;
	public int readentry(Integer ID, HamsterString owner, HamsterString name, HamsterInteger price) throws HamsterRPCException_NotFound, HamsterRPCException_StorageError, HamsterRPCException_DatabaseCorrupt, IOException, HamsterRPCException;
	public int givetreats(Integer ID, Integer treats) throws HamsterRPCException_NotFound, HamsterRPCException_StorageError, HamsterRPCException_DatabaseCorrupt, IOException, HamsterRPCException;
	public int collect(String owner_name) throws HamsterRPCException_NotFound, HamsterRPCException_StorageError, HamsterRPCException_DatabaseCorrupt, IOException, HamsterRPCException ;
	
}
