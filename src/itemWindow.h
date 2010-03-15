#ifndef ITEM_WINDOW_H_
#define ITEM_WINDOW_H_

#include <guichan.hpp>

#include "inventory.h"

class ItemWindow : public gcn::Window
{
public:
    ItemWindow(int x, int y, Inventory inventory);
    ~ItemWindow();

    void draw(gcn::Graphics* graphics);
private:
    Inventory                   _inventory;
    std::vector<gcn::Label*>    _labels;
};

#endif
