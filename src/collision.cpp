#include "collision.h"

Rect::Rect( double left, double top, double right, double bottom )
{
    this->left   = left;
    this->top    = top;
    this->right  = right;
    this->bottom = bottom;
}

Physical::Physical( CollisionWorld& world, const Rect& position )
: _world( world )
, _rect( position )
{
    _world._objects.push_back( this );
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}

Physical::Physical( CollisionWorld& world, double width, double height )
: _world( world )
, _rect( 0, 0, width, height )
{
    _world._objects.push_back( this );
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}

Physical::~Physical()
{
    for ( CollisionWorld::Vector::iterator i = _world._objects.begin(); i != _world._objects.end(); i++ ) {
        if ( *i == this ) {
            _world._objects.erase( i );
            break;
        }
    }
}

const Rect& Physical::GetCollisionRect() const
{
    return _rect;
}

void Physical::SetCollisionRect( const Rect& rect )
{
    _rect = rect;
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}

void Physical::SetPosition( double x, double y )
{
    _rect.left = x;
    _rect.top  = y;
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}

void Physical::SetSize( double width, double height )
{
    _rect.left -= ( width  - ( _rect.right  - _rect.left ) ) / 2.0;
    _rect.top  -= ( height - ( _rect.bottom - _rect.top  ) ) / 2.0;
    _rect.right  = _rect.left + width;
    _rect.bottom = _rect.top  + height;
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}

void Physical::Move( double xOffset, double yOffset )
{
    _rect.left  +=  xOffset;
    _rect.right +=  xOffset;
    _rect.top   +=  yOffset;
    _rect.bottom += yOffset;
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}

CollisionWorld::CollisionWorld()
: _instantCollisions( false )
{
}

CollisionWorld::~CollisionWorld()
{
}

void CollisionWorld::SetInstantCollisions( bool instantCollisions )
{
    _instantCollisions = instantCollisions;
}

bool CollisionWorld::GetInstantCollisions() const
{
    return _instantCollisions;
}

void CollisionWorld::TriggerCollisions() const
{
    for ( Vector::const_iterator i = _objects.begin(); i != _objects.end(); i++ ) {
        for ( Vector::const_iterator j = i + 1; j != _objects.end(); j++ ) {
            const Rect& a = ( *i )->GetCollisionRect();
            const Rect& b = ( *j )->GetCollisionRect();

            if ( !( a.left >= b.right || a.top >= b.bottom ||
                    b.left >= a.right || b.top >= a.bottom ) ) {
                ( *i )->OnCollision( **j );
                ( *j )->OnCollision( **i );
            }
        }
    }
}

void CollisionWorld::TriggerCollisions( Physical* context ) const
{
    const Rect& a = context->GetCollisionRect();
    for ( Vector::const_iterator i = _objects.begin(); i != _objects.end(); i++ ) {
        if ( *i == context )
            continue;
        const Rect& b = ( *i )->GetCollisionRect();

        if ( !( a.left >= b.right || a.top >= b.bottom ||
                b.left >= a.right || b.top >= a.bottom ) ) {
            ( *i )->OnCollision( *context );
            context->OnCollision( **i );
        }
    }
}