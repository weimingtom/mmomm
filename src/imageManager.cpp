#include "imageManager.h"

#include <vector>

ImageManager *ImageManager::_current = 0;

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
}

ImageManager::shared_ptr ImageManager::getImage(string filename)
{
    imgmap::iterator it = _images.find(filename);
    if(it == _images.end()) {
        shared_ptr img(new Image(filename));
        shared_ptr wptr_img(img);
        _images.insert(pair<string, shared_ptr>(filename, img));
        return wptr_img;
    } else {
        shared_ptr wptr_img(_images[filename]);
        return wptr_img;
    }
}

void ImageManager::reloadImages()
{
    imgmap::iterator it;
    vector<string> scheduledForDeletion;
    for(it = _images.begin(); it != _images.end(); it++) {
        if((*it).second.unique())
            scheduledForDeletion.push_back((*it).first);
        else
            (*it).second.get()->reload();
    }

    for(unsigned i = 0; i < scheduledForDeletion.size(); i++) {
#ifndef NDEBUG
        cout << "Deleting: " << scheduledForDeletion[i] << endl;
#endif
        _images.erase(scheduledForDeletion[i]);
    }
}

#ifndef NDEBUG
void ImageManager::use_count()
{
    imgmap::iterator it;
    for(it = _images.begin(); it != _images.end(); it++) {
        cout << "use_count of \"" << (*it).first << "\": " << (*it).second.use_count() << endl;
    }
}
#endif
