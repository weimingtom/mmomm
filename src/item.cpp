#include "item.h"

Item::Item(unsigned id, std::string name, unsigned quality, ImageManager::shared_ptr image)
:   _id(id),
    _name(name),
    _quality(quality),
    _image(image)
{

}
