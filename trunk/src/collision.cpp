#include "collision.h"
#include <cmath>
#include <algorithm>

const double CollisionWorld::CELL_SIZE = 32.0;

Rect::Rect( double left, double top, double right, double bottom )
{
    this->left   = left;
    this->top    = top;
    this->right  = right;
    this->bottom = bottom;
}

CollisionWorld::CollisionWorld()
: _instantCollisions( false )
, _leftBound( 0 )
, _topBound( 0 )
, _rightBound( 0 )
, _bottomBound( 0 )
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

void CollisionWorld::TriggerCollisions()
{
    for ( long y = _topBound; y < _bottomBound; y++ ) {
        for ( long x = _leftBound; x < _rightBound; x++ ) {
            Cell& a = _map[ CellCoord( x,     y     ) ];
            Cell& b = _map[ CellCoord( x + 1, y     ) ];
            Cell& c = _map[ CellCoord( x,     y + 1 ) ];
            Cell& d = _map[ CellCoord( x + 1, y + 1 ) ];

            for ( Cell::const_iterator i = a.begin(); i != a.end(); i++ ) {
                const Rect& r = ( *i )->GetCollisionRect();
                
                Cell::const_iterator j = i;
                for ( j++; j != a.end(); j++ ) {
                    if ( !CheckCollision( *i, *j, r ) )
                        break;
                }

                for ( j = b.begin(); j != b.end(); j++ ) {
                    if ( !CheckCollision( *i, *j, r ) )
                        break;
                }

                for ( j = c.begin(); j != c.end(); j++ ) {
                    if ( !CheckCollision( *i, *j, r ) )
                        break;
                }

                for ( j = d.begin(); j != d.end(); j++ ) {
                    if ( !CheckCollision( *i, *j, r ) )
                        break;
                }

            }
        }
    }
}

CollisionWorld::CellCoord::CellCoord( long x, long y )
{
    this->x = x;
    this->y = y;
}

bool CollisionWorld::CellCoord::operator==( const CellCoord& a ) const
{
    return x == a.x && y == a.y;
}

bool CollisionWorld::Sort::operator()( const Physical* a, const Physical* b ) const
{
    return a->GetCollisionRect().left < b->GetCollisionRect().left;
}

std::size_t CollisionWorld::Hash::operator()( const CellCoord& a ) const
{
    std::size_t seed = 0;
    boost::hash_combine( seed, a.x );
    boost::hash_combine( seed, a.y );
    return seed;
}

void CollisionWorld::TriggerCollisions( Physical* context )
{
    long x = long( floor( context->GetCollisionRect().left / CELL_SIZE ) );
    long y = long( floor( context->GetCollisionRect().top  / CELL_SIZE ) );
    const Rect& r = context->GetCollisionRect();

    for ( long ty = y - 1; ty <= y + 1; ty++ ) {
        for ( long tx = x - 1; tx <= x + 1; tx++ ) {
            Cell& cell = _map[ CellCoord( tx, ty ) ];
            for ( Cell::const_iterator i = cell.begin(); i != cell.end(); i++ ) {
                if ( *i == context )
                    continue;
                if ( !CheckCollision( context, *i, r ) )
                    break;
            }
        }
    }
}

bool CollisionWorld::CheckCollision( Physical* a, Physical* b, const Rect& aRect )
{
    const Rect& bRect = b->GetCollisionRect();

    if ( bRect.left >= aRect.right )
        return false;

    if ( !( aRect.left >= bRect.right || aRect.top >= bRect.bottom ||
            bRect.left >= aRect.right || bRect.top >= aRect.bottom ) ) {
        a->OnCollision( *b );
        b->OnCollision( *a );
    }
    return true;
}

CollisionWorld::Cell& CollisionWorld::GetCellAtPoint( double x, double y )
{
    long lx = long( floor( x / CELL_SIZE ) );
    long ly = long( floor( y / CELL_SIZE ) );

    _leftBound   = std::min( _leftBound,   lx );
    _topBound    = std::min( _topBound,    ly );
    _rightBound  = std::max( _rightBound,  lx + 1 );
    _bottomBound = std::max( _bottomBound, ly + 1 );

    return _map[ CellCoord( lx, ly ) ];
}

Physical::Physical( CollisionWorld& world, const Rect& rect )
: _world( world )
, _cell( 0 )
, _rect( rect )
{
    assert( rect.right  - rect.left < CollisionWorld::CELL_SIZE );
    assert( rect.bottom - rect.top  < CollisionWorld::CELL_SIZE );

    _cell = &_world.GetCellAtPoint( _rect.left, _rect.top );
    _cell->insert( this );
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}

Physical::Physical( CollisionWorld& world, double width, double height )
: _world( world )
, _cell( 0 )
, _rect( 0, 0, width, height )
{
    assert( width  < CollisionWorld::CELL_SIZE );
    assert( height < CollisionWorld::CELL_SIZE );

    _cell = &_world.GetCellAtPoint( _rect.left, _rect.top );
    _cell->insert( this );
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}

Physical::~Physical()
{
    _cell->erase( this );
}

const Rect& Physical::GetCollisionRect() const
{
    return _rect;
}

void Physical::SetCollisionRect( const Rect& rect )
{
    assert( rect.right  - rect.left < CollisionWorld::CELL_SIZE );
    assert( rect.bottom - rect.top  < CollisionWorld::CELL_SIZE );

    _rect = rect;
    UpdateWorld();
}

void Physical::SetPosition( double x, double y )
{
    _rect.left = x;
    _rect.top  = y;
    UpdateWorld();
}

void Physical::SetSize( double width, double height )
{
    assert( width  < CollisionWorld::CELL_SIZE );
    assert( height < CollisionWorld::CELL_SIZE );

    _rect.left -= ( width  - ( _rect.right  - _rect.left ) ) / 2.0;
    _rect.top  -= ( height - ( _rect.bottom - _rect.top  ) ) / 2.0;
    _rect.right  = _rect.left + width;
    _rect.bottom = _rect.top  + height;
    UpdateWorld();
}

void Physical::Move( double xOffset, double yOffset )
{
    _rect.left  +=  xOffset;
    _rect.right +=  xOffset;
    _rect.top   +=  yOffset;
    _rect.bottom += yOffset;
    UpdateWorld();
}

void Physical::UpdateWorld()
{
    Cell* t = &_world.GetCellAtPoint( _rect.left, _rect.top );
    if ( t != _cell ) {
        _cell->erase( this );
        t->insert( this );
        _cell = t;
    }
    if ( _world._instantCollisions )
        _world.TriggerCollisions( this );
}