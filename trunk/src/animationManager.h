#ifndef ANIMATION_MANAGER_H_
#define ANIMATION_MANAGER_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <vector>
#include "animation.h"

using namespace std;

typedef boost::unordered_map<int, boost::shared_ptr<Animation> > animap;

class AnimationManager
{
private:
    static AnimationManager *_current;
    animap                   _activeAnimations;
    animap                   _inactiveAnimations;
    int                      _idCounter;

public:
    typedef boost::shared_ptr<Animation> shared_ptr;
    typedef boost::weak_ptr<Animation>   weak_ptr;

    AnimationManager();
    ~AnimationManager();

    static AnimationManager& current() { assert(_current); return *_current; }
    static void setCurrent(AnimationManager *current) { _current = current; }

    weak_ptr            getAnimation(int id, bool reverse = false);
    vector<weak_ptr>    getActiveAnimations();
    int                 createAnimation(ImageManager::shared_ptr img, int frameWidth, int frameHeight,
                                        int interval, int startFrame = 0, bool active = true);
    int                 createNewInstanceOf(int id, int interval = -1, int startFrame = -1, int active = -1);
    void                deleteAnimation(int id);
    void                update(unsigned msPassed);
};

#endif
