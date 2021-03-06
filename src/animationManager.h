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

    shared_ptr          getAnimation(int id, bool reverse = false);
    vector<weak_ptr>    getActiveAnimations();
    // Start frame is inclusive, end frame exclusive. Default value of endFrame (-1) sets to all possible frames.
    int                 createAnimation(ImageManager::shared_ptr img, int frameWidth, int frameHeight,
                                        double interval, int startFrame = 0, int endFrame = -1, bool active = true);
    // Default values (-1) indicate the value should be copied from existing animation.
    int                 createNewInstanceOf(int id, double interval = -1, int startFrame = -1, int endFrame = -1, int active = -1);
    void                deleteAnimation(int id);
    void                setAnimationActive(int id, bool active);
    void                update(double elapsed);
};

#endif
