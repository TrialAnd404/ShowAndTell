package de.hsrm.cs.wwwvs.hamster.tests.suite;

import static org.junit.Assert.*;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.concurrent.TimeUnit;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.Timeout;

import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_DatabaseCorrupt;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_NameTooLong;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_NotFound;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_StorageError;
import de.hsrm.cs.wwwvs.hamster.rpc.client.HamsterRPCConnection;
import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

public class TestLookup {

	private static Process sut = null;
	static HamsterTestDataStore store = HamsterTestDataStore.getInstance();	
	static HamsterRPCConnection hmstr = null;
	
	static int port = store.getPort();
	static String hostname = "localhost";
	
	@Rule
	public Timeout globalTimeout= new Timeout(HamsterTestDataStore.getInstance().testcaseTimeoutms, TimeUnit.MILLISECONDS);
	
	@BeforeClass
	public static void setUpBeforeClass() {
		String sutPath = store.getSUT();
		
		try {
			sut = Runtime.getRuntime().exec(sutPath + " -p " + port);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to start server.");
		}
		
		assertTrue("Server process is not running.", sut.isAlive());
		
		HamsterTestDataStore.sleepMin();
		
	}

	@AfterClass
	public static void tearDownAfterClass() {
		sut.destroy();

		HamsterTestDataStore.sleepMin();

		assertFalse("Server process is not shuting down.", sut.isAlive());
	}
	
	@Before
	public void setUp() {
		
		assertTrue("Server process is not running.", sut.isAlive());
		
		try {
			hmstr = new HamsterRPCConnection(hostname, port, true);
		} catch (UnknownHostException e) {
			e.printStackTrace();
			fail("Failed to connect to server: " + e.getMessage());
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to connect to server: " + e.getMessage());
		}
		
		HamsterTestDataStore.getInstance().wipeHamsterfile();
		
		HamsterTestDataStore.sleepMin();
		
	}	
	
	@After
	public void tearDown() {
		try {
			hmstr.close();
		} catch (IOException e) {
			e.printStackTrace();
			fail("Connection failed");
		} 
		HamsterTestDataStore.sleepMin();
	}

	@Test
	public void lookup_td1() {
		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat"));
		
		String owner_name = "otto";
		String hamster_name = "heinz";
		int expectedUUID = 1996485908;
		
		int UUID = -1;
		
		
		try {
			UUID = hmstr.lookup(owner_name, hamster_name);
			
			assertTrue("lookup returned the UUID " + UUID + ", but " + expectedUUID + " was expected.", expectedUUID == UUID);
			
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_NotFound e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_StorageError e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		}
	}
	
	@Test
	public void lookup_not_found() {
		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat"));
		
		String owner_name = "not";
		String hamster_name = "found";
		
		try {
			hmstr.lookup(owner_name, hamster_name);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_NotFound e) {
			assertTrue(e instanceof HamsterRPCException_NotFound);
		} catch (HamsterRPCException_StorageError e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		}
	}
	
	@Test
	public void lookup_empty_owner() {
		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat"));
		
		String owner_name = "";
		String hamster_name = "heinz";
		
		try {
			hmstr.lookup(owner_name, hamster_name);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_NotFound e) {
			assertTrue(e instanceof HamsterRPCException_NotFound);
		} catch (HamsterRPCException_StorageError e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		}
	}
	
	@Test
	public void lookup_empty_hamster() {
		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat"));
		
		String owner_name = "otto";
		String hamster_name = "";
		
		try {
			hmstr.lookup(owner_name, hamster_name);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_NotFound e) {
			assertTrue(e instanceof HamsterRPCException_NotFound);
		} catch (HamsterRPCException_StorageError e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		}
	}
	
	@Test
	public void lookup_empty_string() {
		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat"));
		
		String owner_name = "";
		String hamster_name = "";
		
		try {
			hmstr.lookup(owner_name, hamster_name);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_NotFound e) {
			assertTrue(e instanceof HamsterRPCException_NotFound);
		} catch (HamsterRPCException_StorageError e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		}
	}
	
	@Test
	public void lookup_td3() {
		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td3.dat"));
		
		String owner_name = "diesnameee123456789012345678901";
		String hamster_name = "langerName";
		
		try {
			hmstr.lookup(owner_name, hamster_name);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_NotFound e) {
			assertTrue(e instanceof HamsterRPCException_NotFound);
		} catch (HamsterRPCException_StorageError e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		}
	}
	
	@Test
	public void lookup_td4() {
		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td4.dat"));
		
		String owner_name = "diesnameee123456789012345678901";
		String hamster_name = "diesnameee123456789012345678901";
		
		try {
			hmstr.lookup(owner_name, hamster_name);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_NotFound e) {
			assertTrue(e instanceof HamsterRPCException_NotFound);
		} catch (HamsterRPCException_StorageError e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to register new hamster: " + e.getMessage());
		}
	}
	
//	@Test
//	public void lookup_owner_name_to_long() {
//		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat"));
//		
//		String owner_name = "abcdefgihjklmnopqrstuvwxyz1234567890";
//		String hamster_name = "heinz";
//		
//		try {
//			hmstr.lookup(owner_name, hamster_name);
//		} catch (HamsterRPCException_NameTooLong e) {
//			assertTrue(e instanceof HamsterRPCException_NameTooLong);
//		} catch (HamsterRPCException_NotFound e) {
//			e.printStackTrace();
//			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " expected HamsterRPCException_NameTooLong");
//		} catch (HamsterRPCException_StorageError e) {
//			e.printStackTrace();
//			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " expected HamsterRPCException_NameTooLong");
//		} catch (HamsterRPCException_DatabaseCorrupt e) {
//			e.printStackTrace();
//			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " expected HamsterRPCException_NameTooLong");
//		} catch (IOException e) {
//			e.printStackTrace();
//			fail("Failed to register new hamster: " + e.getMessage());
//		} catch (HamsterRPCException e) {
//			e.printStackTrace();
//			fail("Failed to register new hamster: " + e.getMessage());
//		}
//	}
//	
//	@Test
//	public void lookup_hamster_name_to_long() {
//		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat"));
//		
//		String owner_name = "otto";
//		String hamster_name = "abcdefgihjklmnopqrstuvwxyz1234567890";
//		
//		try {
//			hmstr.lookup(owner_name, hamster_name);
//		} catch (HamsterRPCException_NameTooLong e) {
//			assertTrue(e instanceof HamsterRPCException_NameTooLong);
//		} catch (HamsterRPCException_NotFound e) {
//			e.printStackTrace();
//			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " expected HamsterRPCException_NameTooLong");
//		} catch (HamsterRPCException_StorageError e) {
//			e.printStackTrace();
//			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " expected HamsterRPCException_NameTooLong");
//		} catch (HamsterRPCException_DatabaseCorrupt e) {
//			e.printStackTrace();
//			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " expected HamsterRPCException_NameTooLong");
//		} catch (IOException e) {
//			e.printStackTrace();
//			fail("Failed to register new hamster: " + e.getMessage());
//		} catch (HamsterRPCException e) {
//			e.printStackTrace();
//			fail("Failed to register new hamster: " + e.getMessage());
//		}
//	}

}
