#include "collision.h"
#include <cmath>
#include <algorithm>

const double CollisionWorld::CELL_SIZE = 32.0;

Rect::Rect()
{
    this->left   = 0;
    this->top    = 0;
    this->right  = 0;
    this->bottom = 0;
}

Rect::Rect(double left, double top, double right, double bottom)
{
    this->left   = left;
    this->top    = top;
    this->right  = right;
    this->bottom = bottom;
}

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

void CollisionWorld::SetInstantCollisions(bool instantCollisions)
{
    _instantCollisions = instantCollisions;
}

bool CollisionWorld::GetInstantCollisions() const
{
    return _instantCollisions;
}

void CollisionWorld::TriggerCollisions()
{
    // Trigger collisions for objects which have blocked each other since the
    // last time this function was called (or instant collisions turned off)
    for ( CollisionList::iterator i = _collisions.begin(); i != _collisions.end(); i++ ) {
        i->first->OnCollision(*(i->second));
        i->second->OnCollision(*(i->first));
    }

    for ( long y = _topBound; y < _bottomBound; y++ ) {
        for ( long x = _leftBound; x < _rightBound; x++ ) {
            // An object's cell is defined by its top-left coordinate, so can
            // check all possible collisions by checking objects in its cell
            // and the cells one space down or to the right (or both)
            Cell& a = _map[CellCoord(x,     y    )];
            Cell& b = _map[CellCoord(x + 1, y    )];
            Cell& c = _map[CellCoord(x,     y + 1)];
            Cell& d = _map[CellCoord(x + 1, y + 1)];

            for ( Cell::const_iterator i = a.begin(); i != a.end(); i++ ) {
                const Rect& r = (*i)->GetCollisionRect();

                // Check objects in cells, starting at the current object (so as
                // not to check the same pair twice)
                Cell::const_iterator j = i;
                for ( j++; j != a.end(); j++ ) {
                    if ( !CheckCollision(*i, *j, r) )
                        break;
                }

                // Check objects in other cells
                for ( j = b.begin(); j != b.end(); j++ ) {
                    if ( !CheckCollision(*i, *j, r) )
                        break;
                }
                for ( j = c.begin(); j != c.end(); j++ ) {
                    if ( !CheckCollision(*i, *j, r) )
                        break;
                }
                for ( j = d.begin(); j != d.end(); j++ ) {
                    if ( !CheckCollision(*i, *j, r) )
                        break;
                }

            }
        }
    }
    _collisions.clear();
}

bool CollisionWorld::Sort::operator()(const Physical* a, const Physical* b) const
{
    if ( a->GetCollisionRect().left != a->GetCollisionRect().left )
        return a->GetCollisionRect().left < b->GetCollisionRect().left;
    // Objects in a set cannot be equal, so need some backup ordering in case
    // x-positions are equal
    return a < b;
}

bool CollisionWorld::CheckCollision(Physical* a, Physical* b, const Rect& aRect)
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

    const Rect& bRect = b->GetCollisionRect();

    // Return false to indicate don't need to check further objects
    if ( bRect.left >= aRect.right )
        return false;

    // Collide if rectangles overlap
    if ( !(aRect.left >= bRect.right || aRect.top >= bRect.bottom ||
           bRect.left >= aRect.right || bRect.top >= aRect.bottom) ) {
        a->OnCollision(*b);
        b->OnCollision(*a);
    }
    return true;
}

CollisionWorld::Cell& CollisionWorld::GetCellAtPoint(double x, double y)
{
    long lx = long(floor(x / CELL_SIZE));
    long ly = long(floor(y / CELL_SIZE));

    // Update bounding area for all cells which potentially contain objects
    _leftBound   = std::min(_leftBound,   lx    );
    _topBound    = std::min(_topBound,    ly    );
    _rightBound  = std::max(_rightBound,  lx + 1);
    _bottomBound = std::max(_bottomBound, ly + 1);

    return _map[CellCoord(lx, ly)];
}

Physical::Physical(CollisionWorld& world, const Rect& rect)
: _world(world)
, _cell(0)
, _rect(rect)
{
    // Object can't be bigger than a cell - increase CELL_SIZE if you need to
    assert(rect.right  - rect.left < CollisionWorld::CELL_SIZE);
    assert(rect.bottom - rect.top  < CollisionWorld::CELL_SIZE);

    _cell = &_world.GetCellAtPoint(_rect.left, _rect.top);
    _cell->insert(this);
    UpdateWorld();
}

Physical::Physical(CollisionWorld& world, double width, double height)
: _world(world)
, _cell(0)
, _rect(0, 0, width, height)
{
    // Object can't be bigger than a cell - increase CELL_SIZE if you need to
    assert(width  < CollisionWorld::CELL_SIZE);
    assert(height < CollisionWorld::CELL_SIZE);

    _cell = &_world.GetCellAtPoint(_rect.left, _rect.top);
    _cell->insert(this);
    UpdateWorld();
}

Physical::~Physical()
{
    _cell->erase(this);
}

const Rect& Physical::GetCollisionRect() const
{
    return _rect;
}

void Physical::SetCollisionRect(const Rect& rect)
{
    // Object can't be bigger than a cell - increase CELL_SIZE if you need to
    assert(rect.right  - rect.left < CollisionWorld::CELL_SIZE);
    assert(rect.bottom - rect.top  < CollisionWorld::CELL_SIZE);

    _rect = rect;
    UpdateWorld();
}

void Physical::SetPosition(double x, double y)
{
    _rect.right  += x - _rect.left;
    _rect.bottom += y - _rect.top;
    _rect.left = x;
    _rect.top  = y;
    UpdateWorld();
}

Vector2D Physical::GetPosition() const
{
	return Vector2D(_rect.left, _rect.top);
}

void Physical::SetSize(double width, double height)
{
    // Object can't be bigger than a cell - increase CELL_SIZE if you need to
    assert(width  < CollisionWorld::CELL_SIZE);
    assert(height < CollisionWorld::CELL_SIZE);

    _rect.left -= (width  - (_rect.right  - _rect.left)) / 2.0;
    _rect.top  -= (height - (_rect.bottom - _rect.top )) / 2.0;
    _rect.right  = _rect.left + width;
    _rect.bottom = _rect.top  + height;
    UpdateWorld();
}

void Physical::Move(double xOffset, double yOffset)
{
    // Potential coordinates to check for obstacles
    long rx = long(floor(_rect.left           / CollisionWorld::CELL_SIZE));
    long ry = long(floor(_rect.top            / CollisionWorld::CELL_SIZE));
    long dx = long(floor(_rect.left + xOffset / CollisionWorld::CELL_SIZE));
    long dy = long(floor(_rect.top  + yOffset / CollisionWorld::CELL_SIZE));

    bool   xCarry = false;
    bool   yCarry = false;
    double carry  = 0.0;

    for ( long ty = std::min(ry, dy) - 1; ty <= std::max(ry, dy) + 1; ty++ ) {
        for ( long tx = std::min(rx, dx) - 1; tx <= std::max(rx, dx) + 1; tx++ ) {
            Cell& cell = _world._map[CellCoord(tx, ty)];
            for ( Cell::const_iterator i = cell.begin(); i != cell.end(); i++ ) {
                // Sweep to find obstacles. Only go as far as the closest obstacle allows.
                // The closest obstacle is added to list of collisions
                if ( *i == this || !_world.ShouldBlock(this, *i) )
                    continue;
                const Rect& iRect = (*i)->GetCollisionRect();

                if ( iRect.left >= _rect.right && iRect.left >= _rect.right + xOffset )
                    break;

                if ( yOffset > 0 && _rect.bottom < (iRect.top + iRect.bottom) / 2.0 &&
                     _rect.bottom + yOffset > iRect.top ) {
                    bool overlap = _rect.bottom > iRect.top;
                    double left  = overlap ? _rect.left  : _rect.left  + xOffset * (iRect.top - _rect.bottom) / yOffset;
                    double right = overlap ? _rect.right : _rect.right + xOffset * (iRect.top - _rect.bottom) / yOffset;

                    if ( !(left >= iRect.right || right <= iRect.left) ) {
                        yCarry   = !(xCarry = true);
                        carry    = xOffset;
                        xOffset *= overlap ? 0 : (iRect.top - _rect.bottom) / yOffset;
                        yOffset  = overlap ? 0 : iRect.top - _rect.bottom;
                        carry   -= xOffset;

                        _collisions.clear();
                        _collisions.push_back(*i);
                    }
                }

                if ( yOffset < 0 && _rect.top > (iRect.bottom + iRect.top) / 2.0 &&
                     _rect.top + yOffset < iRect.bottom ) {
                    bool overlap = _rect.top < iRect.bottom;
                    double left  = overlap ? _rect.left  : _rect.left  + xOffset * (iRect.bottom - _rect.top) / yOffset;
                    double right = overlap ? _rect.right : _rect.right + xOffset * (iRect.bottom - _rect.top) / yOffset;

                    if ( !(left >= iRect.right || right <= iRect.left) ) {
                        yCarry   = !(xCarry = true);
                        carry    = xOffset;
                        xOffset *= overlap ? 0 : (iRect.bottom - _rect.top) / yOffset;
                        yOffset  = overlap ? 0 : iRect.bottom - _rect.top;
                        carry   -= xOffset;

                        _collisions.clear();
                        _collisions.push_back(*i);
                    }
                }

                if ( xOffset > 0 && _rect.right < (iRect.left + iRect.right) / 2.0 &&
                     _rect.right + xOffset > iRect.left ) {
                    bool overlap  = _rect.right > iRect.left;
                    double top    = overlap ? _rect.top    : _rect.top    + yOffset * (iRect.left - _rect.right) / xOffset;
                    double bottom = overlap ? _rect.bottom : _rect.bottom + yOffset * (iRect.left - _rect.right) / xOffset;

                    if ( !(top >= iRect.bottom || bottom <= iRect.top) ) {
                        xCarry   = !(yCarry = true);
                        carry    = yOffset;
                        yOffset *= overlap ? 0 : (iRect.left - _rect.right) / xOffset;
                        xOffset  = overlap ? 0 : iRect.left - _rect.right;
                        carry   -= yOffset;

                        _collisions.clear();
                        _collisions.push_back(*i);
                    }
                }

                if ( xOffset < 0 && _rect.left > (iRect.right + iRect.left) / 2.0 &&
                     _rect.left + xOffset < iRect.right ) {
                    bool overlap  = _rect.left < iRect.right;
                    double top    = overlap ? _rect.top    : _rect.top    + yOffset * (iRect.right - _rect.left) / xOffset;
                    double bottom = overlap ? _rect.bottom : _rect.bottom + yOffset * (iRect.right - _rect.left) / xOffset;

                    if ( !(top >= iRect.bottom || bottom <= iRect.top) ) {
                        xCarry   = !(yCarry = true);
                        carry    = yOffset;
                        yOffset *= overlap ? 0 : (iRect.right - _rect.left) / xOffset;
                        xOffset  = overlap ? 0 : iRect.right - _rect.left;
                        carry   -= yOffset;

                        _collisions.clear();
                        _collisions.push_back(*i);
                    }
                }
            }
        }
    }


    for ( long ty = std::min(ry, dy) - 1; ty <= std::max(ry, dy) + 1; ty++ ) {
        for ( long tx = std::min(rx, dx) - 1; tx <= std::max(rx, dx) + 1; tx++ ) {
            Cell& cell = _world._map[CellCoord(tx, ty)];
            for ( Cell::const_iterator i = cell.begin(); i != cell.end(); i++ ) {
                // Now sweep again to find objects on the restricted path that don't block
                // (only such objects exist, since path limited to closest obstacle).
                // Add to list of collisions
                if ( *i == this || _world.ShouldBlock(this, *i) )
                    continue;
                const Rect& iRect = (*i)->GetCollisionRect();

                if ( iRect.left >= _rect.right && iRect.left >= _rect.right + xOffset )
                    break;

                if ( yOffset > 0 && _rect.bottom < iRect.top &&
                     _rect.bottom + yOffset >= iRect.top ) {
                    double left  = _rect.left  + xOffset * (iRect.top - _rect.bottom) / yOffset;
                    double right = _rect.right + xOffset * (iRect.top - _rect.bottom) / yOffset;
                    if ( !(left >= iRect.right || right < iRect.left) ) {
                        _collisions.push_back(*i);
                        continue;
                    }
                }

                if ( yOffset < 0 && _rect.top >= iRect.bottom &&
                     _rect.top + yOffset < iRect.bottom ) {
                    double left  = _rect.left  + xOffset * (iRect.bottom - _rect.top) / yOffset;
                    double right = _rect.right + xOffset * (iRect.bottom - _rect.top) / yOffset;
                    if ( !( left >= iRect.right || right < iRect.left ) ) {
                        _collisions.push_back(*i);
                        continue;
                    }
                }

                if ( xOffset > 0 && _rect.right < iRect.left &&
                     _rect.right + xOffset >= iRect.left ) {
                    double top    = _rect.top    + yOffset * ( iRect.left - _rect.right ) / xOffset;
                    double bottom = _rect.bottom + yOffset * ( iRect.left - _rect.right ) / xOffset;
                    if ( !( top >= iRect.bottom || bottom < iRect.top ) ) {
                        _collisions.push_back(*i);
                        continue;
                    }
                }

                if ( xOffset < 0 && _rect.left >= iRect.right &&
                     _rect.left + xOffset < iRect.right ) {
                    double top    = _rect.top    + yOffset * (iRect.right - _rect.left) / xOffset;
                    double bottom = _rect.bottom + yOffset * (iRect.right - _rect.left) / xOffset;
                    if ( !( top >= iRect.bottom || bottom < iRect.top ) ) {
                        _collisions.push_back(*i);
                        continue;
                    }
                }
            }
        }
    }

    _rect.left   += xOffset;
    _rect.top    += yOffset;
    _rect.right  += xOffset;
    _rect.bottom += yOffset;

    if ( xCarry ) {
        Move( carry, 0.0 );
    }
    else if ( yCarry ) {
        Move( 0.0, carry );
    }
    else {
        // If instant collisions are turned off, need to store collisions in the
        // CollisionWorld for the next call to TriggerCollisions
        if ( !_world._instantCollisions ) {
            for ( std::size_t i = 0; i < _collisions.size(); i++ )
                _world._collisions.push_back(std::pair< Physical*, Physical* >(this, _collisions[i]));
        }
        UpdateWorld();
    }
}

void Physical::UpdateWorld()
{
    // Change current cell
    Cell* t = &_world.GetCellAtPoint(_rect.left, _rect.top);
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
        const Rect& r = GetCollisionRect();

        // Collide with objects that were in the path of the last Move
        for ( std::size_t i = 0; i < _collisions.size(); i++ ) {
            OnCollision(*_collisions[i]);
            _collisions[i]->OnCollision(*this);
        }

        for ( long ty = y - 1; ty <= y + 1; ty++ ) {
            for ( long tx = x - 1; tx <= x + 1; tx++ ) {
                Cell& cell = _world._map[CellCoord(tx, ty)];
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

                    if ( !_world.CheckCollision(this, *i, r) )
                        break;
                }
            }
        }
    }
    _collisions.clear();
}
