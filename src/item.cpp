
#include <iostream>

#include "SDL.h"

#include "item.h"

Item::Item():
_id(_INVALID_ITEM_ID),
_eType(E_ITEM_NONE)
{  _pos.x = _pos.y = 0;  }

Item::Item(int id):
_eType(E_ITEM_NONE),
_id(id)
{  _pos.x = _pos.y = 0;  }

Item::Item(int id, E_ITEM_TYPE eType):
_eType(eType),
_id(id)
{  _pos.x = _pos.y = 0;  }

void Item::Place(int x, int y)
{
    _pos.x = x;
    _pos.y = y;
}

void ItemFood::Activate(Snake& snake)
{
    // Add Score
    Score::GetInstance().AddScore();

    // Grow sanke body
    snake.GrowBody();
    snake.speed += 0.02;
}

void ItemReset::Activate(Snake& snake)
{
    snake.speed = SNAKE_INIT_SPEED;
}

