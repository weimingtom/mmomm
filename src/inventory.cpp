#include "inventory.h"

Inventory::Inventory()
{

}

Inventory::Inventory(itemmap items)
{
    _items = items;
}

Inventory::~Inventory()
{

}


void    Inventory::addItem(Item item)
{
    _items.push_back(item);
}

void    Inventory::removeItem(Item item)
{
    removeItem(item.getId());
}

void    Inventory::removeItem(unsigned id)
{
    for(unsigned i = 0; i < _items.size(); i++)
    {
        if(_items[i].getId() == id) {
            _items.erase(_items.begin() + i);
            break;
        }
    }
}


Item    Inventory::getItem(unsigned id)
{
    for(unsigned i = 0; i < _items.size(); i++)
    {
        if(_items[i].getId() == id)
            return _items[i];
    }

    throw "Item does not exist";
}

itemmap Inventory::getItems()
{
    return _items;
}

itemmap Inventory::getItemsByName(std::string name)
{
    itemmap items;

    for(unsigned i = 0; i < _items.size(); i++)
    {
        if(_items[i].getName() == name)
            items.push_back(_items[i]);
    }

    return items;
}

itemmap Inventory::getItemsByNameAndMinimumQuality(std::string name, unsigned minQuality)
{
    itemmap items;

    for(unsigned i = 0; i < _items.size(); i++)
    {
        if(_items[i].getName() == name && _items[i].getQuality() >= minQuality)
            items.push_back(_items[i]);
    }

    return items;
}

itemmap Inventory::getItemsByNameAndMaximumQuality(std::string name, unsigned maxQuality)
{
    itemmap items;

    for(unsigned i = 0; i < _items.size(); i++)
    {
        if(_items[i].getName() == name && _items[i].getQuality() <= maxQuality)
            items.push_back(_items[i]);
    }

    return items;
}
