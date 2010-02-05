#include "sprite.h"
#include <boost/shared_ptr.hpp>

Sprite::Sprite()
: _defaultAnim(-1)
, _oneShotAnim(-1)
, _oneShotReps(0)
{
}

Sprite::~Sprite()
{
    for ( AnimationMap::const_iterator i = _map.begin(); i != _map.end(); i++ ) {
        AnimationManager::current().deleteAnimation(i->second);
    }
}

void Sprite::addAnimation(const std::string& name, int animationId)
{
    assert(_map.find(name) == _map.end());
    bool first = _map.size() == 0;
    if ( first ) {
        _defaultString = name;
        _defaultAnim = animationId;
    }
    _map[name] = AnimationManager::current().createNewInstanceOf(animationId, -1, -1, -1, first);
}

void Sprite::setDefaultAnimation(const std::string& name)
{
    AnimationMap::iterator i = _map.find(name);
    if ( i == _map.end() )
        return;

    if ( _oneShotReps == 0 && _defaultAnim >= 0 )
        AnimationManager::current().setAnimationActive(_defaultAnim, false);
    _defaultAnim = i->second;
    _defaultString = name;
    if ( _oneShotReps == 0 )
        AnimationManager::current().setAnimationActive(_defaultAnim, true);
}

void Sprite::playAnimation(const std::string& name, int repetitionCount)
{
    if ( repetitionCount < 1 )
        return;
    AnimationMap::iterator i = _map.find(name);
    if ( i == _map.end() )
        return;

    if ( _oneShotReps == 0 && _defaultAnim >= 0 )
        AnimationManager::current().setAnimationActive(_defaultAnim, false);
    if ( _oneShotReps > 0 ) {
        AnimationManager::current().setAnimationActive(_oneShotAnim, false);
        boost::shared_ptr<Animation>(AnimationManager::current().getAnimation(_oneShotAnim))->setCallbackSprite(0);
    }
    _oneShotReps = repetitionCount;
    _oneShotAnim = i->second;
    AnimationManager::current().setAnimationActive(_oneShotAnim, true);
    boost::shared_ptr<Animation>(AnimationManager::current().getAnimation(_oneShotAnim))->setCallbackSprite(this);
}

void Sprite::repetitionCallback()
{
    if ( _oneShotReps == 0 )
        return;
    if ( --_oneShotReps != 0 )
        return;
        
    AnimationManager::current().setAnimationActive(_oneShotAnim, false);
    boost::shared_ptr<Animation>(AnimationManager::current().getAnimation(_oneShotAnim))->setCallbackSprite(0);
    if ( _defaultAnim >= 0 )
        AnimationManager::current().setAnimationActive(_defaultAnim, true);
}

Sprite* Sprite::clone() const
{
    Sprite* r = new Sprite();
    for ( AnimationMap::const_iterator i = _map.begin(); i != _map.end(); i++ ) {
        r->addAnimation(i->first, i->second);
    }
    r->setDefaultAnimation(_defaultString);
    return r;
}

AnimationManager::shared_ptr Sprite::getCurrentAnimation() const
{
    assert(_oneShotReps != 0 || _defaultAnim >= 0);
    if ( _oneShotReps != 0 )
        return AnimationManager::current().getAnimation(_oneShotAnim);
    return AnimationManager::current().getAnimation(_defaultAnim);
}