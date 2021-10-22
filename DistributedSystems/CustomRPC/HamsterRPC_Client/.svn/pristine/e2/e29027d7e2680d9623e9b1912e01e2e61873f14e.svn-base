package de.hsrm.cs.wwwvs.hamster.tests.suite;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.IOException;
import java.util.Random;
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

public class TestGiveTreats {

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

		HamsterTestDataStore.sleepMin();

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
	
	// testcase 1: before x after x-2
	@Test
	public void testGive5Treats() {
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
		int expectedUUID = 1996485908;
		
		try {
			int left = hamster.givetreats(expectedUUID, 5);
			
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
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		}
		
		
		
	}
	// testcase 2: id  = 0
	@Test
	public void testZeroID() {
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
		int expectedUUID = 0;
		
		boolean ok = false;
		try {
			int left = hamster.givetreats(expectedUUID, 5);
			
			
		} catch (HamsterRPCException_NotFound e) {
			// should be thrown
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		}
		try {
			ok = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td1.dat");
			assertTrue("After giveTreats of 0, the hamsterfile.dat is not as expeced", ok);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	// testcase 3: id = random
	@Test
	public void testRandomID() {
		
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
		int expectedUUID = new Random().nextInt();
		
		boolean ok = false;
		try {
			int left = hamster.givetreats(expectedUUID, 5);
			
			
		} catch (HamsterRPCException_NotFound e) {
			// should be thrown
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		}
		try {
			ok = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td1.dat");
			assertTrue("After giveTreats of 0, the hamsterfile.dat is not as expeced", ok);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	// testcase 4: treats = 0
	@Test
	public void testGiveZeroTreats() {
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
		int expectedUUID = 1996485908;
		
		try {
			int left = hamster.givetreats(expectedUUID, 0);
			
			boolean ok = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td1.dat");
			
			assertTrue("After giveTreats of 0, the hamsterfile.dat is not as expeced", ok);
			
			assertSame(23, left);
			
		} catch (HamsterRPCException_NotFound e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		}
	}
	// testcase 5: treats > x
	@Test
	public void testGiveMoreTreats() {
		
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
		int expectedUUID = 1996485908;
		
		try {
			int left = hamster.givetreats(expectedUUID, 50);
			
			boolean ok = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td10.dat");
			
			assertTrue("After giveTreats of 50, the hamsterfile.dat is not as expeced", ok);
			
			assertSame(0, left);
			
		} catch (HamsterRPCException_NotFound e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		}
	}
	// testcase 6: treats = UINT16_MAX
	@Test
	public void testGiveMaxTreats() {
		
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
		int expectedUUID = 1996485908;
		
		try {
			int left = hamster.givetreats(expectedUUID, 65535);
			
//			boolean ok = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td10.dat");
			
	//		assertTrue("After giveTreats of 50, the hamsterfile.dat is not as expeced", ok);
			
			assertSame(0, left);
			
		} catch (HamsterRPCException_NotFound e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_StorageError e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException_DatabaseCorrupt e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (IOException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		} catch (HamsterRPCException e) {
			fail("Unexpected Exception: " + e.getClass().getSimpleName());
		}
	}
	
}
