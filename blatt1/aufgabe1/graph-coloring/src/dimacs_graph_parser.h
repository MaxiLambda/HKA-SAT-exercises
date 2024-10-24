//
// Created by maxi on 10/24/24.
//

#ifndef GRAPH_COLORING_DIMACS_GRAPH_PARSER_H
#define GRAPH_COLORING_DIMACS_GRAPH_PARSER_H
//
// Created by maxi on 10/24/24.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

struct Graph_Definition{
    int num_nodes;
    int num_edges;
    int* vals;  // Dynamically allocated array to hold edges

    // Destructor to release the dynamically allocated array
    ~Graph_Definition() {
      delete[] vals;
    }
};

Graph_Definition* graph_from_file(char* path) {

  std::cout << "Reading path: " << path << std::endl;

  std::ifstream graph_file(path);

  // Check if file was successfully opened
  if (!graph_file.is_open()) {
    std::cerr << "Error: Could not open file at path: " << path << std::endl;
    exit(1);
  }

  Graph_Definition* definition = nullptr;

  int num_edges = 0;
  int num_nodes = 0;

  char first_char;
  std::string line;
  int edge = 0;

  //read the first line into the file
  while (std::getline(graph_file, line)) {
    //if the line is blank, skip it
    if (!line.empty()) {
      //turn the line in a string stream to read it by word
      std::istringstream iss(line);
      iss >> first_char;

      //e -> definition of an edge
      //most common case, therefore checked first
      if (first_char == 'e') {
        int x, y;
        iss >> x >> y;

        //assumes that a line starting with 'p' was executed first
        if (definition) {
          definition->vals[edge] = x;
          definition->vals[edge + 1] = y;
        } else {
          std::cout << "Invalid file. Must have 'p' line before a 'e' line" << std::endl;
          exit(1);
        }

        // Output or process x and y
        std::cout << "x = " << x << ", y = " << y << std::endl;
        edge+=2;
      } else if (first_char == 'c') {
        //skip comments
        continue;
      } else if (first_char == 'p') {
        //create new Struct
        std::string ignore;
        iss >> ignore;
        iss >> num_nodes >> num_edges;

        definition = new Graph_Definition();
        definition->num_nodes = num_nodes;
        definition->num_edges = num_edges;

        // Allocate an array of size num_edges * 2 (since each edge has 2 nodes)
        definition->vals = new int[num_edges * 2];

        std::cout << "num_nodes = " << num_nodes << ", num_edges = " << num_edges << std::endl;
      }
    }
  }
  return definition;
}
#endif //GRAPH_COLORING_DIMACS_GRAPH_PARSER_H
