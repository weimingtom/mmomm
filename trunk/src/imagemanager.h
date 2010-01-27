#ifndef IMAGE_MANAGER_H_
#define IMAGE_MANAGER_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "image.h"

using namespace std;
using namespace boost;

typedef unordered_map<string, shared_ptr<Image> > imgmap;

class ImageManager
{
private:
    static ImageManager *_current;
    imgmap images;

public:
    ImageManager();
    ~ImageManager();

    static ImageManager& current() { assert(_current); return *_current; }
    static void setCurrent(ImageManager *current) { _current = current; }

    weak_ptr<Image> getImage(string filename);
    void reloadImages();
    void use_count();
};

#endif
