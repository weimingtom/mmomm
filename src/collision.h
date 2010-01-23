#ifndef COLLISION_H_
#define COLLISION_H_
// implementations horrible ineffecient - will change

#include <vector>

class CollisionWorld;
struct Rect {
    Rect( double left, double top, double right, double bottom );

    double left;
    double top;
    double right;
    double bottom;
};

// A Physical object has a (mobile) collision rectangle and is associated
// with a CollisionWorld instance. Should be subclassed by physical entities.
class Physical {
public:

    // Create physical object associated with given world, and with given
    // initial collision rectangle. Can trigger OnCollision calls if the
    // associated CollisionWorld is set to use instant collisions.
    Physical( CollisionWorld& world, const Rect& position );

    // Create physical object associated with given world, and with collision
    // rectangle of given size. The top-left corner of the rectangle is set to
    // (0, 0). Can trigger OnCollision calls if the associated CollisionWorld
    // is set to use instant collisions.
    Physical( CollisionWorld& world, double width, double height );

    // Destructor.
    virtual ~Physical();

    // Retrieve current collision rectangle of this object.
    const Rect& GetCollisionRect() const;

    // Set current collision rectangle of this object. Can trigger OnCollision
    // calls if the associated CollisionWorld is set to use instant collisions.
    void SetCollisionRect( const Rect& rect );

    // Set top-left corner of the current collision rectangle of this object.
    // Can trigger OnCollision calls if the associated CollisionWorld is set to
    // use instant collisions.
    void SetPosition( double x, double y );

    // Set size of the current collision rectangle of this object. The rectangle's
    // centre is preserved. Can trigger OnCollision calls if the associated
    // CollisionWorld is set to use instant collisions.
    void SetSize( double width, double height );

    // Convenience function to move current collision rectangle of this object.
    // Can trigger OnCollision calls if the associated CollisionWorld is set to
    // use instant collisions.
    void Move( double xOffset, double yOffset );

    // Called when this object collides with another. When a collision occurs
    // between objects A and B, both A.OnCollision(B) and B.OnCollision(A) are
    // called, in no specified order.
    virtual void OnCollision( Physical& other ) = 0;

private:

    friend class CollisionWorld;

    CollisionWorld& _world;
    Rect            _rect;

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
    void SetInstantCollisions( bool instantCollisions );

    // Retrieves the current setting for instant collisions.
    bool GetInstantCollisions() const;

    // Checks all objects in this CollisionWorld for collisions. For each collision
    // between objects A and B, both A.OnCollision(B) and B.OnCollision(A) are
    // called, in no specified order.
    void TriggerCollisions() const;

private:

    void TriggerCollisions( Physical* context ) const;

    friend class Physical;
    typedef std::vector< Physical* > Vector;

    Vector _objects;
    bool   _instantCollisions;

};

#endif