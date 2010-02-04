#ifndef COLLISION_H_
#define COLLISION_H_

#include "worldCommon.h"
#include <set>
#include <boost/unordered_map.hpp>
#include <vector>
#include "vector2D.h"

#ifdef __LP64__ //64 linux support and such.
typedef unsigned ActorID;
#else
typedef unsigned long ActorID;
#endif

class Physical;
struct Rect {
	Rect();
    Rect(double left, double top, double right, double bottom);

    double left;
    double top;
    double right;
    double bottom;
};

// A world consisting of physical objects that can collide with each other.
// All objects in a world should be destroyed before the world is destroyed.
class CollisionWorld {
public:

    // Constructor.
    CollisionWorld();

    // Destructor.
    ~CollisionWorld();

    // Sets whether this CollisionWorld should use instant collisions. When instant
    // collisions are turned on, the OnCollision() methods of colliding objects are
    // called whenever their collision rectangles overlap after either has moved.
    // When turned off, the TriggerCollisions method should be used periodically to
    // call the OnCollision() methods of all objects whose collision rectangles
    // currently overlap. The default value is false.
    void SetInstantCollisions(bool instantCollisions);

    // Retrieves the current setting for instant collisions.
    bool GetInstantCollisions() const;

    // Checks all objects in this CollisionWorld for collisions. For each collision
    // between objects A and B, both A.OnCollision(B) and B.OnCollision(A) are
    // called, in no specified order. If instant collisions are turned off, also calls
    // collision methods for objects which have collided, but block each other (and so
    // are not actually currently overlapping).
    void TriggerCollisions();

    // Must be implemented to determine whether two given objects should block each
    // others' movement. Odd behaviour could result if this relation is not symmetric.
    virtual bool ShouldBlock(const Physical* a, const Physical* b) const = 0;

    template< typename T >
    void GetNearbyPhysicals(std::vector< T* >& output, double x, double y);

private:

    // Size of internal collision cells.
    static const double CELL_SIZE;

    class Sort {
    public:

        bool operator()(const Physical* a, const Physical* b) const;

    };

    typedef std::set            < Physical*, Sort >                   Cell;
    typedef boost::unordered_map< CellCoord, Cell, CellCoord::Hash >  Map;
    typedef std::vector         < std::pair< Physical*, Physical* > > CollisionList;

    bool CheckCollision(Physical* a, Physical* b, const Rect& aRect);
    Cell& GetCellAtPoint(double x, double y);

    friend class Physical;

    Map           _map;
    bool          _instantCollisions;
    long          _leftBound;
    long          _topBound;
    long          _rightBound;
    long          _bottomBound;
    CollisionList _collisions;
};

// A Physical object has a (mobile) collision rectangle and is associated
// with a CollisionWorld instance. Should be subclassed by physical entities.
class Physical {
public:

    // Create physical object associated with given world, and with given
    // initial collision rectangle. Can trigger OnCollision calls if the
    // associated CollisionWorld is set to use instant collisions.
    Physical(CollisionWorld& world, const Rect& rect);

    // Create physical object associated with given world, and with collision
    // rectangle of given size. The top-left corner of the rectangle is set to
    // (0, 0). Can trigger OnCollision calls if the associated CollisionWorld
    // is set to use instant collisions.
    Physical(CollisionWorld& world, double width, double height);

    // Destructor.
    virtual ~Physical();

    // Retrieve current collision rectangle of this object.
    const Rect& GetCollisionRect() const;

    // Set current collision rectangle of this object. Can trigger OnCollision
    // calls if the associated CollisionWorld is set to use instant collisions.
    void SetCollisionRect(const Rect& rect);

    // Set top-left corner of the current collision rectangle of this object.
    // Can trigger OnCollision calls if the associated CollisionWorld is set to
    // use instant collisions.
    void SetPosition(double x, double y);
	void SetPosition(const Vector2D& position) { SetPosition(position.x, position.y); }
	Vector2D GetPosition() const;

    // Set size of the current collision rectangle of this object. The rectangle's
    // centre is preserved. Can trigger OnCollision calls if the associated
    // CollisionWorld is set to use instant collisions.
    void SetSize(double width, double height);

    // Move current collision rectangle of this object. Is blocked by obstacles,
    // with continuous collision detection. Can trigger OnCollision calls if the
    // associated CollisionWorld is set to use instant collisions.
    virtual void Move(double xOffset, double yOffset);
    /*   */ void Move(const Vector2D& vector) { Move(vector.x, vector.y); }

    // Called when this object collides with another. When a collision occurs
    // between objects A and B, both A.OnCollision(B) and B.OnCollision(A) are
    // called, in no specified order.
    virtual void OnCollision(Physical& other) = 0;

private:

    void UpdateWorld();

    typedef CollisionWorld::Cell       Cell;
    typedef std::vector< Physical* >   CollisionList;

    CollisionWorld& _world;
    CollisionList   _collisions;
    Cell*           _cell;
    Rect            _rect;

};

template< typename T >
void CollisionWorld::GetNearbyPhysicals(std::vector< T* >& output, double x, double y)
{
    long lx = long(floor(x / CELL_SIZE));
    long ly = long(floor(y / CELL_SIZE));
    for ( long tx = lx - 2; tx <= lx + 1; tx++ ) {
        for ( long ty = ly - 2; ty <= ly + 1; ty++ ) {
            Cell& cell = _map[CellCoord(tx, ty)];
            for ( Cell::iterator i = cell.begin(); i != cell.end(); i++ ) {
                if ( tx == lx - 2 && ( *i )->GetCollisionRect().right < ( lx - 1 ) * CELL_SIZE )
                    continue;
                if ( ty == ly - 2 && ( *i )->GetCollisionRect().bottom < ( ly - 1 ) * CELL_SIZE )
                    continue;
                assert(dynamic_cast< T* >(*i));
                output.push_back((T*)*i);
            }
        }
    }
}

#endif
