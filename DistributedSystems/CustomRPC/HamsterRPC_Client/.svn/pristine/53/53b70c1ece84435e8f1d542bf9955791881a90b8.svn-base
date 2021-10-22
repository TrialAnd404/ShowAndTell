package de.hsrm.cs.wwwvs.hamster.tests.suite;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.concurrent.TimeUnit;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.Timeout;

import de.hsrm.cs.wwwvs.hamster.rpc.HamsterRPCException;
import de.hsrm.cs.wwwvs.hamster.rpc.client.HamsterRPCConnection;
import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

public class TestHelper {
	
	private String sutPath = HamsterTestDataStore.getInstance().getSUT();
	private Process sut = null;
	
	@Rule
	public Timeout globalTimeout= new Timeout(HamsterTestDataStore.getInstance().testcaseTimeoutms, TimeUnit.MILLISECONDS);

	@Before
	public void setUp() throws Exception {
	//	HamsterTestDataStore.getInstance().wipeHamsterfile();
	}

	@After
	public void tearDown() throws Exception {
	//	HamsterTestDataStore.getInstance().wipeHamsterfile();
		
		if (sut != null) {
			sut.destroy();
		}
	}
	
	@Test
	public void testHamsterfile() {
		
		
		String userDir = System.getProperty("user.dir");
		System.out.println(userDir);
		boolean t;
		t = HamsterTestDataStore.getInstance().copyTestHamsterfile("td1.dat");
		assertTrue(t);
		try {
			t= HamsterTestDataStore.getInstance().compareHamsterFileEqual("td1.dat");
			assertTrue(t);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			fail(e.getMessage());
		}
		
		HamsterTestDataStore.getInstance().wipeHamsterfile();
		
	}
	
	@Test
	public void testHamsterStart() {
		
		try {
			this.sut = Runtime.getRuntime().exec(this.sutPath + " -p 8088");
		} catch (IOException e) {
			fail("SUT laesst sich nicht startetn");
		}
		
		HamsterRPCConnection hamster = null;
		try {
			hamster = new HamsterRPCConnection("localhost", 8088);
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		try {
			hamster.new_("otto", "heinz", 23);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (HamsterRPCException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try {
			boolean ok = HamsterTestDataStore.getInstance().compareHamsterFileEqual("td1.dat");
			assertTrue(ok);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}
}
