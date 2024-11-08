# How it should work

## Build instructions
`cd blatt1/aufgabe2`
`mkdir build && cd build`
`cmake CMakeLists.txt ..`
`make`

## Run with
`./sudoku -f <Path/To/sudokuFile> -s <path/To/satSolverPath>`

Der Code ist leider quasi unleserlich, aufgrund der verschiedenen Optionen, welche durch 
PreProzessor Macros (de)aktiviert werden. Die sind hauptsächlich dazu da, um möglichst wenig 
unnötiges Zeug zu machen. Um dadurch möglichst schnell zu sein. 
