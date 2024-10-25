#include <iostream>
#include "../third_party/cadical/include/ipasir.h"
#include "dimacs_graph_parser.h"
#include <chrono>

int main(int argc, char *argv[]) {

  auto start_time = std::chrono::high_resolution_clock::now();

  if(argc != 2) {
    std::cout << argc << " args supplied!" << std::endl;
    return 1;
  }

 Graph_Definition* def =   graph_from_file(argv[1]);

  auto end_read_file = std::chrono::high_resolution_clock::now();

  //early return if no edges are given, one color is sufficient then
  if (def->num_edges == 0) {
    std::cout << "Solvable with " << 1 << " color" << std::endl;
    return 0;
  }

  //global setup
  int colors = 1;
  void *solver = ipasir_init();
  bool solved = false;
  int result;

  //initial clauses
  for (int i = 0; i < def->num_edges * 2; i += 2) {
    ipasir_add(solver, -def->vals[i]);
    ipasir_add(solver, -def->vals[i + 1]);
    ipasir_add(solver, 0);
    std::cout << -def->vals[i] << " " << -def->vals[i + 1] << " 0" << std::endl;
  }

  while (!solved) {
    //changes for next round
    colors++;
    std::cout << "Try solving with " << colors << " colors" << std::endl;
    for (int i = 0; i < def->num_edges * 2; ++i) {
      //the new variable has the same value as the previous one, offset by the number of nodes +1
      //the plus one is reserved for assumptions
      def->vals[i] = def->vals[i] + def->num_nodes + 1;
    }
    //add new clauses from vars
    for (int i = 0; i < def->num_edges * 2; i += 2) {
      //-a -b 0 as this clause is only false when a and b are set e.g. the same color
      ipasir_add(solver, -def->vals[i]);
      ipasir_add(solver, -def->vals[i + 1]);
      ipasir_add(solver, 0);
      std::cout << -def->vals[i] << " " << -def->vals[i + 1] << " 0" << std::endl;
    }

    //add clauses to make sure each node has a color
    for (int i = 1; i <= def->num_nodes; ++i) {
      //assumption variable, for the current round
      ipasir_add(solver, colors * (def->num_nodes + 1));
      std::cout << colors * (def->num_nodes + 1) << " ";
      //add all multiples/color values of the node i
      for (int j = 0; j < colors; ++j) {
        ipasir_add(solver, i + j * (def->num_nodes + 1));
        std::cout << i + j * (def->num_nodes + 1) << " ";
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
      ipasir_assume(solver, i * (def->num_nodes + 1));
      std::cout << "assume " << i * (def->num_nodes + 1) << std::endl;
    }
    //enable last assumption
    ipasir_assume(solver, -colors * (def->num_nodes + 1));
    std::cout << "assume " << -colors * (def->num_nodes + 1) << std::endl;

    //check if solvable
    result = ipasir_solve(solver);
    solved = 10 == result;

    if(!solved && result != 20) {
      std::cout << " Unsolvable" << std::endl;
      exit(1);
    }
  }

  auto done_time = std::chrono::high_resolution_clock::now();

  std::cout << "Solvable with " << colors << " colors" << std::endl;

  //print the solution, each row corresponds to the bitvector of a node
  //the assumptions are omitted, change "def->num_nodes" to "def->num_nodes + 1" to print them as the last line
  for (int i = 1; i <= def->num_nodes; ++i) {
    for (int j = 0; j < colors; ++j) {
      std::cout << ipasir_val(solver, i + j * (def->num_nodes + 1)) << " ";
    }
    std::cout << std::endl;
  }

  auto parse_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_read_file - start_time);
  auto solve_time = std::chrono::duration_cast<std::chrono::milliseconds>(done_time - end_read_file);
  auto full_time = std::chrono::duration_cast<std::chrono::milliseconds>(done_time - start_time);

  //print again, because printing all the nodes takes a lot of space
  std::cout << "Solvable with " << colors << " colors" << std::endl;
  std::cout <<"Parsing file took " <<   parse_time.count()  << " ms" << std::endl;
  std::cout <<"Solving took " <<   solve_time.count()  << " ms" << std::endl;
  std::cout << "Full duration " << full_time.count() << " ms" << std::endl;
  return 0;
}
