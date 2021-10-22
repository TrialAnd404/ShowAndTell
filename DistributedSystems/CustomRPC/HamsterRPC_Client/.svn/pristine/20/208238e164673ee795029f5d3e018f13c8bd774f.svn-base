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
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_NotFound;
import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException_StorageError;
import de.hsrm.cs.wwwvs.hamster.rpc.Hmstr.State;
import de.hsrm.cs.wwwvs.hamster.rpc.client.HamsterRPCConnection;
import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

public class TestHowsDoing {

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
	public void howsdoing_td1() {
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
			assertTrue("treatsLeft expected "+expectedState.treatsLeft+", received " + state.treatsLeft, state.treatsLeft == expectedState.treatsLeft);
			assertTrue("cost expected "+expectedState.cost+", received " + state.cost, state.cost == expectedState.cost);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		}
	}
	
	@Test
	public void howsdoing_not_found() {
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}
				
		int uuid = 1234567890;
		
		State state = new State();
		State expectedState = new State();
		expectedState.treatsLeft = 23;
		expectedState.cost = 18;
		
		try {
			int returnCode = hmstr.howsdoing(uuid, state);
			assertTrue("returnCode should be -1.", returnCode == -1);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		} catch (HamsterRPCException_NotFound e) {
			assertTrue(e instanceof HamsterRPCException_NotFound);
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Expected HamsterRPCException_NotFound.");
		}
	}
	
	@Test
	public void howsdoing_td2() {
		try {
			HamsterTestDataStore.getInstance().createTestdata2();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}
		
		int uuid = 1996485908;
		
		State state = new State();
		State expectedState = new State();
		expectedState.treatsLeft = 0;
		expectedState.cost = 23;
		
		try {
			int returnCode = hmstr.howsdoing(uuid, state);
			assertTrue("returnCode should be 0.", returnCode == 0);
			assertTrue("treatsLeft expected "+expectedState.treatsLeft+", received " + state.treatsLeft, state.treatsLeft == expectedState.treatsLeft);
			assertTrue("cost expected "+expectedState.cost+", received " + state.cost, state.cost == expectedState.cost);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		}
	}

	@Test
	public void howsdoing_td5() {
		try {
			HamsterTestDataStore.getInstance().createTestdata5();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}
		
		int uuid = 1996485908;
		
		State state = new State();
		State expectedState = new State();
		expectedState.treatsLeft = -1;  // workaround because of unsigned value not available in java (65535 => -1)
		expectedState.cost = 50;
		
		try {
			int returnCode = hmstr.howsdoing(uuid, state);
			assertTrue("returnCode should be 0.", returnCode == 0);
			assertTrue("treatsLeft expected "+expectedState.treatsLeft+", received " + state.treatsLeft, state.treatsLeft == expectedState.treatsLeft);
			assertTrue("cost expected "+expectedState.cost+", received " + state.cost, state.cost == expectedState.cost);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		}
	}
	
	// testcase 5: not additional payload after successful call
	@Test
	public void testNoAddPayloadAfterSuccCall() {
		
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
			return;
		}
		
		int uuid = 1996485908;
		
		State state = new State();
		State expectedState = new State();
		expectedState.treatsLeft = 23;
		expectedState.cost = 18;
		
		hmstr.setTestNoPayloadAfterMessage(true);
		
		try {
			int returnCode = hmstr.howsdoing(uuid, state);
			assertTrue("returnCode should be 0.", returnCode == 0);
		//	assertTrue("treatsLeft expected "+expectedState.treatsLeft+", received " + state.treatsLeft, state.treatsLeft == expectedState.treatsLeft);
		//	assertTrue("cost expected "+expectedState.cost+", received " + state.cost, state.cost == expectedState.cost);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		}
		
		try {
			int addByte = hmstr.receiveOneByte();
			assertTrue("Received payload after sucessfull rpc call", (addByte == -1));
		} catch (IOException e) {
			
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
	}
	// testcase 6: no payload after error message
	@Test
	public void testNoAddPayloadAfterErrorCall() {
		
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
			return;
		}
		
		int uuid = 232342;
		
		State state = new State();
		State expectedState = new State();
		expectedState.treatsLeft = 23;
		expectedState.cost = 18;
		
		hmstr.setTestNoPayloadAfterMessage(true);
		
		try {
			int returnCode = hmstr.howsdoing(uuid, state);
			fail("Expected HamsterRPCException_StorageError");
	
			} catch (HamsterRPCException_NotFound e) {
			//
			} catch (HamsterRPCException_StorageError e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (HamsterRPCException_DatabaseCorrupt e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (IOException e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (HamsterRPCException e) {
				//
			}
		
		try {
			int addByte = hmstr.receiveOneByte();
			assertTrue("Received payload after sucessfull rpc call", (addByte == -1));
		} catch (IOException e) {
			
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
	}
	
	// testcase 7: two calls in a row
	
	@Test
	public void testTwoCallsInARow() {
		
		try {
			HamsterTestDataStore.getInstance().createTestdata13();
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
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
			assertTrue("treatsLeft expected "+expectedState.treatsLeft+", received " + state.treatsLeft, state.treatsLeft == expectedState.treatsLeft);
			assertTrue("cost expected "+expectedState.cost+", received " + state.cost, state.cost == expectedState.cost);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		}
		
	
		
		State state2 = new State();
		State expectedState2 = new State();
		expectedState2.treatsLeft = 42;
		expectedState2.cost = 18;
		
		int uuid2 = 1988566364;
		
		
		try {
			int returnCode = hmstr.howsdoing(uuid2, state2);
			assertTrue("returnCode should be 0.", returnCode == 0);
			assertTrue("treatsLeft expected "+expectedState2.treatsLeft+", received " + state2.treatsLeft, state2.treatsLeft == expectedState2.treatsLeft);
			assertTrue("cost expected "+expectedState2.cost+", received " + state2.cost, state.cost == expectedState2.cost);
		} catch (IOException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		} catch (HamsterRPCException e) {
			e.printStackTrace();
			fail("Failed to check hamster state.");
		}
		
	}
	
	
	
	
}
