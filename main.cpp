#include <stdlib.h>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include "Dungeon.h"

#define DELAY 30000
#define EVENT_CONSTANT 1000
#define END_X 81
#define END_Y 23


Dungeon main_dungeon;

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

int main(int argc, char* argv[])
{
    /**
     * TODO read from file 
     * and parse the monsters
     */

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
      /* else if(ch == 'm')
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
       }*/
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
             //init_monsters(monster_arr, num_monsters);
             //place_monsters(monster_arr, num_monsters);
           }
       }
       else if (ch == '>' && !main_dungeon.list_monster_state)
       {
           if(main_dungeon.pc_last_position == '>')
           {
             wclear(dungeon_win);
             //wclear(message_win);
             main_dungeon.remake_dungeon();
             //init_monsters(monster_arr, num_monsters);
             //place_monsters(monster_arr, num_monsters); 
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

    main_dungeon.print_game_status();
    endwin();
    return 0;
   
}