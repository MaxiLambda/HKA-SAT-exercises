#include <iostream>
#include "third_party/cadical/include/cadical.hpp"
#include "third_party/cadical/include/ipasir.h"

int main() {
  std::cout << "Hello, World!" << std::endl;

  void* solver = ipasir_init();

  ipasir_add(solver, 1);
  ipasir_add(solver, -2);
  ipasir_add(solver, 0);

  std::cout << ipasir_solve(solver) << std::endl;

  return 0;
}
