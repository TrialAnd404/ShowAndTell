/**
 * 
 */
package de.hsrm.cs.wwwvs.hamster.tests.suite;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.junit.runners.Suite;
import org.junit.runners.Suite.SuiteClasses;

import de.hsrm.cs.wwwvs.hamster.tests.HamsterTestDataStore;

@RunWith(Suite.class)
@SuiteClasses({ TestCollect.class,
				TestConnection.class, 
				TestCornerCases.class,
				TestDirectory.class,
				TestGiveTreats.class,
				TestHowsDoing.class,
				TestLookup.class,
				TestMultipleClients.class,
				TestNew.class, 
				TestReadEntry.class, 
				 
				})

/**
 * @author Olga Dedi
 *
 */
public class HamsterTestSuite {

	/**
	 * @throws java.lang.Exception
	 */
	@BeforeClass
	public static void setUpBeforeClass() throws Exception {
		HamsterTestDataStore.getInstance().setPort(9000);
		
		System.out.println("user.dir: " + System.getProperty("user.dir"));
		
		String hamster = System.getProperty("hamsterPath");
		String server = System.getProperty("serverPath");
		
		if (hamster != null) {
			HamsterTestDataStore.getInstance().setPathToHamsterExe(hamster);
		}
		if (server != null) {
			HamsterTestDataStore.getInstance().setPathToSUT(server);
			
		}
		
		
	}

	/**
	 * @throws java.lang.Exception
	 */
	@AfterClass
	public static void tearDownAfterClass() throws Exception {
	}

}
