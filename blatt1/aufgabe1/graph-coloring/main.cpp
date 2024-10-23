#include <iostream>
#include "third_party/cadical/include/cadical.hpp"
#include "third_party/cadical/include/ipasir.h"

int main() {

  void* solver = ipasir_init();
  //FIRST round

  //1 -2 0
  ipasir_add(solver, 1);
  ipasir_add(solver, -2);
  ipasir_add(solver, 0);

  int result = ipasir_solve(solver);

  if(result == 10) {
    std::printf("Solvable: \n");
    for (int i = 1; i <= 2; i++) {
      std::cout << ipasir_val(solver, i) << " ";
    }
    std::cout << std::endl;
  }

  //SECOND round

  //2 3 0
  ipasir_add(solver, 2);
  ipasir_add(solver, 3);
  ipasir_add(solver, 0);

  //1 0
  ipasir_add(solver, 1);
  ipasir_add(solver, 0);
  int result2 = ipasir_solve(solver);

  if(result2 == 10) {
    std::printf("Solvable: \n");
    for (int i = 1; i <= 3; i++) {
      std::cout << ipasir_val(solver, i) << " ";
    }
    std::cout << std::endl;
  }

  //THIRD round => fails
  //-1 0 => conflicts with 1 0
  ipasir_add(solver, -1);
  ipasir_add(solver, 0);

  int result3 = ipasir_solve(solver);
  if(result3 == 20) {
    std::printf("No solvable :(");
    std::cout << std::endl;
  }

  return 0;
}
