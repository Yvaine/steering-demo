
#ifndef __STEERING_H__
#define __STEERING_H__

#include "locomotion.h"

//////////////////////////////////////////////

class SteeringBehaviour {
public:
	virtual void getSteering(SteeringRequest& output, const Kinematic &k) = 0;
};

class Seek : public SteeringBehaviour {
public:
	Vector3 target;
	float maxAcceleration;
	virtual void getSteering(SteeringRequest& output, const Kinematic &k);
};

//////////////////////////////////////////////

#endif // __STEERING_H__