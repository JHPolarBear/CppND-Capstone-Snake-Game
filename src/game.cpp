#include <iostream>
#include <thread>
#include <future>

#include "game.h"
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)),
      random_item_cycle(5, 25),
      item_cycle_start(0),
      item_cycle_end(0) {
  InitItemList();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;  

  // init item cycle to current ticks
  item_cycle_start = item_cycle_end = SDL_GetTicks();

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, vecItem);

    // upcate frame and item cycle ticks
    item_cycle_end = frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every half second, update the window title.
    if (frame_end - title_timestamp >= 500) {
      renderer.UpdateWindowTitle(Score::GetInstance().GetScore(), frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::InitItemList()
{
  vecItem.clear();

  // create food item
  shared_ptr<Item> item = CreateItem(E_ITEM_FOOD); 
  vecItem.push_back(item);

  // set initial time duration for speed reset item
  nItemCycle = random_item_cycle(engine);
}

// check wheter the input location is occupied by items.
bool Game::IsItemExistsInPos(int x, int y)
{
  for(auto item: vecItem)
  {
    if(item->GetPosX() == x && item->GetPosY() == y)
    {
      return true;
    }
  }

  return false;
}

// Get random position where snake and items are not both exist
void Game::GetRandomEmptyPos(int& _x, int& _y)
{
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    std::cout << "x: " << x << ", y: " << y << std::endl;

    if(x == random_w.max() || y == random_h.max())
      continue;

    // Check that the location is not occupied by a snake item before placing
    // Also Check that the location is not occupied by other items
    if (!snake.SnakeCell(x, y) && IsItemExistsInPos(x, y) == false) {
      _x = x;
      _y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check interaction with items
  auto item = std::begin(vecItem);

  while(item != std::end(vecItem))
  {
    if( (*item)->GetPosX() == new_x && (*item)->GetPosY() == new_y)
    { 
      // save current item type
      E_ITEM_TYPE eItemType = (*item)->GetType();

      //Perform item ability      
      (*item)->Activate(snake);

      //Erase Item;
      vecItem.erase(item);

      if(eItemType == E_ITEM_FOOD)
      {
        // create New food item immediately
        shared_ptr<Item> item = CreateItem(E_ITEM_FOOD);
        vecItem.push_back(item);
      }
      else
      {
        // Set new random count for speed reset item
        item_cycle_start = item_cycle_end;
        nItemCycle = random_item_cycle(engine);
      }
    }
    else
    {
      // reset item cycle if speed item exists
      if((*item)->GetType() == E_ITEM_SPEED_RESET)
      {
          item_cycle_start = item_cycle_end;
      }

      ++item;
    }
  }
  
  if(item_cycle_end - item_cycle_start >= nItemCycle*1000)
  {
    // create New food item immediately
    shared_ptr<Item> item = CreateItem(E_ITEM_SPEED_RESET);
    vecItem.push_back(item);
  }
}

// create item by item type
shared_ptr<Item> Game::CreateItem(E_ITEM_TYPE eItemType)
{
  switch(eItemType)
  {
    case E_ITEM_FOOD:
    {
      // create food item
      shared_ptr<ItemFood> food = make_shared<ItemFood>((int)E_ITEM_FOOD, E_ITEM_FOOD);

      int x , y = 0;
      GetRandomEmptyPos(x, y);
      food->Place(x, y); 

      return food;
    }
    break;
    case E_ITEM_SPEED_RESET:
    {
      // create food item
      shared_ptr<ItemReset> reset = make_shared<ItemReset>((int)E_ITEM_SPEED_RESET, E_ITEM_SPEED_RESET);

      int x , y = 0;
      GetRandomEmptyPos(x, y);
      reset->Place(x, y); 

      return reset;
    }
    break;

    default:
    {
      // create food item (default)
      shared_ptr<ItemFood> food = make_shared<ItemFood>((int)E_ITEM_FOOD, E_ITEM_FOOD);

      int x , y = 0;
      GetRandomEmptyPos(x, y);
      food->Place(x, y); 

      return food;
    }
    break;    
  }
 
}

int Game::GetSize() const { return snake.size; }