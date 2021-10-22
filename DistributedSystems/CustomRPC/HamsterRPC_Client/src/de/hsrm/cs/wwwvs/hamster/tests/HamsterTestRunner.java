package de.hsrm.cs.wwwvs.hamster.tests;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;

import de.hsrm.cs.wwwvs.hamster.tests.suite.HamsterTestSuite;

public class HamsterTestRunner {

	private static void rtfm() {
		System.out.println("Usage:");
		System.out.println("java -jar HamsterTestRunner.jar {option}");
		System.out.println("\t\t-P [pathToHamsterServer]\t\t Path to the hamster_server executable. Default: " + HamsterTestDataStore.getInstance().getPathToSUT());
		System.out.println("\t\t-H [pathToHamsterProgram]\t\t Path to the hamster executable. Default: " + HamsterTestDataStore.getInstance().getPathToHamsterExe());
		System.out.println("\t\t-N [\"Examier name\"]\t\t Name of the examier, inserted in test result. Default: Bernd");
		System.out.println("\t\t-T [timount for testcases]\t\t Timeout for testcases im ms, Default: " + HamsterTestDataStore.getInstance().testcaseTimeoutms);
		System.out.println("\t\t-h \t\t\t This help text");
	}
	
	/**
	 * 
	 * 
	 * 
	 * @param args
	 * @throws UnsupportedEncodingException 
	 * @throws FileNotFoundException 
	 */
	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
		
		String examinerName = "Bernd";
		
		for (int i = 0; i < args.length; i++) {
			if (args[i].trim().matches("-h")) {
				rtfm();
				return;
			}
			
			if (args[i].trim().matches("-P")) {
				if (args.length < 2) {
					rtfm();
					return;
				}
				String path = args[i+1];
				// validate path?
				HamsterTestDataStore.getInstance().setPathToSUT(path);
			}
			if (args[i].trim().matches("-H")) {
				if (args.length < 2) {
					rtfm();
					return;
				}
				String path = args[i+1];
				// validate path?
				HamsterTestDataStore.getInstance().setPathToHamsterExe(path);
			}
			if (args[i].trim().matches("-N")) {
				examinerName = args[i+1];
			}
			if (args[i].trim().matches("-T")) {
				int timeout = Integer.parseInt(args[i+1]);
				HamsterTestDataStore.getInstance().testcaseTimeoutms = timeout;
				
			}
			
			
		}
		
		
	//	System.out.println("Starting in working dir: " + System.getProperty("user.dir"));
		
		
		Result result = JUnitCore.runClasses(HamsterTestSuite.class);
		
		
		
		PrintWriter writer = new PrintWriter(System.out);
	
		writer.println("Test summery: ");
		writer.println("\t run " + result.getRunCount() + " test cases");
		writer.println("\t test failed " + result.getFailureCount());
		writer.println("\t execution time " + result.getRunTime()/1000 + "s");
		writer.println("Failure list: ");
		for (Failure failure : result.getFailures()) {
			
			if (currentTestSuite == null) {
				// init
				currentTestSuite = failure.getDescription().getClassName();
				writer.println();
			}
			
			// now there is another test suite
			if (failure.getDescription().getClassName().equals(currentTestSuite) == false) {
				// sum result
				int pointDeduction = Math.min(failurePointsTestSuite, 3);
				writer.println("TestSuite " + currentTestSuite + ":");
				writer.println("   Fehlgeschlagene Testfälle: " + failedTestCases4TestSuite);
			//	writer.println("   Punktabzüge:               " + pointDeduction);
				
				failurePointsSummery += pointDeduction;
							
							
				// next testsuite
				failurePointsTestSuite = 0;
				failedTestCases4TestSuite = 0;
							
				currentTestSuite = failure.getDescription().getClassName();
			}
			failurePointsTestSuite++;
			failedTestCases4TestSuite++;
			
		}
		
		if (failurePointsTestSuite > 0 ) {
			int pointDeduction = Math.min(failurePointsTestSuite, 3);
			writer.println("TestSuite:    " + currentTestSuite );
			writer.println("   Fehlgeschlagene Testfälle: " + failedTestCases4TestSuite);
		//	writer.println("   Punktabzüge:               " + pointDeduction);
			
			failurePointsSummery += pointDeduction;
		}
		
		writer.println();
		

		writer.println();
		writer.println();
		writer.println();
		
		writer.println("Fehlgeschlagene Testfälle, detailliert: ");
		writer.println("==============================================");
		writer.println();
	
		
		for (Failure failure : result.getFailures()) {
			
			String msg = new String();
			msg += failure.getTestHeader();
			
			if (failure.getMessage() != null) {
				msg += ": ";
				msg += failure.getMessage();
			} else {
				msg += " - Aufgetretende Exception - Trace: \n";
				msg += failure.getTrace();
			}
			
			writer.println(msg);
			
		}
		
		writer.close();
		
		
	}

}
