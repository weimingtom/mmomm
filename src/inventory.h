#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <vector>

#include "item.h"

typedef std::vector<Item> itemmap;

class Inventory
{
public:
    Inventory();
    Inventory(itemmap items);
    ~Inventory();

    void    addItem(Item);
    void    removeItem(Item);
    void    removeItem(unsigned id);

    Item    getItem(unsigned id);
    itemmap getItems();
    itemmap getItemsByName(std::string name);
    itemmap getItemsByNameAndMinimumQuality(std::string name, unsigned minQuality); //ridiculously large function name. Suggestions?
    itemmap getItemsByNameAndMaximumQuality(std::string name, unsigned maxQuality); //ridiculously large function name. Suggestions?

private:
    static Inventory *_current;
    itemmap _items;
};

#endif
