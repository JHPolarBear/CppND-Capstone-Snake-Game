#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

#include "item.h"
#include "score.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;

  // list of items
  std::vector<shared_ptr<Item>> vecItem;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h; 

  // random pool for speed reset item cycle 
  std::uniform_int_distribution<int> random_item_cycle;  

  // time cycle for speed reset item;
  int nItemCycle{999};
  Uint32 item_cycle_start{0};
  Uint32 item_cycle_end{0};

  // create items at a beginning of program
  void InitItemList();

  // Get random position where snake and items are not both exist
  void GetRandomEmptyPos(int&x, int& y);

  // check wheter the input location is occupied by items.
  bool IsItemExistsInPos(int x, int y);

  // create item by item type
  shared_ptr<Item> CreateItem(E_ITEM_TYPE eItemType);

  void Update();
};

#endif