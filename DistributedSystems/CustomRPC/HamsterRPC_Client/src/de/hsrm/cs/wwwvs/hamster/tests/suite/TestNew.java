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
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_Extists;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_NameTooLong;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_StorageError;
import de.hsrm.cs.wwwvs.hamster.rpc.client.HamsterRPCConnection;
import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

public class TestNew {

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
		HamsterTestDataStore.sleepMin();
		assertTrue("Server process is not running.", sut.isAlive());

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
	public void new_hamster() {
		String owner_name 	= "otto";
		String hamster_name = "heinz";
		int treats 			= 23;
		
		int returnCode = -1;
		
		try {
			returnCode = hmstr.new_(owner_name, hamster_name, treats);
			assertTrue("UUID must be greater or equal to 0.", returnCode >= 0);
			
			Boolean result = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td1.dat");
			assertTrue("Content of hamsterfile.dat is not as expected.", result);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_Extists e) {
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
	public void new_duplicate() {
		String owner_name 	= "otto";
		String hamster_name = "heinz";
		int treats 			= 23;
		
		int returnCode = -1;
		
		try {
			returnCode = hmstr.new_(owner_name, hamster_name, treats);
			assertTrue("UUID must be greater or equal to 0.", returnCode >= 0);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_Extists e) {
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
		
		// insert duplicate
		returnCode = -1;
		
		try {
			returnCode = hmstr.new_(owner_name, hamster_name, treats);
			assertTrue("UUID must be less then 0.", returnCode < 0);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_Extists e) {
			assertTrue(e instanceof HamsterRPCException_Extists);
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
		
		Boolean result = false;
		try {
			result = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td1.dat");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		assertTrue("Content of hamsterfile.dat is not as expected.", result);
	}

	@Test
	public void new_max_owner_name() {
		String owner_name 	= "diesnameee123456789012345678901";
		String hamster_name = "langerName";
		int treats 			= 0;
		
		int returnCode = -1;
		
		try {
			returnCode = hmstr.new_(owner_name, hamster_name, treats);
			assertTrue("UUID must be greater or equal to 0.", returnCode >= 0);
			
			Boolean result = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td3.dat");
			assertTrue("Content of hamsterfile.dat is not as expected.", result);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_Extists e) {
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
	public void new_max_hamster_name() {
		String owner_name 	= "diesnameee123456789012345678901";
		String hamster_name = "diesnameee123456789012345678902";
		int treats 			= 0;
		
		int returnCode = -1;
		
		try {
			returnCode = hmstr.new_(owner_name, hamster_name, treats);
			assertTrue("UUID must be greater or equal to 0.", returnCode >= 0);
			
			Boolean result = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td4.dat");
			assertTrue("Content of hamsterfile.dat is not as expected.", result);
		} catch (HamsterRPCException_NameTooLong e) {
			e.printStackTrace();
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_Extists e) {
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
	public void new_too_log_owner_name() {
		String owner_name 	= "a_too_long_owner_name_with_more_than_31_chars";
		String hamster_name = "heinz";
		int treats 			= 0;
		
		int returnCode = -1;
		
		try {
			returnCode = hmstr.new_(owner_name, hamster_name, treats);
			assertTrue("UUID must be greater or equal to 0.", returnCode >= 0);
		} catch (HamsterRPCException_NameTooLong e) {
			assertTrue(e instanceof HamsterRPCException_NameTooLong);
		} catch (HamsterRPCException_Extists e) {
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
	public void new_too_log_hamster_name() {
		String owner_name 	= "otto";
		String hamster_name = "a_too_long_hamster_name_with_more_than_31_chars";
		int treats 			= 0;
		
		int returnCode = -1;
		
		try {
			returnCode = hmstr.new_(owner_name, hamster_name, treats);
			assertTrue("UUID must be greater or equal to 0.", returnCode >= 0);
		} catch (HamsterRPCException_NameTooLong e) {
			assertTrue(e instanceof HamsterRPCException_NameTooLong);
		} catch (HamsterRPCException_Extists e) {
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
}
