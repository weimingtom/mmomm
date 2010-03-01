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
    if(reverse) {
        animap::iterator it = _inactiveAnimations.find(id);
        if(it == _inactiveAnimations.end()) {
            animap::iterator it = _activeAnimations.find(id);
            assert(it != _activeAnimations.end());
            return _activeAnimations[id];
        } else {
            return _inactiveAnimations[id];
        }
    } else {
        animap::iterator it = _activeAnimations.find(id);
        if(it == _activeAnimations.end()) {
            animap::iterator it = _inactiveAnimations.find(id);
            assert(it != _inactiveAnimations.end());
            return _inactiveAnimations[id];
        } else {
            return _activeAnimations[id];
        }
    }
}

int AnimationManager::createAnimation(ImageManager::shared_ptr img, int frameWidth, int frameHeight,
                                      double interval, int startFrame, int endFrame, bool active)
{
    _idCounter++;
    shared_ptr anim = shared_ptr(new Animation(_idCounter, img, frameWidth, frameHeight, interval, startFrame, endFrame, active));

    if(active)
        _activeAnimations.insert(pair<int, shared_ptr>(_idCounter, anim));
    else
        _inactiveAnimations.insert(pair<int, shared_ptr>(_idCounter, anim));

    return _idCounter;
}

int AnimationManager::createNewInstanceOf(int id, double interval, int startFrame, int endFrame, int active)
{
    double      actualInterval  = interval;
    int         actualStart     = startFrame;
    int         actualEnd       = endFrame;
    bool        actualActive    = (active != 0);
    Animation  *anim            = getAnimation(id, !active).get();

    if(interval == -1)
        actualInterval = anim->getInterval();

    if(startFrame == -1)
        actualStart = anim->getStartFrame();

    if(endFrame == -1)
        actualEnd = anim->getEndFrame();

    if(active == -1)
        actualActive = anim->isActive();

    return createAnimation(anim->getImage(), anim->getFrameWidth(), anim->getFrameHeight(),
                           actualInterval, actualStart, actualEnd, actualActive);
}

void AnimationManager::deleteAnimation(int id)
{
    _activeAnimations.erase(id);
    _inactiveAnimations.erase(id);
}

void AnimationManager::update(double elapsed)
{
    for(animap::iterator it = _activeAnimations.begin(); it != _activeAnimations.end(); it++) {
        (*it).second.get()->update(elapsed);
    }
}

vector<AnimationManager::weak_ptr> AnimationManager::getActiveAnimations()
{
    vector<weak_ptr> anims;
    for(animap::iterator it = _activeAnimations.begin(); it != _activeAnimations.end(); it++) {
        anims.push_back((*it).second);
    }
    return anims;
}

void AnimationManager::setAnimationActive(int id, bool active)
{
    animap::iterator i = _activeAnimations.find(id);
    animap::iterator j = _inactiveAnimations.find(id);
    if ( i == _activeAnimations.end() && !active ) {
        j->second->setActive(false);
        return;
    }
    if ( j == _inactiveAnimations.end() && active ) {
        i->second->setActive(true);
        return;
    }
    if ( active ) {
        _activeAnimations[id] = j->second;
        _inactiveAnimations.erase(id);
    }
    else {
        _inactiveAnimations[id] = i->second;
        _activeAnimations.erase(id);
    }
}
