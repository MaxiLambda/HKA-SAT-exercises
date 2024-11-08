#define WRITE_TO_FILE true
#define WRITE_TO_CONSOLE false
#define WRITE_STATUS_TO_CONSOLE false
#define SPEED_OPTION true

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "speed.h"

void readSudokuFile(const char* fileName) {
    FILE *file = fopen(fileName, "r");
    char line[1024];

    fgets(line, sizeof(line), file);
    sscanf(line, "%d", &LITTLE_SIZE);
    SIZE = LITTLE_SIZE * LITTLE_SIZE;
    SIZE_QUAD = SIZE * SIZE;
    mySudoku = malloc(sizeof(int *) * SIZE * SIZE);
    for(int i = 0; i < SIZE; i++) mySudoku[i] = malloc(SIZE * sizeof(int));

    int rowCount = 0;
    while (fgets(line, sizeof(line), file)) {
        int columnCount = 0;
        int num;
        const char *ptr = line;
        while (sscanf(ptr, "%d", &num) == 1) {
            mySudoku[rowCount][columnCount] = num;

            columnCount++;
            // Move pointer to next number
            while (*ptr != ' ' && *ptr != '\0') {
                ptr++;
            }
            if (*ptr == ' ') {
                ptr++; // Skip spaces
            }
        }
        rowCount++;
    }
}

int main(const int argc, char **argv) {
#if (SPEED_OPTION)
    const char* sudokuFilePath = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
            case 'f':
                sudokuFilePath = optarg;
                break;
            case 'h':
                printf("Usage: -f <path_to_sudoku_file>\n");
                return 0;
            default:
                printf("Unknown Option %c!\nUsage: -f <path_to_sudoku_file>\n", opt);
                _Exit(EXIT_FAILURE);
        }
    }
    if (sudokuFilePath == NULL) {
        printf("option f requires an argument!\nUsage: -f <path_to_sudoku_file>\n");
        _Exit(EXIT_FAILURE);
    }

    readSudokuFile(sudokuFilePath);

    // Build .dimacs file with clauses
    fptr = fopen("clauses.dimacs", "w");
    calculateNumberVariablesAndClauses();
    fclose(fptr);

    fptr = fopen("clauses.dimacs", "a");
    generateSpeedCNF();
    fclose(fptr); // We need to close the file, to write our changes.
    _Exit(EXIT_SUCCESS);
#else
    printf("this program was compiled without the SPEED_OPTION set to true in the preprocessor macro. Therefore this wont work.\n");
    _Exit(EXIT_FAILURE);
#endif
}
