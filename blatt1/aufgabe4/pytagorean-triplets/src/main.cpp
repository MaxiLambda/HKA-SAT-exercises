#include <iostream>
#include "../third_party/cadical/include/ipasir.h"
#include <chrono>
#include <list>
#include <set>

int main(int argc, char *argv[]) {

  struct Triplet {
      int a;
      int b;
      int c;
  };


  void *solver = ipasir_init();

  //all required pytagorean triplets
  std::list<Triplet *> triplets{};
  //nums::size() determines number o required variables
//  std::set<int> nums;

  for (int c = 1; c < 1000; ++c) {
    for (int b = 1; b < c; ++b) {
      for (int a = 1; a < b; ++a) {
        if (a * a + b * b == c * c) {
          printf("(%d,%d,%d)\n", a, b, c);
          ipasir_add(solver, a);
          ipasir_add(solver, b);
          ipasir_add(solver, c);
          ipasir_add(solver, 0);
          ipasir_add(solver, -a);
          ipasir_add(solver, -b);
          ipasir_add(solver, -c);
          ipasir_add(solver, 0);
          triplets.push_back(new Triplet{a, b, c});
//          nums.insert(a);
//          nums.insert(b);
//          nums.insert(c);
        }
      }
    }
  }

  int result = ipasir_solve(solver);
  if (result != 10) {
    std::cout << " Unsolvable" << std::endl;
    exit(1);
  }

  printf("Solution:\n");

  for (const Triplet *triplet: triplets) {
    printf("(%d,%d,%d): (%d,%d,%d)\n", triplet->a, triplet->b, triplet->c, ipasir_val(solver, triplet->a),
           ipasir_val(solver, triplet->b), ipasir_val(solver, triplet->c));
  }

  return 0;
}