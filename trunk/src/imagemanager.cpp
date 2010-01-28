#include "imagemanager.h"

#include <vector>

ImageManager *ImageManager::_current = 0;

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
}

ImageManager::weak_ptr ImageManager::getImage(string filename)
{
    imgmap::iterator it = images.find(filename);
    if(it == images.end()) {
        shared_ptr img(new Image(filename));
        weak_ptr wptr_img(img);
        images.insert(pair<string, weak_ptr >(filename, wptr_img));
        return wptr_img;
    } else {
        weak_ptr wptr_img(images[filename]);
        return wptr_img;
    }
}

void ImageManager::reloadImages()
{
    imgmap::iterator it;
    vector<string> scheduledForDeletion;
    for(it = images.begin(); it != images.end(); it++) {
        if((*it).second.use_count() == 0)
            scheduledForDeletion.push_back((*it).first);
        else
            (*it).second.get()->reload();
    }

    for(unsigned i = 0; i < scheduledForDeletion.size(); i++) {
        cout << "Deleting: " << scheduledForDeletion[i] << endl;
        images.erase(scheduledForDeletion[i]);
    }
}

void ImageManager::use_count()
{
    imgmap::iterator it;
    for(it = images.begin(); it != images.end(); it++) {
        cout << "stuff: " << (*it).second.use_count() << endl;
    }
}
