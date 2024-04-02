#include <iostream>
#include "bot.hpp"
#include "testlib.hpp"

bool foo() {
	return false;
}

int main() {

	test("Get piece value on empty board", foo);
	
	printResults();

	return 0;
}