#include <stdlib.h>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "Dungeon.h"
#include "Monster.h"

#define DELAY 30000
#define EVENT_CONSTANT 1000
#define END_X 81
#define END_Y 23

Dungeon main_dungeon;
Monster *monster_arr = NULL;

void print_dungeon(WINDOW *win, int fog)
{
   for(int i = 0; i < DUNGEON_ROWS; i++)
   {
         for(int j = 0; j < DUNGEON_COLUMNS; j++)
         {
             if(fog)
             {
               wprintw(win, "%c", main_dungeon.pc_dungeon[i][j]);      
             }
             else 
             {
               wprintw(win, "%c", main_dungeon.dungeon[i][j]);
             }
         }
         wprintw(win, "\n");
   }
   wrefresh(win);
}

void place_monsters(Monster *arr, int size)
{
    srand(time(NULL));

    for(int index = 0; index < size; index++)
    {
        //Pick a random room
        int room = rand() % NUM_ROOMS;

        int room_rows = main_dungeon.rooms[room][0];
        int room_cols = main_dungeon.rooms[room][1];
        int room_start_row = main_dungeon.rooms[room][2];
        int room_start_col = main_dungeon.rooms[room][3];

        int temp1 = (room_start_row + room_rows) -1;
        int temp2 = (room_start_col + room_cols) -1;
        
        int temp3 = room_start_row - temp1;
        int temp4 = room_start_col - temp2;
    
        //Pick a random position in that room so long as there is nothing there
        int success = 0;
        while(!success)
        {
            int monster_start_row = (rand() % temp3) + room_start_row;
            int monster_start_col = (rand() % temp4) + room_start_col;

            if(main_dungeon.dungeon[monster_start_row][monster_start_col] == '.')
            {
                //place the character of the monster
                int characteristics = arr[index].attrib;
                arr[index].x_position = monster_start_row;
                arr[index].y_position = monster_start_col;
                arr[index].room = room;
                success = 1;

                switch (characteristics)
                {
                    case 0x0 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '0';
                    arr[index].symbol = '0';
                    break;
                    case 0x1 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '1';
                    arr[index].symbol = '1';
                    break;
                    case 0x2 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '2';
                    arr[index].symbol = '2';
                    break;
                    case 0x3 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '3';
                    arr[index].symbol = '3';
                    break;
                    case 0x4 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '4';
                    arr[index].symbol = '4';
                    break;
                    case 0x5 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '5';
                    arr[index].symbol = '5';
                    break;
                    case 0x6 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '6';
                    arr[index].symbol = '6';
                    break;
                    case 0x7 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '7';
                    arr[index].symbol = '7';
                    break;
                    case 0x8 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '8';
                    arr[index].symbol = '8';
                    break;
                    case 0x9 :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = '9';
                    arr[index].symbol = '9';
                    break;
                    case 0xa :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = 'a';
                    arr[index].symbol = 'a';
                    break;
                    case 0xb :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = 'b';
                    arr[index].symbol = 'b';
                    break;
                    case 0xc :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = 'c';
                    arr[index].symbol = 'c';
                    break;
                    case 0xd :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = 'd';
                    arr[index].symbol = 'd';
                    break;
                    case 0xe :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = 'e';
                    arr[index].symbol = 'e';
                    break;
                    case 0xf :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = 'f';
                    arr[index].symbol = 'f';
                    break;
                    default :
                    main_dungeon.dungeon[monster_start_row][monster_start_col] = 'g';
                    arr[index].symbol = 'g';
                }
            }
        }
    }
}

void init_monsters(Monster *arr, int size)
{
    srand(time(NULL));

    for (int index = 0; index < size; index++)
    {    
        int characteristics = rand() & 0xf;
        int speed = (rand() % 15) + 5;
        arr[index].attrib = characteristics;
        arr[index].speed = speed;
        arr[index].move = EVENT_CONSTANT / speed;
        arr[index].alive = 1;   
    }
}

int main (int argc, char* argv[])
{
   int arg_count = 1;
   int num_monsters = 0;
   int monster_index = 0;   

   if(argc < 3)
   {
       printf("Insufficient arguements\n");
       return -1;
   }

   while(arg_count < argc)
   {
       if(strcmp(argv[arg_count],  "--nummon") == 0)
       {
           arg_count++;
           num_monsters = atoi(argv[arg_count]);
           if(num_monsters <= 0)
           {
               printf("Incorrect argument passed\n");
               return -1;
           }

           //printf("Setting dungeon up for %d monsters\n", num_monsters);
           arg_count++;
       }
       else 
       {
           printf("incorrect argument detected: %s\n", argv[arg_count]);
           return -1;
       }
   }

   monster_arr = new Monster[num_monsters];

   init_monsters(monster_arr, num_monsters);
   place_monsters(monster_arr, num_monsters);

   WINDOW *dungeon_win;
   WINDOW *message_win;
   initscr();
   noecho();
   curs_set(FALSE);
   keypad(stdscr, TRUE);
   dungeon_win = newwin(END_Y, END_X, 1, 0);
   message_win = newwin(1, END_X, 0, 0);


   while(1)
   {

      
      wclear(dungeon_win);
      print_dungeon(dungeon_win, main_dungeon.fog_state);
      wrefresh(dungeon_win);

      int ch = getch();
      
      if(ch == 'Q' || ch == 'q')
      {
         main_dungeon.win_status = -1;
         break;
      }
      else if((ch == 'y' || ch == '7') && !main_dungeon.list_monster_state)
      {
         //Move the character to the upper left
         main_dungeon.move_pc(UPPER_LEFT);
      }
      else if((ch == 'k' || ch == '8') && !main_dungeon.list_monster_state)
      {
         //Move the character to the up
         main_dungeon.move_pc(UP);
      }
      else if((ch == 'u' || ch == '9') && !main_dungeon.list_monster_state)
      {
         //Move the character to the upper right
         main_dungeon.move_pc(UPPER_RIGHT);
      }
      else if((ch == 'l' || ch == '6') && !main_dungeon.list_monster_state)
      {
         //Move the character to the right
         main_dungeon.move_pc(RIGHT);
      }
      else if((ch == 'n' || ch == '3') && !main_dungeon.list_monster_state)
      {
         //Move the character lower right
         main_dungeon.move_pc(LOWER_RIGHT);
      }
      else if((ch == 'j' || ch == '2') && !main_dungeon.list_monster_state)
      {
         //Move the character down
         main_dungeon.move_pc(DOWN);
      }
      else if((ch == 'b' || ch == '1') && !main_dungeon.list_monster_state)
      {
         //Move the character to the lower left
         main_dungeon.move_pc(LOWER_LEFT);
      }
      else if((ch == 'h' || ch == '4') && !main_dungeon.list_monster_state)
      {
         //Move the character to the left
         main_dungeon.move_pc(LEFT);
      }
      else if((ch == ' ' || ch == '5') && !main_dungeon.list_monster_state)
      {
         //Don't move the character but move the monsters
      }
      else if(ch == 'm')
      {
          main_dungeon.list_monster_state = 1;
          monster_index = 0;
          wclear(message_win);
          int monsterx_pos = monster_arr[monster_index].x_position;
          int monstery_pos = monster_arr[monster_index].y_position;
          int difference_in_x = monster_arr[monster_index].x_position - main_dungeon.pc_x_position;
          int difference_in_y = monster_arr[monster_index].y_position - main_dungeon.pc_y_position;

          if(difference_in_x > 0 && difference_in_y > 0)
          {
            wprintw(message_win, "monster %c, is %d south and %d right", main_dungeon.dungeon[monsterx_pos][monstery_pos], abs(difference_in_x), abs(difference_in_y));
          }
          else if(difference_in_x < 0 && difference_in_y > 0)
          {
            wprintw(message_win, "monster %c, is %d north and %d right", main_dungeon.dungeon[monsterx_pos][monstery_pos], abs(difference_in_x), abs(difference_in_y));
            
          }
          else if(difference_in_x > 0 && difference_in_y < 0)
          {
            wprintw(message_win, "monster %c, is %d south and %d left", main_dungeon.dungeon[monsterx_pos][monstery_pos], abs(difference_in_x), abs(difference_in_y));
            
          }
          else if (difference_in_x < 0 && difference_in_y < 0)
          {
            wprintw(message_win, "monster %c, is %d north and %d left", main_dungeon.dungeon[monsterx_pos][monstery_pos], abs(difference_in_x), abs(difference_in_y));  
          }
          
          wrefresh(message_win);
      }
      else if(ch == 27)
      {
          main_dungeon.list_monster_state = 0;
          wclear(message_win);
          wrefresh(message_win);

      }
      else if(ch == '<' && !main_dungeon.list_monster_state)
      {
          if(main_dungeon.pc_last_position == '<')
          {
            wclear(dungeon_win);
            //wclear(message_win);
            main_dungeon.remake_dungeon();
            init_monsters(monster_arr, num_monsters);
            place_monsters(monster_arr, num_monsters);
          }
      }
      else if (ch == '>' && !main_dungeon.list_monster_state)
      {
          if(main_dungeon.pc_last_position == '>')
          {
            wclear(dungeon_win);
            //wclear(message_win);
            main_dungeon.remake_dungeon();
            init_monsters(monster_arr, num_monsters);
            place_monsters(monster_arr, num_monsters); 
          }
      }
      else if(ch == 't' && !main_dungeon.list_monster_state)
      {
          if(main_dungeon.teleport_state)
          {
              main_dungeon.teleport_state = 0;
              main_dungeon.fog_state = 1;
              main_dungeon.pc_character = '@';
              main_dungeon.dungeon[main_dungeon.pc_x_position][main_dungeon.pc_y_position] = main_dungeon.pc_character;
              //main_dungeon.pc_dungeon[main_dungeon.pc_x_position][main_dungeon.pc_y_position] = main_dungeon.pc_character;
          }
          else{
              main_dungeon.teleport_state = 1;
              main_dungeon.fog_state = 0;
              main_dungeon.pc_character = '*';
              main_dungeon.dungeon[main_dungeon.pc_x_position][main_dungeon.pc_y_position] = main_dungeon.pc_character;
              //main_dungeon.pc_dungeon[main_dungeon.pc_x_position][main_dungeon.pc_y_position] = main_dungeon.pc_character;
          }
      }
      else if(ch == 'r' && main_dungeon.teleport_state)
      {
        main_dungeon.place_pc_randomly();
        main_dungeon.teleport_state = 0;
        main_dungeon.fog_state = 1;
        main_dungeon.pc_character = '@';
      }
      else if(ch == 'f' && !main_dungeon.list_monster_state)
      {
         if(main_dungeon.fog_state)
         {
            main_dungeon.fog_state = 0;
         }
         else{
            main_dungeon.fog_state = 1;
         }
      }

      if(!main_dungeon.teleport_state)
      {
        main_dungeon.update_pc_dungeon();
      }
      
   }

   delete []monster_arr;
   main_dungeon.print_game_status();
   endwin();
   return 0;
}


