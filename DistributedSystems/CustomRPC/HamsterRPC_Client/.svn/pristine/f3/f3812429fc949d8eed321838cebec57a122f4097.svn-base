package de.hsrm.cs.wwwvs.hamster.tests;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.Arrays;

public class HamsterTestDataStore {

	// /home/olga/VS/hg_repos/VS/Praktikum/Material/
	// /home/kai/hs-rm/kaiser-hg/VS/Praktikum/Material/
	//private String pathToSUT = "/home/kai/hs-rm/kaiser-hg/VS/Praktikum/Material/HamsterRPC";
	//private String pathToHamsterExe = "/home/kai/hs-rm/kaiser-hg/VS/Praktikum/Material/Hamster";
	private String pathToHamsterExe = "./tests/";
	private String pathToSUT = "./";
	private String SUTName = "hamster_server";
	private String pathToHamsterFile = System.getProperty("user.dir");
	private String hamsterFileName = "hamsterfile.dat";
	private String testdatenPath = "testdaten/";

	private int port = 8088;
	
	private  static int sleepMin = 200;
	private  static int sleepMed = 400;
	private  static int sleepMax = 1000;
	
	public int testcaseTimeoutms = 2000;

	/*
	 * 
	 * Testdaten
	 * 			besitzer	hamster		treats	price	revol
	 * td1:		otto		heinz		23		0
	 * 
	 * td2:		otto		heinz		0		23
	 * 
	 * td3:		diesnameee123456789012345678901		langerName	0		0		
	 * 
	 * td4:		diesnameee123456789012345678901		diesnameee123456789012345678902		0		0
	 * 
	 * td5 		otto		heinz		65535	0
	 * 
	 * td6		otto{1..50} heinz{1..50} {1..50} 0
	 * 
	 * td7		otto 		heinz 		23
	 * 			karl 		blondy 		42
	 * 
	 * td8		otto		heinz 		23
	 * 			otto		blondy 		42
	 * 
	 * td9		otto		heinz 		18
	 * 
	 * td10		otto		heinz		0 (after giving 50)
	 * 
	 * td11		otto		heinz		0		65535
	 * 
	 * td12		otto 		blondy 23
	 *			hans 		blondy 23
     * 			ernst 		foo 23
     * 
     * td13		otto		heinz		23
     * 			bernd		blondy		42
	 */
	
	
	public static void sleepMin() {
		try {
			Thread.sleep(sleepMin);
		} catch (InterruptedException e) {
			
		}
	}
	public static void sleepMid() {
		try {
			Thread.sleep(sleepMed);
		} catch (InterruptedException e) {
			
		}
	}
	public static void sleepMax() {
		try {
			Thread.sleep(sleepMax);
		} catch (InterruptedException e) {
			
		}
	}
	
	/**
	 * deletes the hamsterfile.dat
	 * @return
	 */
	public boolean wipeHamsterfile() {
		
		Path path = Paths.get(this.pathToHamsterFile, this.hamsterFileName);
		
		try {
			Files.deleteIfExists(path);
		} catch (IOException e) {
			return false;
		}

		return true;
	}
	
	public boolean compareHamsterFileEqual(String hamsterTestFileName) throws IOException {
		
		String userDir = System.getProperty("user.dir");
		Path testFilePath = Paths.get(userDir, this.testdatenPath, hamsterTestFileName);
		
		
		byte[] sutFile = Files.readAllBytes(Paths.get(this.pathToHamsterFile, this.hamsterFileName));
		byte[] testFile = Files.readAllBytes(testFilePath);
		
		int countByteSUT = sutFile.length;
		int countByteTest = testFile.length;
		
		if (countByteSUT != countByteTest) {
			System.out.println("ungleiche groesse");
			return false;
		}

		// System.out.println("count b " +countByteSUT + " t " + countByteTest);
		// 6 * 8 * 2 = 96 byte

		int countDataSets = countByteSUT / 96;

		for (int i = 0; i < countDataSets; i++) {
			// offset in file
			int offset = i*96;
			// first int32
			if (Arrays.equals(Arrays.copyOfRange(sutFile, 0+offset, 4+offset), Arrays.copyOfRange(testFile, 0+offset, 4+offset)) == false) {
				System.out.println("uuid unterschiedlich");
				return false;
			}
			
			// without timeval
			
			if (Arrays.equals(Arrays.copyOfRange(sutFile, 20+offset, 95+offset), Arrays.copyOfRange(testFile, 20+offset, 95+offset)) == false) {
				System.out.println("rest unterschiedlich");
				return false;
			}
		}
		
		
		
		
		//return Arrays.equals(sutFile, testFile);
		
		return true;
		
	}
	public boolean copyTestHamsterfile(String hamsterTestFileName) {
		
	//	URL urlTestfile = HamsterTestDataStore.class.getClassLoader().getResource(this.testdatenPath + "/" + hamsterTestFileName);
		
		String userDir = System.getProperty("user.dir");
	//	HamsterTestDataStore.class.getClassLoader().
		
		
		Path srcFile = Paths.get(userDir, this.testdatenPath, hamsterTestFileName);
		
	//	URI uriDestFolder = URI.create();
		
		Path destFile = Paths.get(this.pathToHamsterFile, hamsterFileName);
		
		try {
			Files.copy(srcFile, destFile, StandardCopyOption.REPLACE_EXISTING);
		} catch (IOException e) {
			System.out.println("error " + e.getMessage() + " " );
			return false;
		}

		return true;
	}

	public void createTestdata1() throws IOException {

		Process sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -n otto heinz 23");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {

			}
		}
	}

	public void createTestdata2() throws IOException {

		Process sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -n otto heinz 0");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {

			}
		}
		for (int i = 18; i < 23; i++) {
			sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -s otto heinz");
			while (sut.isAlive()) {
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {

				}
			}
		}
	}

	public void createTestdata5() throws IOException {

		Process sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -n otto heinz 65535");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {

			}
		}
		for (int i = 18; i < 50; i++) {
			sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -s otto heinz");
			while (sut.isAlive()) {
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {

				}
			}
		}
	}
	
public void createTestdata11( ) throws IOException {
		
		Process sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -n otto heinz 0");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				
			}
		}
		sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -f otto heinz 16374");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				
			}
		}
	}
	
	
	
	public void createTestdata8( ) throws IOException {
		
		Process sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -n otto heinz 23");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				
			}
		}
		sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -n otto blondy 42");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				
			}
		}

	}
	
	public void createTestdata13() throws IOException {
		Process sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -n otto heinz 23");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				
			}
		}
		sut = Runtime.getRuntime().exec(this.pathToHamsterExe + "/hamster" + " -n bernd blondy 42");
		while (sut.isAlive()) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				
			}
		}
		
	}
	
	public String getSUT() {
		return this.pathToSUT + "/" + this.SUTName;
	}
	public String getPathToSUT( ) {
		
		return this.pathToSUT;
	}
	
	public String getPathToHamsterFile() {
		
		return this.pathToHamsterFile;
	}

	public String getSUTName() {
		return SUTName;
	}

	public void setPathToSUT(String p) {
		this.pathToSUT = p;
	}

	public void setPathToHamsterFile(String p) {
		this.pathToHamsterFile = p;
	}
	
	public void setPathToHamsterExe(String p) {
		this.pathToHamsterExe = p;
	}
	public String getPathToHamsterExe() {
		return this.pathToHamsterExe;
	}

	public void setSUTName(String n) {
		this.SUTName = n;
	}

	private static HamsterTestDataStore inst = null;

	public static HamsterTestDataStore getInstance() {

		if (inst == null) {
			inst = new HamsterTestDataStore();
		}

		return inst;
	}

	private HamsterTestDataStore() {

	}

	public int getPort() {
		return port++;
	}

	public void setPort(int port) {
		this.port = port;
	}

}
