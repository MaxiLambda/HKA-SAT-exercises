./createDimacs -f /home/user/Downloads/sudokus/puzzle03a.sudoku

/home/user/Downloads/kissat-4.0.1-linux-amd64 clauses.dimacs | grep -e ^v > result.txt

./readSolution -f /home/user/Downloads/sudokus/puzzle03a.sudoku -r result.txt