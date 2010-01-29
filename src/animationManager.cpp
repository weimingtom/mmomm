#include <boost/foreach.hpp>
#include "animationManager.h"

AnimationManager *AnimationManager::_current = 0;

AnimationManager::AnimationManager()
{
    _idCounter = 0;
}

AnimationManager::~AnimationManager()
{
}

AnimationManager::shared_ptr AnimationManager::getAnimation(int id)
{
    animap::iterator it = _activeAnimations.find(id);
    if(it == _activeAnimations.end())
    {
        animap::iterator it = _inactiveAnimations.find(id);
        assert(it != _activeAnimations.end());
        shared_ptr ptr_img(_inactiveAnimations[id]);
        return ptr_img;
    }
    else
    {
        shared_ptr ptr_img(_activeAnimations[id]);
        return ptr_img;
    }
}

int AnimationManager::createAnimation(ImageManager::shared_ptr img, int frameWidth, int frameHeight,
                                      int interval, int startFrame, bool active)
{
    _idCounter++;
    shared_ptr anim = shared_ptr(new Animation(img, frameWidth, frameHeight, interval, startFrame, active));

    if(active)
        _activeAnimations.insert(pair<int, shared_ptr>(_idCounter, anim));
    else
        _inactiveAnimations.insert(pair<int, shared_ptr>(_idCounter, anim));

    return _idCounter;
}

void AnimationManager::update(int msPassed)
{
    for(animap::iterator it = _activeAnimations.begin(); it != _activeAnimations.end(); it++)
    {
        (*it).second.get()->update(msPassed);
    }
}
