#ifndef ITEM_H_
#define ITEM_H_

#include <boost/shared_ptr.hpp>
#include "image.h"
#include "imageManager.h"

class Item
{
public:
    Item(unsigned id, std::string name, unsigned quality, ImageManager::shared_ptr image);

    unsigned                    getId()         { return _id; }
    std::string                 getName()       { return _name; }
    unsigned                    getQuality()    { return _quality; }
    ImageManager::shared_ptr    getImage()      { return _image; }

private:
    unsigned                    _id;
    std::string                 _name;
    unsigned                    _quality;
    ImageManager::shared_ptr    _image;
};

#endif
