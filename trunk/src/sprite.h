#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>
#include <boost/unordered_map.hpp>
#include "animationManager.h"

class Sprite {
public:

    Sprite();
    ~Sprite();

    // Adds the given animation with the given name. The first animation to be
    // added in this way is set as the default animation.
    void AddAnimation(const std::string& name, int animationId);

    // Set the default animation. The default animation is played whenever
    // no one-shot animation is playing.
    void SetDefaultAnimation(const std::string& name);

    // Plays the given animation as a one-shot the specified number of times.
    // Will return to the set default animation afterwards.
    void PlayAnimation(const std::string& name, int repetitionCount = 1);

    // Create a new sprite from this one, deep-copying animations.
    Sprite* Clone() const;

    AnimationManager::weak_ptr GetCurrentAnimation() const;

    // Don't call this.
    void RepetitionCallback();

private:

    typedef boost::unordered_map<std::string, int> AnimationMap;
    AnimationMap _map;

    std::string _defaultString;
    int _defaultAnim;
    int _oneShotAnim;
    int _oneShotReps;

};

#endif