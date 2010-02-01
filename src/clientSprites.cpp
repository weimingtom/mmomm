#include "clientSprites.h"
#include "animationManager.h"
#include "sprite.h"

boost::unordered_map< ClientSprites::SpriteType, Sprite* > ClientSprites::_templates;

#define SPRITE( name ) sprite = new Sprite(); \
    _templates[name] = sprite;
#define ANIMATION( name, image, width, height, time, start, end ) img = ImageManager::current().getImage(image); \
    anim = AnimationManager::current().createAnimation(img, width, height, time, start, end, false); \
    sprite->AddAnimation(name, anim); \
    AnimationManager::current().deleteAnimation(anim);
void ClientSprites::Setup()
{
    ImageManager::shared_ptr   img;
    Sprite*                    sprite;
    int                        anim;

    SPRITE_LIST
}
#undef SPRITE
#undef ANIMATION

void ClientSprites::Delete()
{
    for ( MapType::iterator i = _templates.begin(); i != _templates.end(); i++ ) {
        delete i->second;
    }
    _templates.clear();
}

Sprite* ClientSprites::Get(SpriteType sprite)
{
    assert(_templates.find(sprite) != _templates.end());
    return _templates[sprite]->Clone();
}
