# Minesweeper
This project is a C-based implementation of a modified version of the game “Minesweeper” called "ND-Minesweeper". You can use the provided functions in `minesweeper.c` to set up and play the game.

## Introduction
ND-Minesweeper is played on a n-dimensional grid of n-dimensional hypercubes, where n is a positive integer and the grid is of arbitrarily defined size. We will refer to each hypercube in the game grid as a “cell”. Each cell can therefore be defined by a set of n integer coordinates (k1, k2 . . . kn) where 0 ≤ ki ≤ xi for 1 ≤ i ≤ n where xi
is the maximum coordinate (one less than the size) of the
given i-th dimension of the grid. The dimensions of the grid do not necessarily have the same size as
each other. The cell at coordinates (0, 0 . . . 0) is always a “corner” of the grid. Each cell may contain
a mine, which is hidden from the player. The game starts with all cells unselected. Every turn, the
player selects a cell.

## Game Structure
Each cell in the ND-Minesweeper grid will be represented by the following struct. The entire grid is represented by an array of such structs.  
```c
struct cell {
    int mined;
    int selected;
    int num_adjacent;
    struct cell * adjacent[MAX_ADJACENT];
    int coords[MAX_DIM];
    int hint;
}
```
`MAX_ADJACENT` is a constant integer that can be available as a preprocessor `#define`, representing the maximum space to store cells adjacent to the given cell. This may be less than the actual number of adjacent cells depending on the dimensionality of the game; however
you are always guaranteed enough space to store pointers to adjacent cells.

`MAX_DIM` is a constant integer that can be available as a preprocessor `#define`, representing the maximum space to store coordinates representing a cell. The actual number
of coordinates required is equal to the number of dimensions of the game; however you are always
guaranteed enough space to store coordinates.

`mined` is an integer that is either 0 (no mine present at this cell) or 1 (mine present).

`selected` is an integer that is either 0 (not selected) or 1 (selected). It represents whether the cell has been selected, whether by the player or automatically.

`num_adjacent` is an integer equal to the number of cells adjacent to this one.

`adjacent` is an array of pointers to the structs representing all adjacent cells to this one, in arbitrary order.

`coords` is an array of integers representing the coordinates of the current cell.

`hint` is an integer that represents the number of adjacent mined cells to the current cell.

## Initialisation
```c
void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines,
int ** mined_cells);
```
This function will be called first, exactly once at the start of the game.

`dim` provides you with the number of dimensions of this game, and `dim_sizes` is an array of `dim`
integers representing the size of the grid (i.e. number of cells) in each respective dimension.

`mined_cells` is a `num_mines` sized array of `dim `sized integer arrays, which represents a `num_mines`
sized array of coordinates of cells that contain mines.

## Gameplay
```c
int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords);
```
This function will be called only after init_game has been called exactly once. It will be called
exactly once for each move of the player.

`coords` is a `dim` sized array of integers representing the coordinates of the cell that the player has selected. `dim_sizes` has the same meaning as for `init_game`.

`game` is the same array that you created in `init_game` or modified in previous calls of `select_cell`. 

If the player has selected a cell with a mine, return 1 and mark the cell as selected. The game has
been lost, and there will be no further function calls for this game array.

If the player has selected a cell without a mine, the `hint` field of the relevant
struct cell in the array contains the total number of mines in adjacent cells.

Furthermore, if the selected cell has 0 adjacent mines, the recursive algorithm automatically selects adjacent cells in all dimensions. If the adjacent cells also have 0 adjacent mines, it continues the recursion and only stops when there is a non-zero number of mines in adjacent cells to a particular cell. All recursively selected cells will also have the correct value for number of adjacent mines stored in the `hint` field of the corresponding `struct cell`.

