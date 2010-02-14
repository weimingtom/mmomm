#include "collision.h"
#include <cmath>
#include <algorithm>

const double CollisionWorld::CELL_SIZE = 32.0;

CollisionWorld::CollisionWorld()
: _instantCollisions(false)
, _leftBound(0)
, _topBound(0)
, _rightBound(0)
, _bottomBound(0)
{
}

CollisionWorld::~CollisionWorld()
{
}

void CollisionWorld::setInstantCollisions(bool instantCollisions)
{
    _instantCollisions = instantCollisions;
}

bool CollisionWorld::getInstantCollisions() const
{
    return _instantCollisions;
}

void CollisionWorld::triggerCollisions()
{
    // Trigger collisions for objects which have blocked each other since the
    // last time this function was called (or instant collisions turned off)
    for ( CollisionList::iterator i = _collisions.begin(); i != _collisions.end(); i++ ) {
        i->first->onCollision(*(i->second));
        i->second->onCollision(*(i->first));
    }

    for ( long y = _topBound; y < _bottomBound; y++ ) {
        for ( long x = _leftBound; x < _rightBound; x++ ) {
            // An object's cell is defined by its top-left coordinate, so can
            // check all possible collisions by checking objects in its cell
            // and the cells one space down or to the right (or both)
            Cell& a = _map[IVector2D(x,     y    )];
            Cell& b = _map[IVector2D(x + 1, y    )];
            Cell& c = _map[IVector2D(x,     y + 1)];
            Cell& d = _map[IVector2D(x + 1, y + 1)];

            for ( Cell::const_iterator i = a.begin(); i != a.end(); i++ ) {
                const Rect& r = (*i)->getCollisionRect();

                // Check objects in cells, starting at the current object (so as
                // not to check the same pair twice)
                Cell::const_iterator j = i;
                for ( j++; j != a.end(); j++ ) {
                    if ( !checkCollision(*i, *j, r) )
                        break;
                }

                // Check objects in other cells
                for ( j = b.begin(); j != b.end(); j++ ) {
                    if ( !checkCollision(*i, *j, r) )
                        break;
                }
                for ( j = c.begin(); j != c.end(); j++ ) {
                    if ( !checkCollision(*i, *j, r) )
                        break;
                }
                for ( j = d.begin(); j != d.end(); j++ ) {
                    if ( !checkCollision(*i, *j, r) )
                        break;
                }

            }
        }
    }
    _collisions.clear();
}

bool CollisionWorld::Sort::operator()(const Physical* a, const Physical* b) const
{
    if ( a->getCollisionRect().left != a->getCollisionRect().left )
        return a->getCollisionRect().left < b->getCollisionRect().left;
    // Objects in a set cannot be equal, so need some backup ordering in case
    // x-positions are equal
    return a < b;
}

bool CollisionWorld::checkCollision(Physical* a, Physical* b, const Rect& aRect)
{
    // If these two objects have already collided this "frame", don't collide them
    // again. This can happen if an object enters another as it moves, and comes to
    // rest still overlapping it
    for ( std::size_t i = 0; i < _collisions.size(); i++ ) {
        if ( (_collisions[i].first  == a && _collisions[i].second == b) ||
             (_collisions[i].second == b && _collisions[i].first  == a) ) {
            return true;
        }
    }

    const Rect& bRect = b->getCollisionRect();

    // Return false to indicate don't need to check further objects
    if ( bRect.left >= aRect.right )
        return false;

    // Collide if rectangles overlap
    if ( !(aRect.left >= bRect.right || aRect.top >= bRect.bottom ||
           bRect.left >= aRect.right || bRect.top >= aRect.bottom) ) {
        a->onCollision(*b);
        b->onCollision(*a);
    }
    return true;
}

CollisionWorld::Cell& CollisionWorld::getCellAtPoint(const Vector2D& position)
{
    long lx = long(floor(position.x / CELL_SIZE));
    long ly = long(floor(position.y / CELL_SIZE));

    // Update bounding area for all cells which potentially contain objects
    _leftBound   = std::min(_leftBound,   lx    );
    _topBound    = std::min(_topBound,    ly    );
    _rightBound  = std::max(_rightBound,  lx + 1);
    _bottomBound = std::max(_bottomBound, ly + 1);

    return _map[IVector2D(lx, ly)];
}

Physical::Physical(CollisionWorld& world, const Rect& rect)
: _world(world)
, _cell(0)
, _rect(rect)
{
    // Object can't be bigger than a cell - increase CELL_SIZE if you need to
    assert(rect.dim().x < CollisionWorld::CELL_SIZE);
    assert(rect.dim().y < CollisionWorld::CELL_SIZE);

    _cell = &_world.getCellAtPoint(_rect.min());
    _cell->insert(this);
    updateWorld();
}

Physical::Physical(CollisionWorld& world, const Vector2D& dimensions)
: _world(world)
, _cell(0)
, _rect(Vector2D(), dimensions)
{
    // Object can't be bigger than a cell - increase CELL_SIZE if you need to
    assert(dimensions.x < CollisionWorld::CELL_SIZE);
    assert(dimensions.y < CollisionWorld::CELL_SIZE);

    _cell = &_world.getCellAtPoint(_rect.min());
    _cell->insert(this);
    updateWorld();
}

Physical::~Physical()
{
    _cell->erase(this);
}

const Rect& Physical::getCollisionRect() const
{
    return _rect;
}

void Physical::setCollisionRect(const Rect& rect)
{
    // Object can't be bigger than a cell - increase CELL_SIZE if you need to
    assert(rect.dim().x < CollisionWorld::CELL_SIZE);
    assert(rect.dim().y < CollisionWorld::CELL_SIZE);

    _rect = rect;
    updateWorld();
}

void Physical::setPosition(const Vector2D& position)
{
	_rect += position - _rect.min();
    updateWorld();
}

Vector2D Physical::getPosition() const
{
	return _rect.min();
}

void Physical::setSize(const Vector2D& dimensions)
{
    // Object can't be bigger than a cell - increase CELL_SIZE if you need to
    assert(dimensions.x < CollisionWorld::CELL_SIZE);
    assert(dimensions.y < CollisionWorld::CELL_SIZE);
	
	Vector2D offset = .5 * (dimensions - _rect.dim());
	
    _rect.left   -= offset.x;
    _rect.top    -= offset.y;
    _rect.right  += offset.x;
    _rect.bottom += offset.y;
    updateWorld();
}

void Physical::move(const Vector2D& offset_)
{
	Vector2D offset = offset_;

    // Potential coordinates to check for obstacles
    long rx = long(floor(_rect.left            / CollisionWorld::CELL_SIZE));
    long ry = long(floor(_rect.top             / CollisionWorld::CELL_SIZE));
    long dx = long(floor(_rect.left + offset.x / CollisionWorld::CELL_SIZE));
    long dy = long(floor(_rect.top  + offset.y / CollisionWorld::CELL_SIZE));
	
    bool   xCarry = false;
    bool   yCarry = false;
    double carry  = 0.0;
	
    for ( long ty = std::min(ry, dy) - 1; ty <= std::max(ry, dy) + 1; ty++ ) {
        for ( long tx = std::min(rx, dx) - 1; tx <= std::max(rx, dx) + 1; tx++ ) {
            Cell& cell = _world._map[IVector2D(tx, ty)];
            for ( Cell::const_iterator i = cell.begin(); i != cell.end(); i++ ) {
                // Sweep to find obstacles. Only go as far as the closest obstacle allows.
                // The closest obstacle is added to list of collisions
                if ( *i == this || !_world.shouldBlock(this, *i) )
                    continue;
                const Rect& iRect = (*i)->getCollisionRect();

                if ( iRect.left >= _rect.right && iRect.left >= _rect.right + offset.x )
                    break;

                if ( offset.y > 0 && _rect.bottom < (iRect.top + iRect.bottom + _rect.bottom - _rect.top) / 2.0 &&
                     _rect.bottom + offset.x > iRect.top ) {
                    bool overlap = _rect.bottom > iRect.top;
                    double left  = overlap ? _rect.left  : _rect.left  + offset.x * (iRect.top - _rect.bottom) / offset.y;
                    double right = overlap ? _rect.right : _rect.right + offset.x * (iRect.top - _rect.bottom) / offset.y;

                    if ( !(left >= iRect.right || right <= iRect.left) ) {
                        yCarry    = !(xCarry = true);
                        carry     = offset.x;
                        offset.x *= overlap ? 0 : (iRect.top - _rect.bottom) / offset.y;
                        offset.y  = overlap ? 0 : iRect.top - _rect.bottom;
                        carry    -= offset.x;

                        _collisions.clear();
                        _collisions.push_back(*i);
                    }
                }

                if ( offset.y < 0 && _rect.top > (iRect.bottom + iRect.top - _rect.bottom + _rect.top) / 2.0 &&
                     _rect.top + offset.y < iRect.bottom ) {
                    bool overlap = _rect.top < iRect.bottom;
                    double left  = overlap ? _rect.left  : _rect.left  + offset.x * (iRect.bottom - _rect.top) / offset.y;
                    double right = overlap ? _rect.right : _rect.right + offset.x * (iRect.bottom - _rect.top) / offset.y;

                    if ( !(left >= iRect.right || right <= iRect.left) ) {
                        yCarry    = !(xCarry = true);
                        carry     = offset.x;
                        offset.x *= overlap ? 0 : (iRect.bottom - _rect.top) / offset.y;
                        offset.y  = overlap ? 0 : iRect.bottom - _rect.top;
                        carry    -= offset.x;

                        _collisions.clear();
                        _collisions.push_back(*i);
                    }
                }

                if ( offset.x > 0 && _rect.right < (iRect.left + iRect.right + _rect.right - _rect.left) / 2.0 &&
                     _rect.right + offset.x > iRect.left ) {
                    bool overlap  = _rect.right > iRect.left;
                    double top    = overlap ? _rect.top    : _rect.top    + offset.y * (iRect.left - _rect.right) / offset.x;
                    double bottom = overlap ? _rect.bottom : _rect.bottom + offset.y * (iRect.left - _rect.right) / offset.x;

                    if ( !(top >= iRect.bottom || bottom <= iRect.top) ) {
                        xCarry    = !(yCarry = true);
                        carry     = offset.y;
                        offset.y *= overlap ? 0 : (iRect.left - _rect.right) / offset.x;
                        offset.x  = overlap ? 0 : iRect.left - _rect.right;
                        carry    -= offset.y;

                        _collisions.clear();
                        _collisions.push_back(*i);
                    }
                }

                if ( offset.x < 0 && _rect.left > (iRect.right + iRect.left - _rect.right + _rect.left) / 2.0 &&
                     _rect.left + offset.x < iRect.right ) {
                    bool overlap  = _rect.left < iRect.right;
                    double top    = overlap ? _rect.top    : _rect.top    + offset.y * (iRect.right - _rect.left) / offset.x;
                    double bottom = overlap ? _rect.bottom : _rect.bottom + offset.y * (iRect.right - _rect.left) / offset.x;

                    if ( !(top >= iRect.bottom || bottom <= iRect.top) ) {
                        xCarry    = !(yCarry = true);
                        carry     = offset.y;
                        offset.y *= overlap ? 0 : (iRect.right - _rect.left) / offset.x;
                        offset.x  = overlap ? 0 : iRect.right - _rect.left;
                        carry    -= offset.y;

                        _collisions.clear();
                        _collisions.push_back(*i);
                    }
                }
            }
        }
    }


    for ( long ty = std::min(ry, dy) - 1; ty <= std::max(ry, dy) + 1; ty++ ) {
        for ( long tx = std::min(rx, dx) - 1; tx <= std::max(rx, dx) + 1; tx++ ) {
            Cell& cell = _world._map[IVector2D(tx, ty)];
            for ( Cell::const_iterator i = cell.begin(); i != cell.end(); i++ ) {
                // Now sweep again to find objects on the restricted path that don't block
                // (only such objects exist, since path limited to closest obstacle).
                // Add to list of collisions
                if ( *i == this || _world.shouldBlock(this, *i) )
                    continue;
                const Rect& iRect = (*i)->getCollisionRect();

                if ( iRect.left >= _rect.right && iRect.left >= _rect.right + offset.x )
                    break;

                if ( offset.y > 0 && _rect.bottom < iRect.top &&
                     _rect.bottom + offset.y >= iRect.top ) {
                    double left  = _rect.left  + offset.x * (iRect.top - _rect.bottom) / offset.y;
                    double right = _rect.right + offset.x * (iRect.top - _rect.bottom) / offset.y;
                    if ( !(left >= iRect.right || right < iRect.left) ) {
                        _collisions.push_back(*i);
                        continue;
                    }
                }

                if ( offset.y < 0 && _rect.top >= iRect.bottom &&
                     _rect.top + offset.y < iRect.bottom ) {
                    double left  = _rect.left  + offset.x * (iRect.bottom - _rect.top) / offset.y;
                    double right = _rect.right + offset.x * (iRect.bottom - _rect.top) / offset.y;
                    if ( !( left >= iRect.right || right < iRect.left ) ) {
                        _collisions.push_back(*i);
                        continue;
                    }
                }

                if ( offset.x > 0 && _rect.right < iRect.left &&
                     _rect.right + offset.x >= iRect.left ) {
                    double top    = _rect.top    + offset.y * ( iRect.left - _rect.right ) / offset.x;
                    double bottom = _rect.bottom + offset.y * ( iRect.left - _rect.right ) / offset.x;
                    if ( !( top >= iRect.bottom || bottom < iRect.top ) ) {
                        _collisions.push_back(*i);
                        continue;
                    }
                }

                if ( offset.x < 0 && _rect.left >= iRect.right &&
                     _rect.left + offset.x < iRect.right ) {
                    double top    = _rect.top    + offset.y * (iRect.right - _rect.left) / offset.x;
                    double bottom = _rect.bottom + offset.y * (iRect.right - _rect.left) / offset.x;
                    if ( !( top >= iRect.bottom || bottom < iRect.top ) ) {
                        _collisions.push_back(*i);
                        continue;
                    }
                }
            }
        }
    }

	_rect += offset;

    if ( xCarry ) {
        move( Vector2D(carry, 0.0) );
    }
    else if ( yCarry ) {
        move( Vector2D(0.0, carry) );
    }
    else {
        // If instant collisions are turned off, need to store collisions in the
        // CollisionWorld for the next call to TriggerCollisions
        if ( !_world._instantCollisions ) {
            for ( std::size_t i = 0; i < _collisions.size(); i++ )
                _world._collisions.push_back(std::pair< Physical*, Physical* >(this, _collisions[i]));
        }
        updateWorld();
    }
}

void Physical::updateWorld()
{
    // Change current cell
    Cell* t = &_world.getCellAtPoint(_rect.min());
    if ( t != _cell ) {
        _cell->erase(this);
        t->insert(this);
        _cell = t;
    }
    // If instanct collisions is turned on, immediately collide with everything
    // overlapping
    if ( _world._instantCollisions ) {
        long x = long(floor(_rect.left / CollisionWorld::CELL_SIZE));
        long y = long(floor(_rect.top  / CollisionWorld::CELL_SIZE));
        const Rect& r = getCollisionRect();

        // Collide with objects that were in the path of the last Move
        for ( std::size_t i = 0; i < _collisions.size(); i++ ) {
            onCollision(*_collisions[i]);
            _collisions[i]->onCollision(*this);
        }

        for ( long ty = y - 1; ty <= y + 1; ty++ ) {
            for ( long tx = x - 1; tx <= x + 1; tx++ ) {
                Cell& cell = _world._map[IVector2D(tx, ty)];
                for ( Cell::const_iterator i = cell.begin(); i != cell.end(); i++ ) {
                    if ( *i == this )
                        continue;

                    // Don't collide twice with objects that were in the path of the
                    // last Move and are still overlapping
                    bool alreadyCollided = false;
                    for ( std::size_t j = 0; j < _collisions.size(); j++ ) {
                        if ( _collisions[j] == *i ) {
                            alreadyCollided = true;
                            break;
                        }
                    }
                    if ( alreadyCollided )
                        continue;

                    if ( !_world.checkCollision(this, *i, r) )
                        break;
                }
            }
        }
    }
    _collisions.clear();
}
