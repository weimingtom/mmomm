#include "clientAnimations.h"
#include "animationManager.h"

boost::unordered_map< ClientAnimations::Animation, int > ClientAnimations::_templates;

#define ANIMATION( name, image, width, height, time ) img = ImageManager::current().getImage(image);\
                                                      _templates[ name ] = AnimationManager::current().createAnimation(img, width, height, time);
void ClientAnimations::Setup()
{
    ImageManager::shared_ptr img;
    ANIMATION_LIST
}
#undef ANIMATION

int ClientAnimations::Get(Animation animation)
{
    assert(_templates.find(animation) != _templates.end());
    return AnimationManager::current().createNewInstanceOf(_templates[animation]);
}