#include <stdlib.h>
#include <string.h>
#include "Monster.h"

Monster::Monster()
{
    symbol = '0';
    x_position = 0;
    y_position = 0;
    alive = 1;
    room = 0;
    speed = 0;
    attrib = 0;
    move = 0;
}

