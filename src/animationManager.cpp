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

AnimationManager::shared_ptr AnimationManager::getAnimation(int id, bool reverse) //TODO: exception throwing?
{
    if(reverse)
    {
        animap::iterator it = _inactiveAnimations.find(id);
        if(it == _inactiveAnimations.end()) {
            animap::iterator it = _activeAnimations.find(id);
            assert(it != _activeAnimations.end());
            shared_ptr ptr_img(_activeAnimations[id]);
            return ptr_img;
        } else {
            shared_ptr ptr_img(_inactiveAnimations[id]);
            return ptr_img;
        }
    } else {
        animap::iterator it = _activeAnimations.find(id);
        if(it == _activeAnimations.end()) {
            animap::iterator it = _inactiveAnimations.find(id);
            assert(it != _inactiveAnimations.end());
            shared_ptr ptr_img(_inactiveAnimations[id]);
            return ptr_img;
        } else {
            shared_ptr ptr_img(_activeAnimations[id]);
            return ptr_img;
        }
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

int AnimationManager::createNewInstanceOf(int id, int interval, int startFrame, int active)
{
    int         actualInterval  = interval;
    int         actualFrame     = startFrame;
    bool        actualActive    = (active != 0);
    Animation  *anim            = getAnimation(id, !active).get();

    if(interval == -1)
        actualInterval = anim->getInterval();

    if(startFrame == -1)
        actualFrame = anim->getCurrentFrame();

    if(active == -1)
        actualActive = anim->isActive();

    return createAnimation(anim->getImage(), anim->getFrameWidth(), anim->getFrameHeight(),
                           actualInterval, actualFrame, actualActive);
}

void AnimationManager::deleteAnimation(int id)
{
    _activeAnimations.erase(id);
    _inactiveAnimations.erase(id);
}

void AnimationManager::update(unsigned msPassed)
{
    for(animap::iterator it = _activeAnimations.begin(); it != _activeAnimations.end(); it++) {
        (*it).second.get()->update(msPassed);
    }
}

vector<AnimationManager::shared_ptr> AnimationManager::getActiveAnimations()
{
    vector<shared_ptr> anims;
    for(animap::iterator it = _activeAnimations.begin(); it != _activeAnimations.end(); it++) {
        anims.push_back((*it).second);
    }
    return anims;
}
