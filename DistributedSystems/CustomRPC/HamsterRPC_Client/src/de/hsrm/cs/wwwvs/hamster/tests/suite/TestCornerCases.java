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
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_NotFound;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_StorageError;
import de.hsrm.cs.wwwvs.hamster.rpc.Hmstr.HamsterInteger;
import de.hsrm.cs.wwwvs.hamster.rpc.Hmstr.HamsterString;
import de.hsrm.cs.wwwvs.hamster.rpc.Hmstr.State;
import de.hsrm.cs.wwwvs.hamster.rpc.client.HamsterRPCConnection;
import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

public class TestCornerCases {

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

		HamsterTestDataStore.sleepMid();
		
		assertFalse("Server process is not shuting down.", sut.isAlive());
	}

	@Before
	public void setUp() {

		assertTrue("Server process is not running.", sut.isAlive());

		try {
			hmstr = new HamsterRPCConnection(hostname, port);
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
	public void cornerCase_new_hamster() {
		String owner_name = "otto";
		String hamster_name = "heinz";
		int treats = 23;

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
			fail("Error in response: " + e.getMessage());
		}
	}

	@Test
	public void cornerCase_lookup_td1() {
		assertTrue("Failed to setup test.", HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat"));

		String owner_name = "otto";
		String hamster_name = "heinz";
		int expectedUUID = 1996485908;

		int UUID = -1;

		try {
			UUID = hmstr.lookup(owner_name, hamster_name);

			assertTrue("lookup returned the UUID " + UUID + ", but " + expectedUUID + " was expected.",
					expectedUUID == UUID);

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
			fail("Error in response: " + e.getMessage());
		}
	}

	@Test
	public void cornerCase_howsdoing_td1() {
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}

		int uuid = 1996485908;

		State state = new State();
		State expectedState = new State();
		expectedState.treatsLeft = 23;
		expectedState.cost = 18;

		try {
			int returnCode = hmstr.howsdoing(uuid, state);
			assertTrue("returnCode should be 0.", returnCode == 0);
			assertTrue("treatsLeft expected " + expectedState.treatsLeft + ", received " + state.treatsLeft,
					state.treatsLeft == expectedState.treatsLeft);
			assertTrue("cost expected " + expectedState.cost + ", received " + state.cost,
					state.cost == expectedState.cost);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Error in response: " + e.getMessage());
		}
	}

	@Test
	public void cornerCase_testGive5Treats() {
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");

		int expectedUUID = 1996485908;

		try {
			int left = hmstr.givetreats(expectedUUID, 5);

			boolean ok = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td9.dat");

			assertTrue("After giveTreats of 5, the hamsterfile.dat is not as expeced", ok);

			assertSame(18, left);

		} catch (HamsterRPCException_NotFound e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Error in response: " + e.getMessage());
		}
	}

	@Test
	public void cornerCase_testHeinz() {
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
			return;
		}

		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
		}

		int expectedUUID = 1996485908;

		HamsterString owner = new HamsterString();
		HamsterString name = new HamsterString();
		HamsterInteger price = new HamsterInteger();

		try {
			int left = hmstr.readentry(expectedUUID, owner, name, price);

			assertEquals(23, left);
			assertEquals("otto", owner.str);
			assertEquals("heinz", name.str);
			assertEquals(17, price.i);

		} catch (HamsterRPCException_NotFound e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Error in response: " + e.getMessage());
		}
	}

	@Test
	public void cornerCase_testCollectOneHamster() {
		// HamsterTestDataStore.getInstance().copyTestHamsterfile("td2.dat");
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}

		try {
			int price = hmstr.collect("otto");

			assertSame(17, price);

		} catch (HamsterRPCException_NotFound e) {
			fail("Owner otto not found");
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Error in response: " + e.getMessage());
		}
	}

}
