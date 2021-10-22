package de.hsrm.cs.wwwvs.hamster.tests.suite;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
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
import de.hsrm.cs.wwwvs.hamster.rpc.Hmstr.HamsterHandle;
import de.hsrm.cs.wwwvs.hamster.rpc.client.HamsterRPCConnection;
import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

public class TestDirectory {

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
			
			assertTrue("Server process is not running.", sut.isAlive());
		} catch (IOException e) {
			fail("SUT laesst sich nicht startetn");
		}
		
		HamsterTestDataStore.sleepMid();
		
		
	}

	@AfterClass
	public static void tearDownAfterClass() {
		
		if (sut != null) {
			sut.destroy();
			
			HamsterTestDataStore.sleepMin();

			assertFalse("Server process is not shuting down.", sut.isAlive());
		}

		
		
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
	
	// testcase 1: einmal alle (1x heinz)
	@Test
	public void testAllHamster1() {
	
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
	
		
		int expectedUUID = 1996485908;
		
		HamsterHandle fdptr = new HamsterHandle();
		
		try {
			
			fdptr.fdptr = -1;
			
			int uuid = hamster.directory(fdptr, null, null);
			
			assertEquals(expectedUUID, uuid);
			assertNotEquals(-1, fdptr.fdptr);
			assertEquals(5, fdptr.fdptr);
			
			
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
		
		try {
			
			
			int uuid = hamster.directory(fdptr, null, null);
			
			fail("Expected HamsterRPCException_NotFound");
			
			
		} catch (HamsterRPCException_NotFound e) {
			
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
	// testcase 2: einmal alle (zwei Hamster)
	@Test
	public void testAllHamster2() {
	
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td7.dat");
	
		
		int expectedUUID = 1996485908;
		
		HamsterHandle fdptr = new HamsterHandle();
		try {
			
			fdptr.fdptr = -1;
			
			int uuid = hamster.directory(fdptr, null, null);
			
			assertEquals(expectedUUID, uuid);
			assertNotEquals(-1, fdptr.fdptr);
			assertEquals(5, fdptr.fdptr);
			
			
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
		
		try {
			
			int uuid = hamster.directory(fdptr, null, null);
			
			assertEquals(1450595035, uuid);
			assertNotEquals(-1, fdptr.fdptr);
			assertEquals(5, fdptr.fdptr);
			
			
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
		
		try {
			
			
			int uuid = hamster.directory(fdptr, null, null);
			
			fail("Expected HamsterRPCException_NotFound");
			
			
		} catch (HamsterRPCException_NotFound e) {
			
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
	// testcase 3: einmal alle (50 Hamster)
	@Test
	public void testAllHamster50() {
	
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td6.dat");
	
		int[] expectedUUID = new int[49];
		//int expectedUUID = 1996485908;
		expectedUUID[0] = 200384917;
		expectedUUID[1] = 908775142;
		expectedUUID[2] = 1617165367;
		expectedUUID[3] = 178071944;
		expectedUUID[4] = 886462169;
		expectedUUID[5] = 1594852394;
		expectedUUID[6] = 155758971;
		expectedUUID[7] = 864149196;
		expectedUUID[8] = 1572539421;
		expectedUUID[9] = 1425996485;
		expectedUUID[10] = 2033140214;
		expectedUUID[11] = 492800295;
		expectedUUID[12] = 1099944024;
		expectedUUID[13] = 1707087753;
		expectedUUID[14] = 166747834;
		expectedUUID[15] = 773891563;
		expectedUUID[16] = 1381035292;
		expectedUUID[17] = 1988179021;
		expectedUUID[18] = 447839102;
		expectedUUID[19] = 2134386710;
		expectedUUID[20] = 594046791;
		expectedUUID[21] = 1201190520;
		expectedUUID[22] = 1808334249;
		expectedUUID[23] = 267994330;
		expectedUUID[24] = 875138059;
		expectedUUID[25] = 1482281788;
		expectedUUID[26] = 2089425517;
		expectedUUID[27] = 549085598;
		expectedUUID[28] = 1156229327;
		expectedUUID[29] = 695293287;
		expectedUUID[30] = 1302437016;
		expectedUUID[31] = 1909580745;
		expectedUUID[32] = 369240826;
		expectedUUID[33] = 976384555;
		expectedUUID[34] = 1583528284;
		expectedUUID[35] = 43188365;
		expectedUUID[36] = 650332094;
		expectedUUID[37] = 1257475823;
		expectedUUID[38] = 1864619552;
		expectedUUID[39] = 1403683512;
		expectedUUID[40] = 2010827241;
		expectedUUID[41] = 470487322;
		expectedUUID[42] = 1077631051;
		expectedUUID[43] = 1684774780;
		expectedUUID[44] = 144434861;
		expectedUUID[45] = 751578590;
		expectedUUID[46] = 1358722319;
		expectedUUID[47] = 1965866048;
		expectedUUID[48] = 425526129;
		
		HamsterHandle fdptr = new HamsterHandle();
		try {
			
			fdptr.fdptr = -1;
			int count = 0;
			
			for (int uu : expectedUUID) {
				
				int uuid = hamster.directory(fdptr, null, null);
				
				assertEquals(uu, uuid);
				assertNotEquals(-1, fdptr.fdptr);
				assertEquals(5, fdptr.fdptr);
				expectedUUID[count] = uuid;
				count++;
			}
			
			assertEquals(49, count);
			
		
			
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
		
		try {
			
			int uuid = hamster.directory(fdptr, null, null);
			
			assertEquals(2112073737, uuid);
			assertNotEquals(-1, fdptr.fdptr);
			assertEquals(5, fdptr.fdptr);
			
			
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
		
		try {
			
			
			int uuid = hamster.directory(fdptr, null, null);
			
			fail("Expected HamsterRPCException_NotFound");
			
			
		} catch (HamsterRPCException_NotFound e) {
			
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
	// testcase 4: einmal alle von otto (1x)
	@Test
	public void testAllHamsterOtto() {
	
	
		HamsterTestDataStore.getInstance().copyTestHamsterfile("td7.dat");
	
	
		
		int expectedUUID = 1996485908;
		
		HamsterHandle fdptr = new HamsterHandle();
		try {
			
			fdptr.fdptr = -1;
			
			int uuid = hamster.directory(fdptr, "otto", null);
			
			assertEquals(expectedUUID, uuid);
			assertNotEquals(-1, fdptr.fdptr);
			assertEquals(5, fdptr.fdptr);
			
			
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
		
		
		try {
			
			
			int uuid = hamster.directory(fdptr, "otto", null);
			
			fail("Expected HamsterRPCException_NotFound");
			
			
		} catch (HamsterRPCException_NotFound e) {
			
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
	// testcase 5: einmal alle von otto (2x)
		@Test
		public void testAllHamsterOtto2() {
		
			try {
				HamsterTestDataStore.getInstance().createTestdata8();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		
			
			int expectedUUID = 1996485908;
			
			HamsterHandle fdptr = new HamsterHandle();
			try {
				
				fdptr.fdptr = -1;
				
				int uuid = hamster.directory(fdptr, "otto", null);
				
				assertEquals(expectedUUID, uuid);
				assertNotEquals(-1, fdptr.fdptr);
				assertEquals(5, fdptr.fdptr);
				
				
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
			
			try {
				
				int uuid = hamster.directory(fdptr, "otto", null);
				
				assertEquals(1573555497, uuid);
				assertNotEquals(-1, fdptr.fdptr);
				assertEquals(5, fdptr.fdptr);
				
				
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
			
			try {
				
				
				int uuid = hamster.directory(fdptr, null, null);
				
				fail("Expected HamsterRPCException_NotFound");
				
				
			} catch (HamsterRPCException_NotFound e) {
				
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
	// testcase 6: einmal alle von goldies (1x)
		@Test
		public void testAllHamsterBlondy() {
		
		
			try {
				HamsterTestDataStore.getInstance().createTestdata8();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				fail();
			}
		
		
			
			int expectedUUID = 1573555497;
			
			HamsterHandle fdptr = new HamsterHandle();
			try {
				
				fdptr.fdptr = -1;
				
				int uuid = hamster.directory(fdptr, null, "blondy");
				
				assertEquals(expectedUUID, uuid);
				assertNotEquals(-1, fdptr.fdptr);
				assertEquals(5, fdptr.fdptr);
				
				
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
			
			
			try {
				
				
				int uuid = hamster.directory(fdptr, "blondy", null);
				
				fail("Expected HamsterRPCException_NotFound");
				
				
			} catch (HamsterRPCException_NotFound e) {
				
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
	// testcase 7: einmal alle von goldies (2x)
		@Test
		public void testAllHamsterBlondy2() {
		
			assertTrue(HamsterTestDataStore.getInstance().copyTestHamsterfile("td12.dat"));
			
			int expectedUUID = 1573555497;
			
			HamsterHandle fdptr = new HamsterHandle();
			try {
				
				fdptr.fdptr = -1;
				
				int uuid = hamster.directory(fdptr, null, "blondy");
				
				assertEquals(expectedUUID, uuid);
				assertNotEquals(-1, fdptr.fdptr);
				assertEquals(5, fdptr.fdptr);
				
				
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
			
			try {
				
				int uuid = hamster.directory(fdptr, null, "blondy");
				
				assertEquals(421521419, uuid);
				assertNotEquals(-1, fdptr.fdptr);
				assertEquals(5, fdptr.fdptr);
				
				
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
			
			try {
				
				
				int uuid = hamster.directory(fdptr, null, "blondy");
				
				fail("Expected HamsterRPCException_NotFound");
				
				
			} catch (HamsterRPCException_NotFound e) {
				
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
	// testcase 8: falscher fdptr
		@Test
		public void testWrongFdptr() {
		
			HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
			
			int expectedUUID = 1996485908;
			
			HamsterHandle fdptr = new HamsterHandle();
			
			try {
				
				fdptr.fdptr = 3849;
				
				int uuid = hamster.directory(fdptr, null, null);
				
				fail("Expected HamsterRPCException_StorageError");
				
			} catch (HamsterRPCException_NotFound e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (HamsterRPCException_StorageError e) {
				
			} catch (HamsterRPCException_DatabaseCorrupt e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (IOException e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (HamsterRPCException e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			}
			
			
			
		}
	// testcase 9: nicht mit -1 begonnen
		@Test
		public void testNotStartedWithZero() {
		
			HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
			
			int expectedUUID = 1996485908;
			
			HamsterHandle fdptr = new HamsterHandle();
			
			try {
				
				fdptr.fdptr = 5;
				
				int uuid = hamster.directory(fdptr, null, null);
				
				fail("Expected HamsterRPCException_StorageError");
				
			} catch (HamsterRPCException_NotFound e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (HamsterRPCException_StorageError e) {
				
			} catch (HamsterRPCException_DatabaseCorrupt e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (IOException e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (HamsterRPCException e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			}
			
			
			
		}
	// testcase 1: keine datenbank
		@Test
		public void testNoDatabase() {
		
		//	HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		
			
			int expectedUUID = 1996485908;
			
			HamsterHandle fdptr = new HamsterHandle();
			
			try {
				
				fdptr.fdptr = 5;
				
				int uuid = hamster.directory(fdptr, null, null);
				
				fail("Expected HamsterRPCException_StorageError");
				
			} catch (HamsterRPCException_NotFound e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (HamsterRPCException_StorageError e) {
				
			} catch (HamsterRPCException_DatabaseCorrupt e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (IOException e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			} catch (HamsterRPCException e) {
				fail("Unexpected Exception: " + e.getClass().getSimpleName());
			}
			
			
			
		}
	
}
