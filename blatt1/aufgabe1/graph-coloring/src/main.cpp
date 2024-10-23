#include <iostream>
#include "../third_party/cadical/include/cadical.hpp"
#include "../third_party/cadical/include/ipasir.h"

int main() {
  //contents of an example file
  const int num_nodes = 5;
  const int num_edges = 6;
  //these correlate with the initial variables
  int edges[num_edges * 2] = {
          1, 5,
          2, 3,
          2, 4,
          2, 5,
          3, 4,
          4, 5,
  };

//  const int num_nodes = 4;
//  const int num_edges = 6;
//  //these correlate with the initial variables
//  int edges[num_edges * 2] = {
//          1, 2,
//          1, 3,
//          1, 4,
//          2, 3,
//          2, 4,
//          3, 4,
//  };

  //early return if no edges are given, one color is sufficient then
  if (num_edges == 0) {
    std::cout << "Solvable with " << 1 << " color" << std::endl;
    return 0;
  }

  //global setup
  int colors = 1;
  void *solver = ipasir_init();
  bool solved = false;

  //initial clauses
  for (int i = 0; i < num_edges * 2; i += 2) {
    ipasir_add(solver, -edges[i]);
    ipasir_add(solver, -edges[i + 1]);
    ipasir_add(solver, 0);
    std::cout << -edges[i] << " " << -edges[i + 1] << " 0" << std::endl;
  }

  while (!solved) {
    //changes for next round
    colors++;
    std::cout << "Try solving with " << colors << " colors" << std::endl;
    for (int i = 0; i < num_edges * 2; ++i) {
      //the new variable has the same value as the previous one, offset by the number of nodes +1
      //the plus one is reserved for assumptions
      edges[i] = edges[i] + num_nodes + 1;
    }
    //add new clauses from vars
    for (int i = 0; i < num_edges * 2; i += 2) {
      //-a -b 0 as this clause is only false when a and b are set e.g. the same color
      ipasir_add(solver, -edges[i]);
      ipasir_add(solver, -edges[i + 1]);
      ipasir_add(solver, 0);
      std::cout << -edges[i] << " " << -edges[i + 1] << " 0" << std::endl;
    }

    //add clauses to make sure each node has a color
    for (int i = 1; i <= num_nodes; ++i) {
      //assumption variable, for the current round
      ipasir_add(solver, colors * (num_nodes + 1));
      std::cout << colors * (num_nodes + 1) << " ";
      //add all multiples/color values of the node i
      for (int j = 0; j < colors; ++j) {
        ipasir_add(solver, i + j * (num_nodes + 1));
        std::cout << i + j * (num_nodes + 1) << " ";
      }
      ipasir_add(solver, 0);
      std::cout << 0 << std::endl;
    }

    //add assumptions to make sure each node has at least one color
    //all multiples of (num_nodes + 1) are assumptions
    //only the last (num_nodes + 1) * colors is set, the rest is unset
    // -> set values to x to disable the old clauses
    // -> set values to -x to enable new clauses
    for (int i = 1; i < colors; ++i) {
      //disable all assumptions but the last
      ipasir_assume(solver, i * (num_nodes + 1));
      std::cout << "assume " << i * (num_nodes + 1) << std::endl;
    }
    //enable last assumption
    ipasir_assume(solver, -colors * (num_nodes + 1));
    std::cout << "assume " << -colors * (num_nodes + 1) << std::endl;

    //check if solvable
    solved = 10 == ipasir_solve(solver);
  }
  std::cout << "Solvable with " << colors << " colors" << std::endl;

  for (int i = 1; i <= colors * (num_nodes + 1); ++i) {
    std::cout << ipasir_val(solver, i) << " ";
  }
  std::cout << std::endl;

  return 0;
}
