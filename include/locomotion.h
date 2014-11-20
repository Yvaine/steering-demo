
#ifndef __LOCOMOTION_H__
#define __LOCOMOTION_H__

#include "vmath.h"

class SteeringRequest {
public:
	Vector3 linear;
    float angular;

public:
	SteeringRequest() : angular(0) {}
	SteeringRequest(const Vector3& linear, float angular = 0)
            : linear(linear), angular(angular) {}
	
	void Clear() {
		linear.Clear();
		angular = 0;
	}
	
	bool operator == (const SteeringRequest& other) const {
		return linear == other.linear &&  angular == other.angular;
	}
	
	bool operator != (const SteeringRequest& other) const {
		return !(*this == other);
	}
	
	float SqLength() {
		return linear.SqLength() + angular*angular;
	}
	
	float Length() {
		return sqrtf(SqLength());
	}
};

class Location {
public:
	Vector3 position;
	float orientation;

public:
	Location() : orientation(0.0f) {}
	
	Location(const Vector3& position)
            : position(position), orientation(0.0f) {}
	
	Location(const Vector3& position, float orientation)
            : position(position), orientation(orientation) {}
	
	Location(float x, float y, float z, float orientation)
            : position(x, y, z), orientation(orientation) {}
	
	Location& operator = (const Location& other) {
		position = other.position;
		orientation = other.orientation;
		return *this;
	}
	
	void Clear() {
		position.Clear();
		orientation = 0.0f;
	}
	
	bool operator == (const Location& other) const {
		return position == other.position && orientation == other.orientation;
	}
	
	bool operator != (const Location& other) const {
		return !(*this == other);
	}
	
	void integrate(float dt);
	void integrate(const SteeringRequest& steer, float dt);
	
	void setOrientationFromVelocity(const Vector3& velocity);
	Vector3 getOrientationAsVector() const;
};

class Kinematic : public Location {
public:
	Vector3 velocity;
	float rotation;
	
	Kinematic() : Location(), velocity(), rotation(0) {}
	
	Kinematic(const Vector3& position, const Vector3& velocity)
            : Location(position), velocity(velocity), rotation(0) {}
	
	Kinematic(const Location& loc, const Vector3& velocity)
            : Location(loc), velocity(velocity), rotation(0) {}
	
	Kinematic(const Location& loc)
            : Location(loc), velocity(), rotation(0) {}
	
	Kinematic(const Vector3& position, float orientation,
                  const Vector3& velocity, float avel)
            : Location(position, orientation), velocity(velocity), rotation(avel) {}
	
	void Clear() {
		Location::Clear();
		velocity.Clear();
		rotation = 0.0f;
	}
	
	bool operator == (const Kinematic& other) const {
		return position == other.position &&
                   orientation == other.orientation &&
                   velocity == other.velocity &&
                   rotation == other.rotation;
	}
	
	bool operator != (const Kinematic& other) const {
		return !(*this == other);
	}
	
	Kinematic& operator = (const Location& other) {
		orientation = other.orientation;
		position = other.position;
		return *this;
	}
	
	Kinematic& operator = (const Kinematic& other) {
		orientation = other.orientation;
        position = other.position;
        velocity = other.velocity;
        rotation = other.rotation;
        return *this;
    }
	
	void operator += (const Kinematic&);
	void operator -= (const Kinematic&);
	void operator *= (float f);

	void integrate(float dt);
	void integrate(const SteeringRequest& steer, float dt);
	void integrate(const SteeringRequest& steer, float drag, float dt);
	void integrate(const SteeringRequest& steer, const SteeringRequest& drag, float dt);
	
	void trimMaxSpeed(float speed);
	void setOrientationFromVelocity();
};

#endif // __LOCOMOTION_H__
