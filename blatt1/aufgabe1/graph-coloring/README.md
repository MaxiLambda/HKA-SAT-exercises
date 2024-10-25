# How it should work

## Read DIMACS graph file
* ignore all lines starting with c
* line with `p` determines the number of nodes and edges
  * this line has to look like `p edge <int nodes> <int edges>`
* lines starting with `e` contain edge definitions
  * `e <int  node1> <int node2>`
  * there are `<int edges>` many lines with `e`
  * the biggest number in `<int node1>` and `<int node2>` is equal or less than `<int nodes>`


## Variables

* Let n be the number of nodes (nodes is 1 indexed)
* Let c be the number of available colors (color is 0 indexed)
* the number n_j + n * c_i describes if node n_j has the color c_i

## Creation of Clauses
* Create a Clauses for every edge that the nodes don't share the same color
* so node n_a and n_b don't have the same color if `-(n_a + n * c_i) -(n_b + n * c_i) 0` holds for all i

* it is neccesaary to add a TEMPORARY CLAUSE (asumption) for each node a:
  * `(n_a + n * 0) (n_a + n * 1) ... (n_a + n * i)`
  * as each node needs at least one color 

# Iteration
* keep a list of the last clauses \[x1 x2 0]
* create the clauses to add for the next iteration by adding n to x1 and x2