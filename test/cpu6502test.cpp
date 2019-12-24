#include "cpu6502test.h"

#include "../catch.hpp"

TEST_CASE("cpu case", "[cpu]")
{
	int a=2;
	int b=3;

	REQUIRE(a+b == 5);
}