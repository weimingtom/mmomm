#ifndef CLIENTANIMATIONS_H_
#define CLIENTANIMATIONS_H_

#include <boost/unordered_map.hpp>

// Define animations
#define ANIMATION_LIST \
    ANIMATION( FIGHTER, "001-Fighter01.png", 32, 48, .25 )

class ClientAnimations {
public:

    enum Animation {
#define ANIMATION( name, image, width, height, time ) name,
        ANIMATION_LIST
#undef ANIMATION
    };

    // Create standard animations.
    static void Setup();

    // Creates a new copy of the specified standard animation.
    // Use AnimationManager::getAnimation() on the returned id
    // to get an animation pointer; also need to call
    // AnimationManager::deleteAnimation() when you're done with it.
    static int Get(Animation animation);

private:

    static boost::unordered_map< Animation, int > _templates;

};

#endif
