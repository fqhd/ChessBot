#pragma once
#include <iostream>

int testLibTotal = 0;
int testLibPassed = 0;

void printTestOutput(const char* testName, bool failed) {
	if (failed) {
		std::cout << testName << ": \033[1;31mFail\033[0m" << std::endl;
	} else {
		std::cout << testName << ": \033[1;32mSuccess\033[0m" << std::endl;
	}
}

void test(const char* testName, bool(*testFunc)()) {
	bool failed = testFunc();
	testLibTotal += 1;
	if (!failed) testLibPassed++;
	printTestOutput(testName, failed);
}

void printResults() {
	std::cout << "Results: " << std::endl;
	std::cout << testLibPassed << " passed" << std::endl;
	std::cout << testLibTotal - testLibPassed << " failed" << std::endl;
	std::cout << "Overall Score: " << testLibPassed << "/" << testLibTotal << std::endl; 
}