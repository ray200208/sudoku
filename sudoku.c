#include <stdio.h>
#include <stdbool.h>

#define SIZE 9

// Function Prototypes
void printGrid(int grid[SIZE][SIZE]);
bool isSafe(int grid[SIZE][SIZE], int row, int col, int num);
bool solveSudoku(int grid[SIZE][SIZE]);
bool findEmptyCell(int grid[SIZE][SIZE], int *row, int *col);
bool validateInitialGrid(int grid[SIZE][SIZE]);
void interactivePlay(int grid[SIZE][SIZE], int solvedGrid[SIZE][SIZE]);

int main() {
    int grid[SIZE][SIZE], solvedGrid[SIZE][SIZE], choice;
    char playerName[50];

    printf("=========================================\n");
    printf("   SUDOKU: PLAY OR SOLVE\n");
    printf("=========================================\n");

    printf("Enter Player Name: ");
    scanf("%s", playerName);

    printf("\nEnter the 9x9 Sudoku grid (0 for empty):\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            scanf("%d", &grid[i][j]);
            solvedGrid[i][j] = grid[i][j]; // Copy for the solver
        }
    }

    if (!validateInitialGrid(grid)) {
        printf("\n[ERROR] Initial puzzle is invalid!\n");
        return 1;
    }

    // Pre-solve the puzzle in the background to validate moves during play
    bool solvable = solveSudoku(solvedGrid);

    printf("\n1. Interactive Play (Solve it yourself)\n");
    printf("2. Show Solution (Automatic Solver)\n");
    printf("Selection: ");
    scanf("%d", &choice);

    if (choice == 1) {
        if (!solvable) {
            printf("\nThis puzzle is unsolvable! Switching to manual check mode.\n");
        }
        interactivePlay(grid, solvedGrid);
    } else {
        if (solvable) {
            printf("\nSolution for %s:\n", playerName);
            printGrid(solvedGrid);
        } else {
            printf("\nNo solution exists for this puzzle[cite: 60].\n");
        }
    }

    return 0;
}

void interactivePlay(int grid[SIZE][SIZE], int solvedGrid[SIZE][SIZE]) {
    int r, c, val, score = 0;
    bool fixed[SIZE][SIZE];

    // Mark original numbers as "fixed" so they can't be overwritten
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            fixed[i][j] = (grid[i][j] != 0);

    while (findEmptyCell(grid, &r, &c)) {
        printf("\nScore: %d\n", score);
        printGrid(grid);
        printf("\nEnter Row (1-9), Column (1-9), and Value (1-9) separated by spaces: ");
        scanf("%d %d %d", &r, &c, &val);

        // Adjust for 0-indexing
        r--; c--;

        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || val < 1 || val > 9) {
            printf("[!] Invalid input. Stay within 1-9[cite: 110].\n");
            continue;
        }

        if (fixed[r][c]) {
            printf("[!] You cannot overwrite a fixed cell[cite: 110].\n");
        } else if (!isSafe(grid, r, c, val)) {
            printf("[X] WRONG MOVE! Duplicate detected in row, col, or sub-grid. Try again.\n");
            score -= 5; // Penalty for wrong move
        } else {
            grid[r][c] = val;
            score += 10; // Reward for valid move
            printf("[+] Good move!\n");
        }
    }
    printf("\nCONGRATULATIONS! Puzzle Completed.\nFinal Score: %d\n", score);
    printGrid(grid);
}

void printGrid(int grid[SIZE][SIZE]) {
    printf("\n    1 2 3   4 5 6   7 8 9\n"); // Column indices for easier play
    printf("  -------------------------\n");
    for (int r = 0; r < SIZE; r++) {
        if (r % 3 == 0 && r != 0) printf("  ------+-------+------\n");
        printf("%d | ", r + 1); // Row indices
        for (int c = 0; c < SIZE; c++) {
            if (c % 3 == 0 && c != 0) printf("| ");
            if (grid[r][c] == 0) printf(". ");
            else printf("%d ", grid[r][c]);
        }
        printf("\n");
    }
    printf("  -------------------------\n");
}

bool isSafe(int grid[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; x++)
        if (grid[row][x] == num || grid[x][col] == num) return false;

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num) return false;
    return true;
}

bool findEmptyCell(int grid[SIZE][SIZE], int *row, int *col) {
    for (*row = 0; *row < SIZE; (*row)++)
        for (*col = 0; *col < SIZE; (*col)++)
            if (grid[*row][*col] == 0) return true;
    return false;
}

bool solveSudoku(int grid[SIZE][SIZE]) {
    int row, col;
    if (!findEmptyCell(grid, &row, &col)) return true;
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveSudoku(grid)) return true;
            grid[row][col] = 0;
        }
    }
    return false;
}

bool validateInitialGrid(int grid[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            int num = grid[r][c];
            if (num != 0) {
                grid[r][c] = 0;
                if (!isSafe(grid, r, c, num)) return false;
                grid[r][c] = num;
            }
        }
    }
    return true;
}
