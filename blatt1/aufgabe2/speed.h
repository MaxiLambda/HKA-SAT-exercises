#pragma once
#include <stdio.h>

static int LITTLE_SIZE;
static int SIZE;
static int SIZE_QUAD;

int** mySudoku;
FILE* fptr;

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
void printClause(const int *clause, const int size) {
#if (WRITE_TO_CONSOLE)
    for (int i = 0; i < size; i++) {
        if (i != 0) printf(" ");
        printf("%d", clause[i]);
    }
    printf(" 0\n");
#endif

#if (WRITE_TO_FILE)
    fprintf(fptr, "%d", clause[0]);
    for (int i = 1; i < size; i++) {
        fprintf(fptr, " %d", clause[i]);
    }
    fprintf(fptr, " 0\n");
#endif
}

#if (SPEED_OPTION)
// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
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
}

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
void generateSpeedCNF() {
    int clauseGrid[LITTLE_SIZE][SIZE];
    int clauseRow[SIZE];
    int clauseCol[SIZE];
    int clauseKnown[SIZE];
    int clauseOneSet[SIZE];
    int conflictClauseCol[2];
    int conflictClauseRow[2];
    int conflictClauseGrid1[2];
    int conflictClauseGrid2[2];
    int conflictClauseGrid3[2];
    for (int number = 0; number < SIZE; number++) {
        const int numberSizeSize = number * SIZE_QUAD;
        for (int i = 0; i < SIZE; i++) {
            const int iSize = i * SIZE;
            const int iSizeNumberSizeSize = numberSizeSize + iSize;
            char isSubGrid = 0;
            //known number
            if (mySudoku[number][i] != 0) {
                for (int iii = 1; iii <= SIZE; iii++) {
                    if (mySudoku[number][i] == iii) {
                        clauseKnown[0] = iii + iSizeNumberSizeSize;
                    } else {
                        clauseKnown[0] = -(iii + iSizeNumberSizeSize);
                    }
                    printClause(clauseKnown, 1);
                }
            } // TODO evtl. einen else Zweig?

            const int numberISizeSize1 = number + iSize * SIZE + 1;
            const int numberISizeSize11 = number + iSize + 1;
            for (int ii = 0; ii < SIZE; ii++) {
                const int iiSize = SIZE * ii;
                //row
                clauseRow[ii] = numberISizeSize1 + iiSize;

                //col
                clauseCol[ii] = numberISizeSize11 + SIZE * iiSize;

                //subGrid
                if (i < LITTLE_SIZE && ii < LITTLE_SIZE) {
                    isSubGrid = 1;
                    int index = 0;
                    for (int row = 0; row < LITTLE_SIZE; row++) {
                        for (int col = 0; col < LITTLE_SIZE; col++) {
                            clauseGrid[ii][index++] = number + 1 + (i * LITTLE_SIZE * SIZE + row * SIZE) * SIZE + (
                                                          ii * LITTLE_SIZE * SIZE + col * SIZE);
                        }
                    }
                    printClause(clauseGrid[ii], SIZE);
                }

                //oneMustBeSet
                clauseOneSet[ii] = ii + 1 + numberSizeSize + iSize;
            }
            printClause(clauseRow, SIZE);
            printClause(clauseCol, SIZE);
            printClause(clauseOneSet, SIZE);

            // only one clause
            for (int iii = 0; iii < SIZE; iii++) {
                conflictClauseRow[0] = -clauseRow[iii];
                conflictClauseCol[0] = -clauseCol[iii];
                conflictClauseGrid1[0] = -clauseGrid[0][iii];
                conflictClauseGrid2[0] = -clauseGrid[1][iii];
                conflictClauseGrid3[0] = -clauseGrid[2][iii];
                for (int iv = iii + 1; iv < SIZE; iv++) {
                    //row
                    conflictClauseRow[1] = -clauseRow[iv];
                    printClause(conflictClauseRow, 2);

                    // col
                    conflictClauseCol[1] = -clauseCol[iv];
                    printClause(conflictClauseCol, 2);

                    //subGrid
                    if (isSubGrid == 1) {
                        conflictClauseGrid1[1] = -clauseGrid[0][iv];
                        printClause(conflictClauseGrid1, 2);
                        conflictClauseGrid2[1] = -clauseGrid[1][iv];
                        printClause(conflictClauseGrid2, 2);
                        conflictClauseGrid3[1] = -clauseGrid[2][iv];
                        printClause(conflictClauseGrid3, 2);
                    }
                }
            }
        }
    }
}
#endif
