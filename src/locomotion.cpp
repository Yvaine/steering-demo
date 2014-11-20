
#include "locomotion.h"

//////////////////////////////////////////////

void Location::integrate(float dt) {
	
}

void Location::integrate(const SteeringRequest& steer, float dt) {
	position += steer.linear * dt;
	orientation += steer.angular * dt; 
	orientation = fmodf(orientation, _2PI);	
}

void Location::setOrientationFromVelocity(const Vector3& velocity) {
	// If we haven't got any velocity, then we can do nothing.
	if (velocity.SqLength() > 0) {
		 orientation = atan2(velocity.x, velocity.z);
	}
}

Vector3 Location::getOrientationAsVector() const {
	return Vector3(sinf(orientation), 0, cosf(orientation));
}

//////////////////////////////////////////////

void Kinematic::integrate(float dt) {
	position += velocity * dt;
	orientation += rotation * dt; 
	orientation = fmodf(orientation, _2PI);	
}

void Kinematic::integrate(const SteeringRequest& steer, float dt) {
        integrate(dt);
        velocity += steer.linear*dt;
        rotation += steer.angular*dt;
}

void Kinematic::integrate(const SteeringRequest& steer,
                              float drag,
                              float dt) {
	integrate(dt);

	// Slowing velocity and rotational velocity
	drag = powf(drag, dt);
	velocity *= drag*drag;
	rotation *= drag;

	velocity += steer.linear*dt;
	rotation += steer.angular*dt;
}

void Kinematic::integrate(const SteeringRequest& steer,
                              const SteeringRequest& drag,
                              float dt) {
	integrate(dt);

	velocity.x *= powf(drag.linear.x, dt);
	velocity.y *= powf(drag.linear.y, dt);
	velocity.z *= powf(drag.linear.z, dt);
	rotation *= powf(drag.angular, dt);

	velocity += steer.linear * dt;
	rotation += steer.angular * dt;
}

void Kinematic::operator += (const Kinematic& other) {
	position += other.position;
	velocity += other.velocity;
	rotation += other.rotation;
	orientation += other.orientation;
}

void Kinematic::operator -= (const Kinematic& other) {
	position -= other.position;
	velocity -= other.velocity;
	rotation -= other.rotation;
	orientation -= other.orientation;
}

void Kinematic::operator *= (float f) {
	position *= f;
	velocity *= f;
	rotation *= f;
	orientation *= f;
}

void Kinematic::trimMaxSpeed(float maxSpeed) {
	if (velocity.SqLength() > maxSpeed*maxSpeed) {
		velocity.Normalise();
		velocity *= maxSpeed;
	}
}

void Kinematic::setOrientationFromVelocity() {
	// If we haven't got any velocity, then we can do nothing.
	if (velocity.SqLength() > 0) {
		orientation = atan2(velocity.x, velocity.z);
	}
}

//////////////////////////////////////////////