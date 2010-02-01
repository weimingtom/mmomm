#ifndef CLIENTSPRITES_H_
#define CLIENTSPRITES_H_

#include <boost/unordered_map.hpp>
class Sprite;

// Define sprites
#define SPRITE_LIST \
    SPRITE( FIGHTER ) \
        ANIMATION( "walk-down",  "001-Fighter01.png", 32, 48, .25,  0,  4 ) \
        ANIMATION( "walk-left",  "001-Fighter01.png", 32, 48, .25,  4,  8 ) \
        ANIMATION( "walk-right", "001-Fighter01.png", 32, 48, .25,  8, 12 ) \
        ANIMATION( "walk-up",    "001-Fighter01.png", 32, 48, .25, 12, 16 )

class ClientSprites {
public:

    enum SpriteType {
#define SPRITE( name ) name,
#define ANIMATION( name, image, width, height, time, start, end )
        SPRITE_LIST
#undef SPRITE
#undef ANIMATION
    };

    // Create standard sprites.
    static void Setup();
    // Call at the end.
    static void Delete();

    // Creates a new copy of the specified standard sprite.
    // Need to delete.
    static Sprite* Get(SpriteType sprite);

private:

    typedef boost::unordered_map< SpriteType, Sprite* > MapType;
    static MapType _templates;

};

#endif
