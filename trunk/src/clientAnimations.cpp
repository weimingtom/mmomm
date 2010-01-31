#include "clientAnimations.h"
#include "animationManager.h"

boost::unordered_map< ClientAnimations::Animation, int > ClientAnimations::_templates;

void ClientAnimations::Setup()
{
    // Create standard animations
    ImageManager::shared_ptr img = ImageManager::current().getImage("001-Fighter01.png");
    _templates[ FIGHTER ] = AnimationManager::current().createAnimation(img, 32, 48, 250);
}

int ClientAnimations::Get(Animation animation)
{
    assert(_templates.find(animation) != _templates.end());
    return AnimationManager::current().createNewInstanceOf(_templates[animation]);
}