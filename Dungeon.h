#ifndef DUNGEON_H
#define DUNGEON_H

#define DUNGEON_ROWS 21
#define DUNGEON_COLUMNS 80
#define NUM_ROOMS 6
#define ROOM_CHARS 4
#define UPPER_LEFT 7
#define UPPER_RIGHT 9
#define LOWER_LEFT 1
#define LOWER_RIGHT 3
#define LEFT 4
#define RIGHT 6
#define UP 8
#define DOWN 2

class Dungeon{
public:
   Dungeon(void);
   
   void print_game_status(void);
   void move_pc(int dir);
   void update_pc_dungeon(void);
   void remake_dungeon(void);
   void place_pc_randomly(void);

   char pc_dungeon[DUNGEON_ROWS][DUNGEON_COLUMNS];
   char dungeon[DUNGEON_ROWS][DUNGEON_COLUMNS];
   char rooms[NUM_ROOMS][ROOM_CHARS];
   char pc_last_position;
   char pc_character;

   int hardness[DUNGEON_ROWS][DUNGEON_COLUMNS];
   int pc_x_position;
   int pc_y_position;
   int win_status;
   int pc_dead_status;
   int turn;
   int teleport_state;
   int fog_state;
   int monster_arr_size;
   int list_monster_state;

private:
   void init_dungeon(void);
   void add_rooms(void);
   void add_corridors(void);
   void init_hardness(void);
   void place_pc(void);
   void place_stairs(void);
   int check_rooms(int rows, int columns, int start_row, int start_column);
   void sort_rooms(void);
   void print_dungeon(void);
};

#endif