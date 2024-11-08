#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int LITTLE_SIZE;
static int SIZE;
int** solvedSudoku;
FILE* fptr;

void readSudokuFile(const char* fileName) {
    FILE *file = fopen(fileName, "r");
    char line[1024];

    fgets(line, sizeof(line), file);
    sscanf(line, "%d", &LITTLE_SIZE);
    SIZE = LITTLE_SIZE * LITTLE_SIZE;

    solvedSudoku = malloc(sizeof(int) * SIZE * SIZE);
    for(int i = 0; i < SIZE; i++) solvedSudoku[i] = malloc(SIZE * sizeof(int));
}

char readAssignment(const char* resultFileName) {
    FILE *file = fopen(resultFileName, "r");
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        const char *ptr = line + 2; // skip v and space
        int num;
        while (sscanf(ptr, "%d", &num) == 1) {
            if (num == 0) { // End of assignment reached
                return 1;
            }
            if (num > 0) {
                const int parsedNumber = (num - ((num / SIZE) * SIZE));
                const int col = (num + ( - 1)) / (SIZE * SIZE);
                const int row = (((num + ( - 1)) / SIZE) % SIZE);
                solvedSudoku[col][row] = parsedNumber == 0 ? SIZE : parsedNumber;
            }
            // Move pointer to next number
            while (*ptr != ' ' && *ptr != '\0') {
                ptr++;
            }
            if (*ptr == ' ') {
                ptr++; // Skip spaces
            }
        }
    }

    return 0;
}

void showSudokuSolution() {
    printf("|");
    for (int j = 0; j < SIZE; j++) {
        printf("---");
        if (j % LITTLE_SIZE == LITTLE_SIZE - 1) {
            printf("-|");
        }
    }
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf("| ");
        for (int j = 0; j < SIZE; j++) {
            printf("%2d ", solvedSudoku[i][j]);
            if (j % LITTLE_SIZE == LITTLE_SIZE - 1) {
                printf("| ");
            }
        }
        if (i % LITTLE_SIZE == LITTLE_SIZE - 1) {
            printf("\n|");
            for (int j = 0; j < SIZE; j++) {
                printf("---");
                if (j % LITTLE_SIZE == LITTLE_SIZE - 1) {
                    printf("-|");
                }
            }
        }
        printf("\n");
    }
}

int main(const int argc, char **argv) {
    const char* sudokuFilePath = NULL;
    const char* resultFilePath = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "f:r:")) != -1) {
        switch (opt) {
            case 'f':
                sudokuFilePath = optarg;
            break;
            case 'r':
                resultFilePath = optarg;
            break;
            case 'h':
                printf("Usage: -f <path_to_sudoku_file> -r <path_to_result_file>\n");
            return 0;
            default:
                printf("Unknown Option %c!\nUsage: -f <path_to_sudoku_file> -r <path_to_result_file>\n", opt);
            _Exit(EXIT_FAILURE);
        }
    }
    if (sudokuFilePath == NULL) {
        printf("option f requires an argument!\nUsage: -f <path_to_sudoku_file> -r <path_to_result_file>\n");
        _Exit(EXIT_FAILURE);
    }
    if (resultFilePath == NULL) {
        printf("option r requires an argument!\nUsage: -f <path_to_sudoku_file> -r <path_to_result_file>\n");
        _Exit(EXIT_FAILURE);
    }

    readSudokuFile(sudokuFilePath);

    if (readAssignment(resultFilePath)) {
        showSudokuSolution();
    } else {
        printf("The Sudoku can't be solved! It is UnSAT!!\n");
    }
    _Exit(EXIT_SUCCESS);
}
