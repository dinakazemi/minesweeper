#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "minesweeper.h"

//Recursive function to give us the coordinates of the cells using the Cartesian product of the arrays.
void cartesian_product (int product_element_1,int product_element_2, int * dim_sizes, int cell_count, int dim, int product_result[1000][MAX_DIM], int counter, int dim_mult){

    //index of the product reault being calculated
    int index = 0;

    //counters to count through each dimension
    int element_1_counter = 0;
    int element_2_counter = 0;

    //if we are calculating the product of the first two elements
    if (counter == 0){
        while (1){

            //if all possible outcomes are calculated break out of the loop
            if (index >= cell_count - 1){
                    break;
            }

            //calculating the product
            for (int i=0;i<dim_mult*product_element_2;i++){

                //dim_mul is a variable that stores the multiplication of the dimensions that have been visited in the function so far
                for (int j=0;j<cell_count/(dim_mult*product_element_2);j++){
                    product_result[index][counter] = element_1_counter;
                    product_result[index][counter+1] = element_2_counter;
                    index++;
                }

                //increment the dimension counter
                element_2_counter++;
                //if the dimension limit has been reached, set the counter back to 0
                if (element_2_counter == product_element_2){
                    element_2_counter = 0;
                    //every time the second dimension counter reaches its limit, first dimension counter is incremented
                    element_1_counter++;
                }
                //if the dimension limit has been reached, set the counter back to 0
                if (element_1_counter == product_element_1){
                    element_1_counter = 0;
                }

            }
        }
    }

    //if calculating the product of any two elements except the first two
    else {
        while (1){
            //if all the products have been calculated end the loop
            if (index>=cell_count){
                break;
            }
            for (int i=0;i<dim_mult*product_element_2;i++){
                for (int j=0;j<cell_count/(dim_mult*product_element_2);j++){
                    product_result[index][counter+1] = element_2_counter;
                    index++;
                }
                element_2_counter++;
                if (element_2_counter == product_element_2){
                    element_2_counter = 0;
                }
            }
        }
    }
    counter++;

    /*if we have done Cartesian product on all dimensions, terminate the recursion and return. All of the coordinates are now
    in product_result[][]*/ 
    if (counter == dim-1) {
        return;
    }

    //otherwise, multiply next dimension into the product_result that we have so far by calling the function recursively.
    else{
        dim_mult*=dim_sizes[counter];
        cartesian_product(dim_sizes[counter],dim_sizes[counter+1], dim_sizes,cell_count,dim, product_result, counter, dim_mult);
    }
}


//a function to set up the adjacents 
void set_adjacents (struct cell * game, int cell_count, int dim) {

    //a variable to keep track of which cell in the adjacent array of the visiting cell in being initialised.
    int index = 0;
    //this acts as a boolean. If we have found an adjacent cell, this will be set to 1 and if not, this will be set to 0
    int is_adjacent = 1;

    //go through each cell in the game and check its coordinates against other cells in the game.
    for (int i=0;i<cell_count;i++){
        for (int j=0;j<cell_count;j++){
            for (int c=0;c<dim;c++){
                //skip the iteration if we are checking the same cells and set the is_adjacent variable to 0 (false).
                if (j == i){
                    is_adjacent = 0;
                    break;
                }
                
                //if the two cells meet the below condition, then they are adjacent. So, we don't need to do anything as is_adjacent is already equal to 1 (true)
                if (abs(game[i].coords[c] - game[j].coords[c]) == 1 || abs(game[i].coords[c] - game[j].coords[c]) == 0){
            
                }
                //if the conditions do not apply, is_adjacent becomes equal to 0 (false) and we break out of the loop
                else{
                    is_adjacent = 0;
                    break;
                }
            }
            //if the two cells are adjacent
            if (is_adjacent){
                game[i].adjacent[index] = &game[j];
                game[i].num_adjacent ++ ;
                index++;
            }

            //for each new iteration, set is_adjacent to its initial value
            is_adjacent = 1;
        }
        index = 0;
    }

}

//function to set up mines
void set_mines (struct cell *game, int cell_count, int dim, int num_mines,int ** mined_cells){
    //an variable that remains 1 when a mined cell is found and turns 0 whenever the cell being visited is not mined
    int is_mined = 1;
    //loop through the cells in the game
    for (int i=0;i<cell_count;i++){
        //loop through the elements of the mined_cells array
        for (int j=0;j<num_mines;j++){
            is_mined = 1;
            //checking the coordinates of the current cell to the ones in the mined_cells array
            for (int c=0;c<dim;c++){
                //if they are not the same, is_mined is wrong and we break out of the loop
                if (game[i].coords[c] != mined_cells[j][c]){
                    is_mined = 0;
                    break;
                }
            }
            //if the cell is mined, set its mined attribute to 1
            if (is_mined){
                game[i].mined = 1;
                break;
            }
        }
    }

}

//a function to calculate the hint attribute for every cell in the game
void calculate_hint (struct cell * game, int cell_count, int dim){
    //loop through each cell
    for (int i=0;i<cell_count;i++){
        //loop through each cell's adjacents
        for (int c=0;c<game[i].num_adjacent;c++){
                //if mined, increase the cell's hint attribute
                if (game[i].adjacent[c]->mined == 1){
                    game[i].hint++;
                }
        }
    }

}


void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {

    //first let's count the total number of cells in the game
    int cell_count = 1;
    for (int i=0;i<dim;i++){
        cell_count = cell_count*dim_sizes[i];
    }
    //if there is only one dimension, we do not need to call the recursive function to find the coordinates for us
    if (dim == 1){
        
        //looping through the cells to initialise their attributes
        for (int i=0;i<cell_count;i++){
            game[i].coords[0] = i;
            //we initially assume that none of the cells are selected or mined
            game[i].selected = 0;
            game[i].mined = 0;
        }
    }
    else{
        //setting up a 2d array to store the results of the cartesian product
        int product_result[1000][MAX_DIM];
        for (int n=0;n<cell_count;n++){
            for (int j=0;j<dim;j++){
                product_result[n][j] = 0;
            }
        }
        int counter = 0;
        //calling the cartesian product function to give us the coordinates
        cartesian_product(dim_sizes[0], dim_sizes[1], dim_sizes, cell_count, dim, product_result, counter, dim_sizes[0]);
        //looping through the cells to initialise their attributes
        for (int i=0;i<cell_count;i++){
            for (int j=0;j<dim;j++){
                game[i].coords[j] = product_result[i][j];
                //we initially assume that none of the cells are selected or mined
                game[i].selected = 0;
                game[i].mined = 0;
            }
        }

    }

    //setting up adjacents 
    set_adjacents(game, cell_count, dim);

    //setting up mines
    set_mines(game, cell_count, dim, num_mines, mined_cells);

    //calculate hint
    calculate_hint(game, cell_count, dim);
   
    return;
}

//the function checks whether all cells are selected, if so it returns 1 and if not it returns 0
int check_win_status (struct cell * game, int cell_count){

    //variable to count the total number of selected cells in the game
    int total_selected = 0;

    //variable to count the total number of mined cells in the game
    int num_mine = 0;

    //counting mines cells and selected cells
    for (int i=0;i<cell_count;i++){
        if (game[i].selected == 1){
            total_selected ++;
        }
        if (game[i].mined == 1){
            num_mine++;
        }

    }

    //if all unmined cells have been selected, the player has won the game (returns 1)
    if (total_selected == cell_count - num_mine){
        return 1;
    }

    //otherwise, the game is not won yet (returns 0)
    return 0;
}

//the function that recursively selects the cells. Used in select_cell
void select_cell_recursive (struct cell * visiting_cell){

    //if the cell does not have any surrounding mines, continue the selection process
    if (visiting_cell -> hint == 0){
        //check its adjacents
        for (int i=0;i<visiting_cell -> num_adjacent; i++){
            //if the adjacent cells are not selected, select them and call the function recursively
            if (visiting_cell -> adjacent[i] -> selected == 0){
                visiting_cell -> adjacent[i] -> selected = 1;
                select_cell_recursive(visiting_cell -> adjacent[i]);
            }
        }
    }
}

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords) {

    //calculating the total number of cells in the game
    int cell_count = 1;
    for (int i=0;i<dim;i++){
        cell_count = cell_count*dim_sizes[i];
    }

    for (int i=0;i<dim;i++){
        //if the coordinates of the selected cell are more than the dimension bound
        if (coords[i] > dim_sizes[i] - 1){
            return 0;
        }

        //if the coordinates of the selected cell are less than 0
        if (coords[i] < 0){
            return 0;
        }
    }

    //looping through the cells in the game to find the selected cell
    int is_found = 1;
    for (int i=0;i<cell_count;i++){
        is_found = 1;
        for (int j=0;j<dim;j++){
            if (game[i].coords[j] != coords[j]){
                is_found = 0;
                break;
            }
        }

        //if a matching cell is found
        if (is_found){

            //if the cell is already selected
            if (game[i].selected == 1){
                return 0;
            }

            //if it is not selected and it is mined
            if (game[i].mined == 1){
                //select the cell
                game[i].selected = 1;
                //the player loses the game and the function returns 1
                return 1;
            }

            //if it is not selected and not mined
            else {
                game[i].selected = 1;
                //if the game is won, return 2
                if (check_win_status(game, cell_count) == 1){
                    return 2;
                }
    
                //if the selected cell has adjacnet mines
                if (game[i].hint != 0){
                    return 0;
                }

                //otherwise, we have to implement the recursive algorithm
                else{
                    select_cell_recursive(&game[i]);
                    //checking to see if the player has won the game after the recursive selection
                    if (check_win_status(game, cell_count) == 1){
                        return 2;
                    }
                }
            }
        }
    }
    return 0;
}
