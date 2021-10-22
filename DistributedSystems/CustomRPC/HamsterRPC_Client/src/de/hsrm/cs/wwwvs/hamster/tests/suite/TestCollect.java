package de.hsrm.cs.wwwvs.hamster.tests.suite;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.IOException;
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
import de.hsrm.cs.wwwvs.hamster.rpc.client.HamsterRPCConnection;
import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

public class TestCollect {
	
	
	private static Process sut = null;
	
	private static int port = HamsterTestDataStore.getInstance().getPort();
	private static String sutPath = HamsterTestDataStore.getInstance().getSUT();
	
	private HamsterRPCConnection hamster = null;
	
	@Rule
	  public Timeout globalTimeout= new Timeout(HamsterTestDataStore.getInstance().testcaseTimeoutms, TimeUnit.MILLISECONDS);
	
	@BeforeClass
	public static void setUpBeforeClass() {
		
		try {
			sut = Runtime.getRuntime().exec(sutPath + " -p " + port);
		} catch (IOException e) {
			fail("SUT laesst sich nicht startetn");
		}
		
		assertTrue("Server process is not running.", sut.isAlive());
		
		HamsterTestDataStore.sleepMin();
	}

	@AfterClass
	public static void tearDownAfterClass() {
		
		if (sut != null) {
			sut.destroy();
		}

	
		HamsterTestDataStore.sleepMid();
		

		assertFalse("Server process is not shuting down.", sut.isAlive());
	}
	
	
	@Before
	public void setUp() throws Exception {
		
		HamsterTestDataStore.getInstance().wipeHamsterfile();
		
		hamster = new HamsterRPCConnection("localhost", port, true);
		
		HamsterTestDataStore.sleepMin();
	}

	@After
	public void tearDown() throws Exception {
		
		hamster.close();
		
		HamsterTestDataStore.getInstance().wipeHamsterfile();
		
		HamsterTestDataStore.sleepMin();
	}
	
	
	// testcase 1: collect one hamster
	@Test
	public void testCollectOneHamster() {
	//	HamsterTestDataStore.getInstance().copyTestHamsterfile("td2.dat");
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}
		
		try {
			int price = hamster.collect("otto");
			
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
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
		
	}
	// testcase 2: collect two hamster
	@Test
	public void testCollectTwoHamster() {
		
		try {
			HamsterTestDataStore.getInstance().createTestdata8();
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
			return;
		}
		
		int price;
		try {
			price = hamster.collect("otto");
			assertSame(34, price);
		} catch (HamsterRPCException_NotFound e) {
			fail("Owner otto not found");
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
		
		
		
	}
	// testcase 3: collect not existing owner
	@Test
	public void testCollectNoExtOwner() {
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
		

		int price;
		try {
			price = hamster.collect("karl");
			fail("Expected HamsterRPCException_NotFound" );
		} catch (HamsterRPCException_NotFound e) {
			//assert
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
	}
	// testcase 4: collect no database
	@Test
	public void testCollectNoDatabase() {
		
		int price;
		try {
			price = hamster.collect("karl");
			fail("Expected HamsterRPCException_StorageError" );
		} catch (HamsterRPCException_NotFound e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_StorageError e) {
			// should be thrown
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
	}
	// testcase 5: collect null name
	@Test
	public void testCollectNullOwner() {
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
		int price;
		try {
			price = hamster.collect("");
			fail("Expected HamsterRPCException_NotFound" );
		} catch (HamsterRPCException_NotFound e) {
			//fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
	}

	// testcase 6:no additional payload after one successfull call
	@Test
	public void testNoPayloadAfterCollectOneHamster() {
	//	HamsterTestDataStore.getInstance().copyTestHamsterfile("td2.dat");
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}
		
		hamster.setTestNoPayloadAfterMessage(true);
		
		try {
			int price = hamster.collect("otto");
			
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
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
		try {
			int addByte = hamster.receiveOneByte();
			assertTrue("Received payload after sucessfull rpc call", (addByte == -1));
		} catch (IOException e) {
			
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
	}
	
	// testcase 7: no additional payload after one failed call 
	
	@Test
	public void testNoPayloadAfterError() {
		try {
			HamsterTestDataStore.getInstance().createTestdata1();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}
		
		hamster.setTestNoPayloadAfterMessage(true);
		
		try {
			int price = hamster.collect("ichextistierenicht");
			
			//fail("Expected HamsterRPCException_NotFound");
			
		} catch (HamsterRPCException_NotFound e) {
			// expected
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
		try {
			int addByte = hamster.receiveOneByte();
			assertTrue("Received payload after error rpc call", (addByte == -1));
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
	}
	
	// testcase 8: two calls in a row
	
	@Test
	public void testTwoCallsInARow() {
		
		try {
			HamsterTestDataStore.getInstance().createTestdata13();
		} catch (IOException e1) {
			fail("Unexpected Exception: " + e1.getClass().getSimpleName() + " msg " + e1.getMessage());
			return;
		}
		
		try {
			int price = hamster.collect("otto");
			
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
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
		try {
			int price = hamster.collect("bernd");
			
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
			fail("Unexpected Exception: " + e.getClass().getSimpleName() + " msg " + e.getMessage());
		}
		
		
		
	}

	
}
