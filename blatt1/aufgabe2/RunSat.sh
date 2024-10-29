# set your paths here:
sudokuFile="/home/user/Downloads/sudokus/puzzle04a.sudoku"
satSolverPath="/home/user/Downloads/kissat-4.0.1-linux-amd64"
resultFileName="result.txt"

./createDimacs -f /home/user/Downloads/sudokus/puzzle04a.sudoku
"$satSolverPath" clauses.dimacs | grep -e ^v > "$resultFileName"
./readSolution -f "$sudokuFile" -r "$resultFileName"
