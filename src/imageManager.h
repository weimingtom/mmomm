#ifndef IMAGE_MANAGER_H_
#define IMAGE_MANAGER_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "image.h"

using namespace std;

typedef boost::unordered_map<string, boost::shared_ptr<Image> > imgmap;

class ImageManager
{
private:
    static ImageManager *_current;
    imgmap               _images;

public:
    typedef boost::shared_ptr<Image> shared_ptr;

    ImageManager();
    ~ImageManager();

    static ImageManager& current() { assert(_current); return *_current; }
    static void setCurrent(ImageManager *current) { _current = current; }

    shared_ptr getImage(string filename);
    void reloadImages();

#ifndef NDEBUG
    void use_count();
#endif
};

#endif
