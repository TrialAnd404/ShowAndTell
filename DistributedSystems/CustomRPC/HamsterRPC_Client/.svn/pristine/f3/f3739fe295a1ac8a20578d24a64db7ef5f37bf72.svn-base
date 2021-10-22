package de.hsrm.cs.wwwvs.hamster.rpc.cli_client;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Scanner;

import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_DatabaseCorrupt;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_NameTooLong;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_NotFound;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_StorageError;
import de.hsrm.cs.wwwvs.hamster.rpc.Hmstr;
import de.hsrm.cs.wwwvs.hamster.rpc.client.HamsterRPCConnection;

public class CLIClient {

	private static Scanner scanner;

	private static void rtfm(String progname) {
		System.out.println("Usage: " + progname + " {<Option>} <param1> \n");
		System.out.println("Function: Hamster Java CLI Client\n");
		System.out.println("Optionen:\n");
		System.out.println("     -h {<hostname>}                - hostname of the server\n");
		System.out.println("     -p {<port>}                    - port of the server\n");

	}

	private static void cli_commands() {
		System.out.println("\n" + "Commands:");
		System.out.println("        q                           - quit");
		System.out.println("        n                           - check in new hamster");
		System.out.println("        b                           - collect hamsters");
		System.out.println("        l                           - list all hamsters");
		System.out.println("        s                           - show hamster status");
		System.out.println("        f                           - feed hamster");
		System.out.print("\n> ");
	}
	private static int get_id(Hmstr hamsterserver, Scanner scanner)  throws InputMismatchException, HamsterRPCException_NameTooLong, HamsterRPCException_NotFound, HamsterRPCException_StorageError, HamsterRPCException_DatabaseCorrupt, IOException, HamsterRPCException {
		System.out.print("Owner Name: ");
		String owner = scanner.next();
		System.out.print("Hamster Name: ");
		String hamster = scanner.next();
		return hamsterserver.lookup(owner, hamster);
	}

	
	public static void main(String[] args) {
		

		/*
		 * predefined hostname and port
		 */
		String hostname = "localhost";
		int port = 2323;

		/*
		 * parse command line arguments
		 */
		for (int i = 0; i < args.length; i++) {
			switch (args[i]) {
			case ("-h"):
				if (++i >= args.length) {
					System.err.println("Error, missing argument");
					rtfm("HamsterRPC_Client");
					return;
				}
				hostname = args[i];
				hostname = hostname.trim();
				break;
			case ("-p"):
				if (++i >= args.length) {
					System.err.println("Error, missing argument");
					rtfm("HamsterRPC_Client");
					return;
				}
				String tmp = args[i];
				tmp = tmp.trim();
				port = Integer.parseInt(tmp);
				break;
			}
		}

		/*
		 * connect to server
		 */
		Hmstr hamsterserver = null;

		try {
			hamsterserver = new HamsterRPCConnection(hostname, port);
		} catch (UnknownHostException e) {
			System.out.println("Unknown host - Msg: " + e.getMessage());
			return;
		} catch (IOException e) {
			System.out.println("Connection error - Msg: " + e.getMessage());
			return;
		}

		scanner = new Scanner(System.in);
		String cmd = "";

		while (!cmd.equals("q")) {
			cli_commands();
			cmd = scanner.next();

			String owner = null;
			String name = null;
			int treats = 0;
			int id = 0;

			switch (cmd) {
			case ("n"):
				System.out.println("checking in new hamster:");
				System.out.print("        owner: ");
				owner = scanner.next();

				System.out.print("        hamster name: ");
				name = scanner.next();

				System.out.print("        treats: ");
				try {
					treats = scanner.nextInt();
				} catch (InputMismatchException e) {
					System.out.println("Not a valid number, treats is set to 0.");
					// flush the invalid input
					scanner.next();
				}

				try {
					id = hamsterserver.new_(owner, name, treats);
					System.out.println("(" + owner + "," + name + ") ID: " + id);
				} catch (IOException | HamsterRPCException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;
			case ("b"):
				System.out.println("collecting hamsters:");
				System.out.print("        owner: ");
				owner = scanner.next();

				try {
					int retCode = hamsterserver.collect(owner);
					System.out.println("return code: " + retCode);
				} catch (IOException | HamsterRPCException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;
			case ("l"):
				Hmstr.HamsterHandle handle = new Hmstr.HamsterHandle();

				ArrayList<HamsterEntry> entries = new ArrayList<>();

				try {
					int ret = 0;
					while (ret >= 0) {
						ret = hamsterserver.directory(handle, null, null);
						// System.out.println("HamsterID " + ret);

						Hmstr.HamsterString dirOwner = new Hmstr.HamsterString();
						Hmstr.HamsterString dirName = new Hmstr.HamsterString();
						Hmstr.HamsterInteger dirPrice = new Hmstr.HamsterInteger();
						treats = hamsterserver.readentry(ret, dirOwner, dirName, dirPrice);

						// System.out.println(dirOwner.str + " " + dirName.str + " " + dirPrice.i + " "
						// + treats);
						entries.add(new HamsterEntry(Integer.toString(ret), dirOwner.str, dirName.str,
								Integer.toString(dirPrice.i), Integer.toString(treats)));

					}
				} catch (HamsterRPCException_NotFound ee) {
					System.out.println("                                        Hamster Date Base");
					System.out.println(
							"===================================================================================================");
					System.out.println(
							"ID        Owner Name                      Hamster Name                    Price €      Treats left");
					for (HamsterEntry entry : entries) {
						System.out.print(entry.id + " ");
						System.out.print(entry.owner);
						for (int i = entry.owner.length(); i < 32; i++)
							System.out.print(" ");
						System.out.print(entry.name);
						for (int i = entry.name.length(); i < 32; i++)
							System.out.print(" ");
						System.out.print(entry.price);
						for (int i = entry.price.length(); i < 13; i++)
							System.out.print(" ");
						System.out.println(entry.treats);

					}
				} catch (HamsterRPCException | IOException e1) {

					System.out.println("Got error " + e1.toString());
				}
				break;
			case ("s"):
				try {
					id = get_id(hamsterserver, scanner);
				} catch (InputMismatchException | IOException | HamsterRPCException e) {
					System.out.println("Cannot get ID of Hamster.");
					// flush the invalid input
					continue;
				}
				
				Hmstr.State state = new Hmstr.State();
				try {
					int ret = hamsterserver.howsdoing(id, state);
					System.out.println("return code: "+ret);
					System.out.println("         Hamster State");
					System.out.println("==================================");
					System.out.println("treats left: " + state.treatsLeft);
					System.out.println("rounds:      " + state.rounds);
					System.out.println("cost:        " + state.cost);
				} catch (IOException | HamsterRPCException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				break;
			case ("f"):
				try {
					id = get_id(hamsterserver, scanner);
				} catch (InputMismatchException | IOException | HamsterRPCException e) {
					System.out.println("Cannot get ID of Hamster.");
					// flush the invalid input
					continue;
				}
				
				System.out.print("        treats: ");
				try {
					treats = scanner.nextInt();
				} catch (InputMismatchException e) {
					System.out.println("Not a valid number.");
					// flush the invalid input
					scanner.next();
					continue;
				}

				try {
					int ret = hamsterserver.givetreats(id, treats);
					System.out.println("return code: "+ret);
				} catch (IOException | HamsterRPCException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				break;
			case ("q"):
				System.out.println("quit");
				break;
			default:
				System.out.println("unknown command\n");
			}
		}

	}

}
