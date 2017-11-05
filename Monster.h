#ifndef MONSTER_H
#define MONSTER_H

class Monster
{
public:
   Monster(void);
   
   char symbol;
   int x_position;
   int y_position;
   int alive;
   int room;
   int speed;
   int attrib;
   int move;
};

#endif