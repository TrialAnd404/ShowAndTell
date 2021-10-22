package de.hsrm.cs.wwwvs.hamster.tests.suite;

import static org.junit.Assert.*;

import java.io.IOException;
import java.net.Inet4Address;
import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Enumeration;
import java.util.List;
import java.util.concurrent.TimeUnit;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.Timeout;

import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

public class TestConnection {
	
	private Process sut = null;
	
	private String sutPath = HamsterTestDataStore.getInstance().getSUT();
	
	static int port = HamsterTestDataStore.getInstance().getPort();
	
	@Rule
	public Timeout globalTimeout= new Timeout(HamsterTestDataStore.getInstance().testcaseTimeoutms, TimeUnit.MILLISECONDS);

	@Before
	public void setUp() throws Exception {
		HamsterTestDataStore.getInstance().sleepMin();
	}

	@After
	public void tearDown() throws Exception {
		sut.destroy();
		HamsterTestDataStore.getInstance().sleepMid();
	}

	@Test
	public void testDefaultConnection() {
		try {
			this.sut = Runtime.getRuntime().exec(this.sutPath + " -p " + port);
		} catch (IOException e) {
			fail("SUT laesst sich nicht startetn");
		}
		HamsterTestDataStore.getInstance().sleepMid();
		
		try {
			// verbindung auf
			Socket sutSocket = new Socket("localhost", port);
			
			assertTrue(sutSocket.isConnected());
			
			// verbindung zu
			sutSocket.close();
			
			assertTrue(sutSocket.isClosed());
			
			
		} catch (UnknownHostException e) {
			fail("localhost ist unbekannt");
		} catch (IOException e) {
			fail("Connection zu localhost port "+ port +" nicht moeglich");
		}
		
		// programm beenden
		this.sut.destroy();
		
		HamsterTestDataStore.sleepMin();
		
		
		assertFalse(this.sut.isAlive());
		
		
	}
	
	@Test
	public void testOnlyLocalhost() {
		// default should be only localhost, not on other interfaces
		try {
			this.sut = Runtime.getRuntime().exec(this.sutPath + " -p " + port);
		} catch (IOException e) {
			fail("SUT laesst sich nicht startetn");
		}
		
		HamsterTestDataStore.getInstance().sleepMid();
		
		try {
	
			
			
			for (Enumeration<NetworkInterface> ifaces = NetworkInterface.getNetworkInterfaces(); ifaces.hasMoreElements();) {
				NetworkInterface iface = ifaces.nextElement();
				System.out.println("iface name: " + iface.getName() + " " + iface.getDisplayName());
				List<InterfaceAddress> iAddrs = iface.getInterfaceAddresses();
				for (InterfaceAddress iAddr : iAddrs) {
					if (iAddr.getAddress().isLoopbackAddress() == false) {
						try {
							Socket sutSocket = new Socket(iAddr.getAddress(), port);
							sutSocket.close();
							this.sut.destroy();
							fail("Can connect to non localhost IP addr " + iAddr.getAddress().getHostAddress());
							
						} catch (IOException e) {
							
						}
					}
				}
			}
			
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		// programm beenden
		this.sut.destroy();

		HamsterTestDataStore.sleepMin();


		assertFalse(this.sut.isAlive());
		
	}
	
	@Test
	public void testEthInterface() {
		
		try {
			int portIter = port;

			for (Enumeration<NetworkInterface> ifaces = NetworkInterface.getNetworkInterfaces(); ifaces.hasMoreElements();) {
				NetworkInterface iface = ifaces.nextElement();
			//	System.out.println("iface name: " + iface.getName() + " " + iface.getDisplayName());
				List<InterfaceAddress> iAddrs = iface.getInterfaceAddresses();
		
				for (InterfaceAddress iAddr : iAddrs) {
					if (iAddr.getAddress().isLoopbackAddress() == false && 
							iAddr.getAddress().isLinkLocalAddress() == false && 
							 (iAddr.getAddress() instanceof Inet4Address) == true ) {
						
						try {
							System.out.println("Starting Server on IP " + iAddr.getAddress().getHostAddress() + ":" + portIter);
							this.sut = Runtime.getRuntime().exec(this.sutPath + " -p " + portIter + " -h " + iAddr.getAddress().getHostAddress());
						} catch (IOException e) {
							
							fail("SUT laesst sich nicht startetn");
						}
						
						try {
							
							HamsterTestDataStore.getInstance().sleepMid();
							Socket sutSocket = new Socket(iAddr.getAddress(), portIter);
							
							sutSocket.close();
							
							
						} catch (IOException e) {
							this.sut.destroy();
							fail("Can not connect on addr " + iAddr.getAddress().getHostAddress() + ":" + portIter + " " + e.getMessage());
						}
						
						this.sut.destroy();
						HamsterTestDataStore.sleepMin();
						
					}
					portIter++;
				}
			}
		} catch (SocketException e) {
			e.printStackTrace();
		}
		
		// programm beenden
		this.sut.destroy();

		HamsterTestDataStore.sleepMin();


		assertFalse(this.sut.isAlive());
		
		
	}
	
	

}
