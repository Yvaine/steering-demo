
#include "steering.h"

//////////////////////////////////////////////

void Seek::getSteering(SteeringRequest& output, const Kinematic &k) {
	// First work out the direction
	output.linear = target;
	output.linear -= k.position;

	// If there is no direction, do nothing
	if (output.linear.SqLength() > 0) {
		output.linear.Normalise();
		output.linear *= maxAcceleration;
	}
}

//////////////////////////////////////////////