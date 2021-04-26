#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sir.hpp"

TEST_CASE("Testing Struct Data") {
  Data data;
  CHECK(data.S == 0);
  CHECK(data.I == 0);
  CHECK(data.R == 0);
  Data data_1{1, 2, 3};
  CHECK(data_1.S == 1);
  CHECK(data_1.I == 2);
  CHECK(data_1.R == 3);
}

TEST_CASE("Testing Class Population") {
  Data data{1, 2, 3};
  Population population{0.1, .2, data};
  CHECK(population.I() == 2);
  CHECK(population.S() == 1);
  CHECK(population.R() == 3);
  CHECK(population.Total() == 6);
  CHECK(population.Beta() == .1);
  CHECK(population.Gamma() == .2);
}