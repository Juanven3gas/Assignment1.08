#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Dungeon.h"

Dungeon::Dungeon()
{
    pc_character = '@';
   init_dungeon();
   add_rooms();
   add_corridors();
   init_hardness();
   place_pc();
   place_stairs();
   win_status = 0;
   pc_dead_status = 0;
   turn = 0;
   pc_last_position = '.';
   teleport_state = 0;
   fog_state = 1;
}

void Dungeon::place_pc_randomly(void)
{
    srand(time(NULL));

    dungeon[pc_x_position][pc_y_position] = pc_last_position;
    pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;

    pc_x_position = (rand() % (DUNGEON_ROWS - 2)) + 1;
    pc_y_position = (rand() % (DUNGEON_COLUMNS - 2)) + 1;

    pc_last_position = dungeon[pc_x_position][pc_y_position];

    dungeon[pc_x_position][pc_y_position] = '@';
    pc_dungeon[pc_x_position][pc_y_position] = '@';
}

void Dungeon::remake_dungeon(void)
{
    init_dungeon();
    add_rooms();
    add_corridors();
    init_hardness();
    place_pc();
    place_stairs();
    pc_character = '@';
    win_status = 0;
    pc_dead_status = 0;
    turn = 0;
    pc_last_position = '.';
    teleport_state = 0;
    fog_state = 1;

}

void Dungeon::init_dungeon()
{   
       for(int i = 0; i < DUNGEON_ROWS; i++)
       {
           for(int j = 0; j < DUNGEON_COLUMNS; j++)
           {
               dungeon[i][j] = ' ';
               dungeon[0][j] = '-';
               dungeon[DUNGEON_ROWS - 1][j] = '-';
               pc_dungeon[i][j] = ' ';
               pc_dungeon[0][j] = '-';
               pc_dungeon[DUNGEON_ROWS - 1][j] = '-';
           }
   
           dungeon[i][0] = '|';
           dungeon[i][DUNGEON_COLUMNS - 1] = '|';
           pc_dungeon[i][0] = '|';
           pc_dungeon[i][DUNGEON_COLUMNS - 1] = '|';
       }
}

void Dungeon::add_rooms()
{
   int room_counter;
   int start_row, start_column;
   int rows, columns;
   int i, j;

   room_counter = 0;

   srand(time(NULL));

   while(room_counter < NUM_ROOMS)
   {
       int success = 1;

       //random row from 3 - 9
       rows = (rand() % 7) + 3;


       //random col from 3 - 9
       columns = (rand() % 7) + 3;

       //random row from 1 - 12
       start_row = (rand() % 11) + 1;

       //random col from 1 - 71
       start_column = (rand() % 70) + 1;

       success = check_rooms(rows, columns, start_row, start_column);

       /*
        * Add room to the dungeon & then increment the room counter
        */
       if(success)
       {
           for(i = start_row; i < (start_row + rows); i++)
           {
               for(j = start_column; j < (start_column + columns); j++)
               {
                   dungeon[i][j] = '.';
               }
           }

           rooms[room_counter][0] = rows;
           rooms[room_counter][1] = columns;
           rooms[room_counter][2] = start_row;
           rooms[room_counter][3] = start_column;

           room_counter++;
       }

   }

   sort_rooms();
}

void Dungeon::add_corridors()
{
   int i, j, room;
       for(room = 0; room < NUM_ROOMS-1; room++)
       {
           int room1_rows = rooms[room][0];
           int room1_columns = rooms[room][1];
           int room1_start_row = rooms[room][2];
           int room1_start_column = rooms[room][3];
   
           int room2_rows = rooms[room+1][0];
           int room2_columns = rooms[room+1][1];
           int room2_start_row = rooms[room+1][2];
           int room2_start_column = rooms[room+1][3];
   
           int room1_mid_x = (room1_rows/2) + room1_start_row;
           int room1_mid_y = (room1_columns/2) + room1_start_column;
   
           //printf("room 1 mid x: %d, room 1 mid y: %d\n", room1_mid_x, room1_mid_y);
   
           int room2_mid_x = (room2_rows/2) + room2_start_row;
           int room2_mid_y = (room2_columns/2) + room2_start_column;
   
           //printf("room 2 mid x: %d, room 2 mid y: %d\n", room2_mid_x, room2_mid_y);
   
           if((room2_mid_x - room1_mid_x) < 0)
           {
               for(i =  room1_mid_x; i > room2_mid_x - 1; i --)
               {
            if(dungeon[i][room1_start_column + room1_columns] != '.')
         {
           dungeon[i][room1_start_column + room1_columns] = '#';
         }
                   
               }
           }
           else
           {
               for(i = room1_mid_x; i < room2_mid_x + 1; i++)
               {
            if(dungeon[i][room1_start_column + room1_columns] != '.')
         {
            dungeon[i][room1_start_column + room1_columns] = '#';
         }
                  
               }
           }
   
           if((room2_mid_y - room1_mid_y) < 0)
           {
               for(j = (room1_start_column + room1_columns); j > (room2_start_column) ; j--)
               {
            if(dungeon[room2_mid_x][j] != '.')
         {
            dungeon[room2_mid_x][j] = '#';
         }
                  
               }
           }
           else
           {
               for(j = (room1_start_column + room1_columns); j < room2_start_column; j++)
               {
            if(dungeon[room2_mid_x][j]  != '.')
         {
           dungeon[room2_mid_x][j] = '#';
         }
                   
               }
           }
   
       }
}

void Dungeon::init_hardness()
{
   srand(time(NULL));
   
       int rows, cols;
   
       for(rows = 0; rows < DUNGEON_ROWS; rows++)
       {
           for(cols = 0; cols < DUNGEON_COLUMNS; cols++)
           {
               //if we get to a point where it is a wall, corridor, or something edge then hardness is 0
               //else it is a random hardness from 1-255
               if(dungeon[rows][cols] == '.' || dungeon[rows][cols] == '#' || dungeon[rows][cols] == '|' || dungeon[rows][cols] == '-')
               {
                   hardness[rows][cols] = 0;
               }
               else
               {
                   int hard = (rand() % 254) + 1;
                   hardness[rows][cols] = hard;
               }
           }
       }
}

void Dungeon::place_pc()
{
   srand(time(NULL));
   
       //Picks a random room & a random spot in the room
       //Places the PC in that random position in the dungeon
       int room = (rand() % NUM_ROOMS);
       
       int room_rows = rooms[room][0];
       int room_cols = rooms[room][1];
       int room_start_row = rooms[room][2];
       int room_start_col = rooms[room][3];
   
       int temp1 = (room_start_row + room_rows) -1;
       int temp2 = (room_start_col + room_cols) -1;
       
       int temp3 = room_start_row - temp1;
       int temp4 = room_start_col - temp2;
   
       int pc_start_row = (rand() % temp3) + room_start_row;
       int pc_start_col = (rand() % temp4) + room_start_col;
   
       pc_x_position = pc_start_row;
       pc_y_position = pc_start_col;
       
       dungeon[pc_start_row][pc_start_col] = pc_character;
}

void Dungeon::place_stairs()
{
   srand(time(NULL));
   
   int num_stairs = (rand() % NUM_ROOMS) + 1;
   int i;

   for(i = 0; i < num_stairs; i++)
   {
       int room = (rand() % NUM_ROOMS);
       
       int room_rows = rooms[room][0];
       int room_cols = rooms[room][1];
       int room_start_row = rooms[room][2];
       int room_start_col = rooms[room][3];
   
       int temp1 = (room_start_row + room_rows) -1;
       int temp2 = (room_start_col + room_cols) -1;
       
       int temp3 = room_start_row - temp1;
       int temp4 = room_start_col - temp2;
   
       int stair_x = (rand() % temp3) + room_start_row;
       int stair_y = (rand() % temp4) + room_start_col;
       
       if(dungeon[stair_x][stair_y] == '.')
       {
           if(rand() % 2 == 0)
           {
               dungeon[stair_x][stair_y] = '>';
           }
           else{
               dungeon[stair_x][stair_y] = '<';
           }
       }
   }
}

int Dungeon::check_rooms(int rows, int columns, int start_row, int start_column)
{
   int i = 0;
   int j = 0;

   //checks to see if there is nothing above room
   for(i = (start_row - 3); i < start_row; i++)
   {
       for(j = start_column; j < (start_column + columns); j++)
       {
           if(dungeon[i][j] == '.')
           {
               return 0;
           }
       }
   }


   //checks to see if there is nothing below room

   for(i = (start_row + rows); i < (start_row + rows + 3); i++)
   {
       for(j = start_column; j < (start_column + columns); j ++)
       {
           if(dungeon[i][j] == '.')
           {
               return 0;
           }
       }
   }


   //checks to see if there is nothing to the right
   for(i = start_row; i < (start_row + rows); i++)
   {
       for(j = (start_column + columns); j < (start_column + columns + 3); j++)
       {
           if(dungeon[i][j] == '.')
           {
               return 0;
           }
       }
   }

   //checks to see if there is nothing to the left
   for(i = start_row; i < (start_row + rows); i++)
   {
       for(j = (start_column - columns); j < start_column; j++)
       {
           if(dungeon[i][j] == '.')
           {
               return 0;
           }
       }
   }


   return 1;
}

void Dungeon::sort_rooms(void)
{
   int i, j;
   
       for(i = 0; i < NUM_ROOMS - 1; i++)
       {
           int min_indx = i;
   
           for(j = i+1; j < NUM_ROOMS; j++)
           {
               int room1_start_y = rooms[min_indx][3];
               int room2_start_y = rooms[j][3];
   
               if(room2_start_y < room1_start_y)
               {
                   min_indx = j;
               }
   
           }
   
           int temp_rows = rooms[min_indx][0];
           int temp_cols = rooms[min_indx][1];
           int temp_start_row = rooms[min_indx][2];
           int temp_start_cols = rooms[min_indx][3];
   
           rooms[min_indx][0] = rooms[i][0];
           rooms[min_indx][1] = rooms[i][1];
           rooms[min_indx][2] = rooms[i][2];
           rooms[min_indx][3] = rooms[i][3];
   
           rooms[i][0] = temp_rows;
           rooms[i][1] = temp_cols;
           rooms[i][2] = temp_start_row;
           rooms[i][3] = temp_start_cols;
       }
}

void Dungeon::print_game_status(void)
{
    if(win_status == 1)
    {
        std::cout << "YOU WON!!" << std::endl;
        printf("YOU WON!!\n");
    }
    else if(win_status == 0)
    {
        std::cout <<"YOU WERE EATEN BY A MONSTER! TRY AGAIN!!" << std::endl;
        printf("YOU WERE EATEN BY A MONSTER! TRY AGAIN!!\n");
    }
    else 
    {
        std::cout << "Exiting the game" << std::endl;
        printf("Exiting the game\n");
    }
}

void Dungeon::move_pc(int dir)
{
    if(dir == UPPER_LEFT)
    {
        if(teleport_state)
        {
            if(dungeon[pc_x_position-1][pc_y_position-1] != '|' && dungeon[pc_x_position-1][pc_y_position-1] != '-')
            {
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                //pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position - 1][pc_y_position - 1];
                dungeon[pc_x_position - 1][pc_y_position - 1] = pc_character;
                pc_x_position = pc_x_position - 1;
                pc_y_position = pc_y_position - 1;
            }
        }
        else{
            if(dungeon[pc_x_position-1][pc_y_position-1] != ' ' && dungeon[pc_x_position-1][pc_y_position-1] != '|' && dungeon[pc_x_position-1][pc_y_position-1] != '-' && !teleport_state)
            {
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position - 1][pc_y_position - 1];
                dungeon[pc_x_position - 1][pc_y_position - 1] = pc_character;
                pc_x_position = pc_x_position - 1;
                pc_y_position = pc_y_position - 1;
            }
        }
    }
    else if(dir == UPPER_RIGHT)
    {

        if(teleport_state)
        {
            if(dungeon[pc_x_position-1][pc_y_position+1] != '|' && dungeon[pc_x_position-1][pc_y_position+1] != '-')
            {
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                //pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position - 1][pc_y_position + 1];
                dungeon[pc_x_position - 1][pc_y_position + 1] = pc_character;
                pc_x_position = pc_x_position - 1;
                pc_y_position = pc_y_position + 1;
            }
        }
        else{
         
            if(dungeon[pc_x_position-1][pc_y_position+1] != ' ' && dungeon[pc_x_position-1][pc_y_position+1] != '|' && dungeon[pc_x_position-1][pc_y_position+1] != '-' && !teleport_state)
            {    
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position - 1][pc_y_position + 1];
                dungeon[pc_x_position - 1][pc_y_position + 1] = pc_character;
                pc_x_position = pc_x_position - 1;
                pc_y_position = pc_y_position + 1;
            }   
        }
        
    }
    else if(dir == UP)
    {

        if(teleport_state)
        {
            if(dungeon[pc_x_position-1][pc_y_position] != '|' && dungeon[pc_x_position-1][pc_y_position] != '-')
            {

                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                //pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position - 1][pc_y_position];
                dungeon[pc_x_position - 1][pc_y_position] = pc_character;
                pc_x_position = pc_x_position - 1;
            }
        }
        else
        {       
            if(dungeon[pc_x_position-1][pc_y_position] != ' ' && dungeon[pc_x_position-1][pc_y_position] != '|' && dungeon[pc_x_position-1][pc_y_position] != '-' && !teleport_state)
            {
                    
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position - 1][pc_y_position];
                dungeon[pc_x_position - 1][pc_y_position] = pc_character;
                pc_x_position = pc_x_position - 1;
            }   
        }    
    }
    else if(dir == LOWER_LEFT)
    {

        if(teleport_state)
        {
            if(dungeon[pc_x_position+1][pc_y_position-1] != '|' && dungeon[pc_x_position+1][pc_y_position-1] != '-')
            {
                        
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                //pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position+1][pc_y_position-1];
                dungeon[pc_x_position + 1][pc_y_position - 1] = pc_character;
                pc_x_position = pc_x_position + 1;
                pc_y_position = pc_y_position - 1;
            }
        }
        else
        {
            if(dungeon[pc_x_position+1][pc_y_position-1] != ' ' && dungeon[pc_x_position+1][pc_y_position-1] != '|' && dungeon[pc_x_position+1][pc_y_position-1] != '-'&& !teleport_state)
            {
                    
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position+1][pc_y_position-1];
                dungeon[pc_x_position + 1][pc_y_position - 1] = pc_character;
                pc_x_position = pc_x_position + 1;
                pc_y_position = pc_y_position - 1;
            }
        }
    }
    else if(dir == LOWER_RIGHT)
    {

        if(teleport_state)
        {
            
            if(dungeon[pc_x_position+1][pc_y_position+1] != '|' && dungeon[pc_x_position+1][pc_y_position+1] != '-')
            {
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                //pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position + 1][pc_y_position + 1];
                dungeon[pc_x_position + 1][pc_y_position + 1] = pc_character;
                pc_x_position = pc_x_position + 1;
                pc_y_position = pc_y_position + 1;
            }
        }
        else{
            if(dungeon[pc_x_position+1][pc_y_position+1] != ' ' && dungeon[pc_x_position+1][pc_y_position+1] != '|' && dungeon[pc_x_position+1][pc_y_position+1] != '-' && !teleport_state)
            {
                    
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position + 1][pc_y_position + 1];
                dungeon[pc_x_position + 1][pc_y_position + 1] = pc_character;
                pc_x_position = pc_x_position + 1;
                pc_y_position = pc_y_position + 1;
            }   
        }    
    }
    else if(dir == DOWN)
    {

        if(teleport_state)
        {
            if(dungeon[pc_x_position+1][pc_y_position] != '|' && dungeon[pc_x_position+1][pc_y_position] != '-')
            {    
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                //pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position + 1][pc_y_position];
                dungeon[pc_x_position + 1][pc_y_position] = pc_character;
                pc_x_position = pc_x_position + 1;
            }
        }
        else
        {
            if(dungeon[pc_x_position+1][pc_y_position] != ' ' && dungeon[pc_x_position+1][pc_y_position] != '|' && dungeon[pc_x_position+1][pc_y_position] != '-'&& !teleport_state)
            {
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position + 1][pc_y_position];
                dungeon[pc_x_position + 1][pc_y_position] = pc_character;
                pc_x_position = pc_x_position + 1;
            }   
        }
    }
    else if(dir == LEFT)
    {
        if(teleport_state)
        {

            if(dungeon[pc_x_position][pc_y_position-1] != '|' && dungeon[pc_x_position][pc_y_position-1] != '-')
            {
                        
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                //pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position][pc_y_position - 1];
                dungeon[pc_x_position][pc_y_position - 1] = pc_character;
                pc_y_position = pc_y_position - 1;
            }
        }
        else
        {
            if(dungeon[pc_x_position][pc_y_position-1] != ' ' && dungeon[pc_x_position][pc_y_position-1] != '|' && dungeon[pc_x_position][pc_y_position-1] != '-'&& !teleport_state)
            {
                    
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position][pc_y_position - 1];
                dungeon[pc_x_position][pc_y_position - 1] = pc_character;
                pc_y_position = pc_y_position - 1;
            }
        }    
    }
    else if(dir == RIGHT)
    {

        if(teleport_state)
        {

            if(dungeon[pc_x_position][pc_y_position+1] != '|' && dungeon[pc_x_position][pc_y_position+1] != '-')
            {           
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                //pc_dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position][pc_y_position + 1];
                dungeon[pc_x_position][pc_y_position + 1] = pc_character;
                pc_y_position = pc_y_position + 1;
            }
        }
        else{
            if(dungeon[pc_x_position][pc_y_position+1] != ' ' && dungeon[pc_x_position][pc_y_position+1] != '|' && dungeon[pc_x_position][pc_y_position+1] != '-'&& !teleport_state)
            {
                dungeon[pc_x_position][pc_y_position] = pc_last_position;
                pc_last_position = dungeon[pc_x_position][pc_y_position + 1];
                dungeon[pc_x_position][pc_y_position + 1] = pc_character;
                pc_y_position = pc_y_position + 1;
            }
        }    
    }
}

void Dungeon::update_pc_dungeon(void)
{
    for(int i = pc_x_position - 1; i < pc_x_position + 2; i++)
    {
        for(int j = pc_y_position - 1; j < pc_y_position + 2; j++)
        {
            if(pc_dungeon[i][j] != dungeon[i][j])
            {
                pc_dungeon[i][j] = dungeon[i][j];
            }
        }
    }
}
