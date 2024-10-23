#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WRITE_TO_FILE true
#define WRITE_TO_CONSOLE false
#define WRITE_STATUS_TO_CONSOLE false

static int LITTLE_SIZE;
static int SIZE;
int** mySudoku;
int** solvedSudoku;
FILE* fptr;
float timeInSeconds = -1;

void printClause(int *clause, int size) {
    if (WRITE_TO_CONSOLE) {
        for (int i = 0; i < size; i++) {
            if (i != 0) printf(" ");
            printf("%d", clause[i]);
        }
        printf(" 0\n");
    }

    if (WRITE_TO_FILE) {
        for (int i = 0; i < size; i++) {
            if (i != 0) fprintf(fptr, " ");
            fprintf(fptr, "%d", clause[i]);
        }
        fprintf(fptr, " 0\n");
    }
}

void generateRowCnf() {
    printf("generate Row CNF\n");
    if (WRITE_TO_FILE) {
        fprintf(fptr, "c generate Row CNF\n");
    }
    for (int number = 0; number < SIZE; number++) {
        if (WRITE_STATUS_TO_CONSOLE) {
            printf("c number %d\n", number + 1);
        }
        for (int row = 0; row < SIZE; row++) {
            if (WRITE_STATUS_TO_CONSOLE) {
                printf("c Row %d\n", row + 1);
            }

            int clause[SIZE];
            for (int col = 0; col < SIZE; col++) {
                clause[col] = (number + row * SIZE * SIZE + 1) + SIZE * col;
            }
            printClause(clause, SIZE);

            for (int col1 = 0; col1 < SIZE; col1++) {
                for (int col2 = col1 + 1; col2 < SIZE; col2++) {
                    int conflict_clause[2] = {
                        -(number + row * SIZE * SIZE + 1 + SIZE * col1),
                        -(number + row * SIZE * SIZE + 1 + SIZE * col2),
                    };
                    printClause(conflict_clause, 2);
                }
            }
        }
    }
}

void generateColumnCnf() {
    printf("generate Column CNF\n");
    if (WRITE_TO_FILE) {
        fprintf(fptr, "c generate Column CNF\n");
    }
    for (int number = 0; number < SIZE; number++) {
        if (WRITE_STATUS_TO_CONSOLE) {
            printf("c number %d\n", number + 1);
        }
        for (int col = 0; col < SIZE; col++) {
            if (WRITE_STATUS_TO_CONSOLE) {
                printf("c Column %d\n", col + 1);
            }

            int clause[SIZE];
            for (int row = 0; row < SIZE; row++) {
                clause[row] = (number + col * SIZE + 1) + SIZE * SIZE * row;
            }
            printClause(clause, SIZE);

            for (int row1 = 0; row1 < SIZE; row1++) {
                for (int row2 = row1 + 1; row2 < SIZE; row2++) {
                    int conflict_clause[2] = {
                        -(number + col * SIZE + 1 + SIZE * SIZE * row1),
                        -(number + col * SIZE + 1 + SIZE * SIZE * row2),
                    };
                    printClause(conflict_clause, 2);
                }
            }
        }
    }
}

void generateSubgridCnf() {
    printf("generate SubGrid CNF\n");
    if (WRITE_TO_FILE) {
        fprintf(fptr, "c generate SubGrid CNF\n");
    }
    for (int number = 0; number < SIZE; number++) {
        if (WRITE_STATUS_TO_CONSOLE) {
            printf("c number %d\n", number + 1);
        }
        for (int i = 0; i < LITTLE_SIZE; i++) {
            for (int j = 0; j < LITTLE_SIZE; j++) {
                if (WRITE_STATUS_TO_CONSOLE) {
                    printf("c Sub-grid (%d, %d)\n", i + 1, j + 1);
                }

                int clause[SIZE];
                int index = 0;
                for (int row = 0; row < LITTLE_SIZE; row++) {
                    for (int col = 0; col < LITTLE_SIZE; col++) {
                        clause[index++] = number + 1 + (i * LITTLE_SIZE * SIZE + row * SIZE) * SIZE + (
                                              j * LITTLE_SIZE * SIZE + col * SIZE);
                    }
                }
                printClause(clause, SIZE);

                for (int cell1 = 0; cell1 < SIZE; cell1++) {
                    for (int cell2 = cell1 + 1; cell2 < SIZE; cell2++) {
                        int conflict_clause[2] = {
                            -(clause[cell1]),
                            -(clause[cell2])
                        };
                        printClause(conflict_clause, 2);
                    }
                }
            }
        }
    }
}

void generateKnownNumbersCnf() {
    printf("generate Known Numbers CNF\n");
    if (WRITE_TO_FILE) {
        fprintf(fptr, "c generate Known Numbers CNF\n");
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (mySudoku[i][j] != 0) {
                int clause[1];
                for (int number = 1; number <= SIZE; number++) {
                    if (mySudoku[i][j] == number) {
                        clause[0] = (number + i * SIZE * SIZE + j * SIZE);
                    } else {
                        clause[0] = -(number + i * SIZE * SIZE + j * SIZE);
                    }
                    printClause(clause, 1);
                }
            }
        }
    }
}

void generateOneNumberMustBeSetCnf() {
    printf("generate exactly one number CNF\n");
    if (WRITE_TO_FILE) {
        fprintf(fptr, "c generate exactly one number CNF\n");
    }
    for (int i = 0; i < SIZE; i++) {
        if (WRITE_STATUS_TO_CONSOLE) {
            printf("c Row %d\n", i + 1);
        }
        for (int j = 0; j < SIZE; j++) {
            if (WRITE_STATUS_TO_CONSOLE) {
                printf("c Column %d\n", j + 1);
            }

            int clause[SIZE];
            for (int number = 0; number < SIZE; number++) {
                clause[number] = (number + 1 + i * SIZE * SIZE) + j * SIZE;
            }
            printClause(clause, SIZE);
        }
    }
}

bool readAssignment() {
    FILE *file = fopen("result.txt", "r");
    char line[1024];
    int count = 0;
    int endOfAssignmentReached = 0;

    fgets(line, sizeof(line), file);
    if (
        line[0] == 's' && line[2] == 'U' && line[3] == 'N' && line[4] == 'S' && line[5] == 'A' && line[6] == 'T'
        && line[7] == 'I' && line[8] == 'S' && line[9] == 'F' && line[10] == 'I' && line[11] == 'A' && line[12] == 'B'
        && line[13] == 'L' && line[14] == 'E'
    ) {
        while (fgets(line, sizeof(line), file)) {
            if (line[0] == 'c' && line[10] == 't' && line[11] == 'i' && line[12] == 'm' && line[13] == 'e') {
                const char *ptr = line + 47;
                while (*ptr == ' ') {
                    ptr++;
                }
                sscanf(ptr, "%f", &timeInSeconds);
                break;
            }
        }

        fclose(file);
        return false;
    }

    while (fgets(line, sizeof(line), file)) {
        if (endOfAssignmentReached == 1) {
            if (line[0] == 'c' && line[10] == 't' && line[11] == 'i' && line[12] == 'm' && line[13] == 'e') {
                const char *ptr = line + 15;
                while (*ptr == ' ' ||  *(ptr + 4) == ' ') {
                    ptr++;
                }
                sscanf(ptr, "%f", &timeInSeconds);
            }

            break;
        }
        const char *ptr = line + 2; // skip v and space
        int num;
        while (sscanf(ptr, "%d", &num) == 1) {
            if (count > SIZE * SIZE * SIZE) {
                printf("Too many numbers, exceeding the limit\n");
                fclose(file);
                return false;
            }
            if (num == 0) { // End of assignment reached
                endOfAssignmentReached = 1;
                break;
            }
            if (num > 0) {
                const int parsedNumber = (num - ((num / SIZE) * SIZE));
                const int col = (num + ( - 1)) / (SIZE * SIZE);
                const int row = (((num + ( - 1)) / SIZE) % SIZE);
                solvedSudoku[col][row] = parsedNumber == 0 ? SIZE : parsedNumber;
            }
            count++;
            // Move pointer to next number
            while (*ptr != ' ' && *ptr != '\0') {
                ptr++;
            }
            if (*ptr == ' ') {
                ptr++; // Skip spaces
            }
        }
    }

    fclose(file);
    return true;
}

bool checkDistinct(const int arr[]) {
    bool found[SIZE];
    for (int i = 0; i < SIZE; i++) {
        found[i] = false;
    }
    for (int i = 0; i < SIZE; i++) {
        if (arr[i] <= 0 || arr[i] > SIZE) {
            printf("validation failed, because of invalid number: %d\n", arr[i]);
            return false;
        }
        if (found[arr[i] - 1]) {
            printf("validation failed, because of duplicate number: %d at position: %d\n", arr[i], i);
            return false;
        }
        found[arr[i] - 1] = true;
    }
    return true;
}

bool checkRows() {
    for (int i = 0; i < SIZE; i++) {
        if (!checkDistinct(solvedSudoku[i])) {
            printf("row: %d is not distinct\n", i);
            return false;
        }
    }
    return true;
}

bool checkColumns() {
    for (int i = 0; i < SIZE; i++) {
        int col[SIZE];
        for (int j = 0; j < SIZE; j++) {
            col[j] = solvedSudoku[j][i];
        }
        if (!checkDistinct(col)) {
            printf("col: %d is not distinct\n", i);
            return false;
        }
    }
    return true;
}

bool checkSubgrids() {
    for (int row = 0; row < SIZE; row += LITTLE_SIZE) {
        for (int col = 0; col < SIZE; col += LITTLE_SIZE) {
            int subgrid[SIZE];
            for (int i = 0; i < SIZE; i++) {
                subgrid[i] = 0;
            }
            int idx = 0;
            for (int i = 0; i < LITTLE_SIZE; i++) {
                for (int j = 0; j < LITTLE_SIZE; j++) {
                    subgrid[idx++] = solvedSudoku[row + i][col + j];
                }
            }
            if (!checkDistinct(subgrid)) {
                printf("subgrid not distinct start at row: %d and col: %d\n", row, col);
                return false;
            }
        }
    }
    return true;
}

bool checkNoChangedOriginalValues() {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (mySudoku[row][col] != 0 && mySudoku[row][col] != solvedSudoku[row][col]) {
                printf("difference to original in row: %d and col: %d\n", row, col);
                return false;
            }
        }
    }

    return true;
}

bool validateSudoku() {
    return checkRows() && checkColumns() && checkSubgrids() && checkNoChangedOriginalValues();
}

void calculateNumberVariablesAndClauses() {
    const int numStandardClauses = SIZE * SIZE * (((SIZE * (SIZE - 1)) / 2) + 1);
    const int numUsageStandardClause = 3; // row, col, subGrid Clauses
    const int numberClausesOneNumberMustBeSet = SIZE * SIZE;
    int numberKnownNumbers = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (mySudoku[i][j] != 0) {
                numberKnownNumbers++;
            }
        }
    }
    const int numberClauses = numStandardClauses * numUsageStandardClause + numberKnownNumbers * SIZE + numberClausesOneNumberMustBeSet;

    fprintf(
        fptr,
        "p cnf %d %d\n",
        SIZE * SIZE * SIZE,
        numberClauses
    );

    printf("Number clauses required: %d\n", numberClauses);
}

void readSudokuFile(const char* fileName) {
    FILE *file = fopen(fileName, "r");
    char line[1024];

    fgets(line, sizeof(line), file);
    sscanf(line, "%d", &LITTLE_SIZE);
    SIZE = LITTLE_SIZE * LITTLE_SIZE;
    mySudoku = malloc(sizeof(int *) * SIZE * SIZE);
    for(int i = 0; i < SIZE; i++) mySudoku[i] = malloc(SIZE * sizeof(int));
    solvedSudoku = malloc(sizeof(int) * SIZE * SIZE);
    for(int i = 0; i < SIZE; i++) solvedSudoku[i] = malloc(SIZE * sizeof(int));

    int rowCount = 0;
    while (fgets(line, sizeof(line), file)) {
        if (rowCount > SIZE) {
            printf("Too many lines, exceeding the limit\n");
            fclose(file);
            return;
        }
        int columnCount = 0;
        int num;
        const char *ptr = line;
        while (sscanf(ptr, "%d", &num) == 1) {
            if (rowCount > SIZE) {
                printf("Too many columns, exceeding the limit\n");
                fclose(file);
                return;
            }
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

int main(int argc, char **argv) {
    char* sudoKuFilePath = NULL;
    char* satSolverPath = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "f:s:")) != -1) {
        switch (opt) {
            case 'f':
                sudoKuFilePath = optarg;
                break;
            case 's':
                satSolverPath = optarg;
                break;
            case 'h':
                printf("Usage: -f <path_to_sudoku_file> -s <path_to_sat_solver>\n");
                return 0;
            default:
                printf("Unknown Option %c!\nUsage: -f <path_to_sudoku_file> -s <path_to_sat_solver>\n", opt);
                exit(EXIT_FAILURE);
        }
    }
    if (sudoKuFilePath == NULL) {
        printf("option f requires an argument!\nUsage: -f <path_to_sudoku_file> -s <path_to_sat_solver>\n");
        return 0;
    }
    if (satSolverPath == NULL) {
        printf("option s requires an argument!\nUsage: -f <path_to_sudoku_file> -s <path_to_sat_solver>\n");
        return 0;
    }

    readSudokuFile(sudoKuFilePath);
    if (mySudoku == NULL) {
        printf("failed to allocate memory for original sudoku!\n");
        free(mySudoku);
        free(solvedSudoku);
        return 500;
    }
    if (solvedSudoku == NULL) {
        printf("failed to allocate memory for solved sudoku!\n");
        free(mySudoku);
        free(solvedSudoku);
        return 500;
    }

    printf("Build .dimacs file with clauses\n");
    // Build .dimacs file with clauses
    fptr = fopen("clauses.dimacs", "w");
    calculateNumberVariablesAndClauses();
    fclose(fptr);

    fptr = fopen("clauses.dimacs", "a");
    generateRowCnf();
    generateColumnCnf();
    generateSubgridCnf();
    generateKnownNumbersCnf();
    generateOneNumberMustBeSetCnf();
    fclose(fptr);

    printf("Calculate solution\n");
    // Calculate solution via terminal
    const char* firstPart = "sh -c '";
    const char* end = " clauses.dimacs | grep -e ^s -e ^v -e \"^c process-time:\" > result.txt'";
    const size_t firstPartLength = strlen(firstPart);
    const size_t pathLength = strlen(satSolverPath);
    const size_t endLength = strlen(end);
    const size_t length = firstPartLength + pathLength + endLength;
    char *const command = malloc(length + 1);
    strcpy(command, firstPart);
    strcpy(command + firstPartLength, satSolverPath);
    strcpy(command + firstPartLength + pathLength, end);
    const int ret = system(command);
    free(command);
    if (ret == 0) {
        printf("Command executed successfully.\n");
    } else {
        printf("Error occurred during command execution. Return code: %d\n", ret);
        free(mySudoku);
        free(solvedSudoku);
        return 503;
    }

    printf("Read solution and show it\n");
    // Read solution from file and show it
    if (readAssignment()) {
        showSudokuSolution();

        if (validateSudoku(solvedSudoku)) {
            printf("The Sudoku solution is valid.\n");
        } else {
            printf("The Sudoku solution is invalid.\n");
        }
    } else {
        printf("The Sudoku can't be solved!\n");
    }

    printf("it took the SAT-solver %f seconds to get a solution.\n", timeInSeconds);

    for(int i = 0; i < SIZE; i++) free(mySudoku[i]);
    free(mySudoku);
    for(int i = 0; i < SIZE; i++) free(solvedSudoku[i]);
    free(solvedSudoku);
    return 0;
}
