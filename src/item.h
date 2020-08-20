#pragma once

#include "SDL.h"
#include "snake.h"
#include "score.h"

#define _INVALID_ITEM_ID -1

enum E_ITEM_TYPE
{
    E_ITEM_NONE,

    E_ITEM_FOOD,            // item which add score and grows snake's size&speed
    E_ITEM_SPEED_RESET,     // item which reset snake's speed

    E_ITEM_MAX,
};

class Item
{
public:
    Item();
    Item(int id);
    Item(int id, E_ITEM_TYPE eType);

    ////    Getter&Setter     ////
    void SetID(int id)  {_id == id;}
    int GetID()         {return _id;}

    void SetPosX(int _val)  {_pos.x = _val;}
    int GetPosX()          {return _pos.x;}

    void SetPosY(int _val)  {_pos.y = _val;}
    int GetPosY()          {return _pos.y;}

    E_ITEM_TYPE GetType()   {return _eType;}

    ////    Methods     ////

    // set item's position
    void Place(int x, int y);

    // Perform specific work to the snake  : pure virtual function  
    virtual void Activate(Snake& snake) = 0;

private:
    // unique id
    int _id;

    // Type of item
    E_ITEM_TYPE _eType;

    // Item coordinate on screen
    SDL_Point _pos;
};

// Food item : only override activate function
class ItemFood: public Item
{
    using Item::Item;
public:
    void Activate(Snake& snake);
    virtual ~ItemFood() {}
};

// Speed reset item: only override activate function
class ItemReset: public Item
{
    using Item::Item;
public:
    void Activate(Snake& snake);
    virtual ~ItemReset() {}
};