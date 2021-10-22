package de.hsrm.cs.wwwvs.hamster.rpc.client;

import java.io.IOException;
import java.net.UnknownHostException;

import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException;
import de.hsrm.cs.wwwvs.hamster.rpc.Hmstr;

public class Client {
	
	private static void rtfm(String progname) {
		System.out.println("Usage: " + progname + " {<Option>} <param1> \n");
		System.out.println("Function: Hamster Java Client\n");
		System.out.println("Optionen:\n");
		System.out.println("     -h {<hostname>}                - hostname of the server\n");
		System.out.println("     -p {<port>}                    - port of the server\n");
		
	}

	public static void main(String[] args) {
		

		if (args.length < 2) {
			System.err.println("Error, missing argument");
			rtfm("HamsterRPC_Client");
			return;
		}
		
		String hostname = "localhost";
		int port = 2323;
		
		for (int i = 0; i < args.length; i++) {
			switch (args[i]) {
			case ("-h"):
				hostname = args[i+1];
				hostname = hostname.trim();
				break;
			case ("-p"):
				String tmp = args[i+1];
				tmp = tmp.trim();
				port = Integer.parseInt(tmp);
				break;
			}
		}
		
		Hmstr hamsterserver = null;
		
		try {
			hamsterserver = new HamsterRPCConnection(hostname, port, true);
		} catch (UnknownHostException e) {
			System.out.println("Unknown host - Msg: " + e.getMessage());
			return ;
		} catch (IOException e) {
			System.out.println("Connection error - Msg: " + e.getMessage());
			return ;
		}
		
		
		
		try {
			int id = hamsterserver.new_("Harald", "Goldi", 23);
			System.out.println("New ID: " + id);
			
		} catch (IOException | HamsterRPCException e) {
			System.out.println("Got error "  + e.toString());
		}
			
		
		
		/*
		try {
			Hmstr.HamsterHandle handle = new Hmstr.HamsterHandle();
			
			int ret = 0;
			
			while (ret >= 0) {
				ret = hamsterserver.directory(handle, null, null);
				System.out.println("HamsterID " + ret);
				
				Hmstr.HamsterString owner = new Hmstr.HamsterString();
				Hmstr.HamsterString name = new Hmstr.HamsterString();
				Hmstr.HamsterInteger price = new Hmstr.HamsterInteger();
				int t = hamsterserver.readentry(ret, owner, name, price);
				
				System.out.println("Owner name " + owner.str + " Hamster name " + name.str + " price " + price.i + " treats left " + t);
				
			}
		}
		catch (HamsterRPCException_NotFound ee) {
				
		}
		catch (HamsterRPCException | IOException e1) {
			
			System.out.println("Got error "  + e1.toString());
		}
		*/
		
		
			//int id2 = hamsterserver.lookup("otto", "heinz");
			
			//System.out.println("look up id: " + id2);
			
		//	Hmstr.State state = new Hmstr.State();
	//		int r = hamsterserver.howsdoing(id, state);
			
	//		System.out.println("ret code " + r );
	//		System.out.println("state: treats left " + state.treatsLeft + " rounds " + state.rounds + " cost " + state.cost);
			
	//		r = hamsterserver.givetreats(id, 20);
	//		System.out.println("ret code " + r );
			
	//		r = hamsterserver.howsdoing(id, state);
	//		System.out.println("ret code " + r );
	//		System.out.println("state: treats left " + state.treatsLeft + " rounds " + state.rounds + " cost " + state.cost);
			
	//		hamsterserver.collect("");
			
			
		

	}

}
