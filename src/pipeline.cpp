
#include "pipeline.h"

//////////////////////////////////////////////

Goal::Goal() {
	Clear();
}

void Goal::Clear() {
}

void Goal::setGoal(const Goal& goal) {
	position = goal.position;
	orientation = goal.orientation;
	velocity = goal.velocity;
	rotation = goal.rotation;
}

//////////////////////////////////////////////

float Path::DistanceFromKinematic(const Kinematic &k) {
	return (k.position - goal.position).Length();
}

//////////////////////////////////////////////

Goal GoalTargeter::getGoal() {
	return goal;
}

//////////////////////////////////////////////

float AvoidSpheresConstraint::willViolate(const Path& path, float maxDistance) {
	float distance = FLT_MAX;
	float thisDistance;

	std::list<Sphere>::iterator soi;
	for (soi = obstacles.begin(); soi != obstacles.end(); soi++) {
		thisDistance = willViolate(path, distance, (*soi));
		if (thisDistance < distance) distance = thisDistance;
	}

	return distance;
}

// check for current obstacle
float AvoidSpheresConstraint::willViolate(const Path& path, float maxDistance, const Sphere &obstacle) {
	
	// Alias the kinematic object
	const Kinematic &kinematic = pipe->actor.kinematic;
	
	// Work out where we're going
	Vector3 direction = path.goal.position - kinematic.position;
	
	// Make sure we're moving
	if (direction.SqLength() > 0) {
	
		// Find the distance from the line we're moving along to the obstacle.
		Vector3 movementNormal = direction.Unit();
		Vector3 characterToObstacle = obstacle.position - kinematic.position;
		float distSq = characterToObstacle * movementNormal;
		distSq = characterToObstacle.SqLength() - distSq*distSq;
		
		// Check for intersection
		float radius = obstacle.radius + avoidMargin;
		if (distSq < radius*radius) {
		
			// Find how far along our movement vector the closest pass is
			float distanceToClosest = characterToObstacle * movementNormal;
			
			// Make sure this isn't behind us and is closer than our lookahead.
			if (distanceToClosest > 0 && distanceToClosest < maxDistance) {
				// Find the closest point
				Vector3 closestPoint = kinematic.position + movementNormal*distanceToClosest;
				// Find the point of avoidance
				suggestion.position = obstacle.position + (closestPoint - obstacle.position).Unit() * radius;
				return distanceToClosest;
			}
		}
	}
	return FLT_MAX;
}

Goal AvoidSpheresConstraint::suggest()
{
	return suggestion;
}

//////////////////////////////////////////////

void BasicActor::getPath(Path& path, const Goal& goal)
{
	path.goal = goal;
}

void BasicActor::getSteering(SteeringRequest& output, const Path& path)
{
	seek.target = path.goal.position;
	seek.maxAcceleration = maxAcceleration;
	seek.getSteering(output, kinematic);
}

//////////////////////////////////////////////

SteeringPipe::SteeringPipe() {}

SteeringPipe::~SteeringPipe() {
	targeters.clear();
	constraints.clear();
}

void SteeringPipe::setActor() {

	actor.maxAcceleration = 100.0f;

    actor.kinematic.position.x = randomBinomial(5.0);
    actor.kinematic.position.y = 0;
    actor.kinematic.position.z = randomBinomial(5.0);
    actor.kinematic.orientation = randomReal(_PI);
    actor.kinematic.velocity.x = randomBinomial(1.0);
    actor.kinematic.velocity.y = 0;
    actor.kinematic.velocity.z = randomReal(5.0);
    actor.kinematic.rotation = 0;
}

Kinematic& SteeringPipe::getKinematic() {
	return actor.kinematic;
}

Path& SteeringPipe::getPath() {
	return actor.path;
}

void SteeringPipe::getSteering(SteeringRequest& output) {
	if(targeters.empty()) return;
		
	// set goal
	Goal goal;
	goal.setGoal((*(targeters.begin()))->getGoal());
	///////////////////
		
	actor.getPath(actor.path, goal);

	// Find the constraint that is violated first
		
	float shortestDistance, currentDistance;
	shortestDistance = actor.path.DistanceFromKinematic(actor.kinematic);

	std::list<BaseObstacleConstraint*>::iterator ci;
	for (ci = constraints.begin(); ci != constraints.end(); ci++) {	
		// Check to see if this constraint is violated early than any other.
		currentDistance = (*ci)->willViolate(actor.path, shortestDistance);
			
		if (currentDistance < shortestDistance) {
			shortestDistance = currentDistance;
			// Update the goal and check constraints again.
			goal = (*ci)->suggest();
			// update path
			actor.getPath(actor.path, goal);
		}
	}

	actor.getSteering(output, actor.path);
}
	
void SteeringPipe::registerComponents() {
	
	std::list<GoalTargeter*>::iterator ti;
	
	for (ti = targeters.begin(); ti != targeters.end(); ti++) {
		(*ti)->pipe = this;
	}
	
	std::list<BaseObstacleConstraint*>::iterator ci;
	for (ci = constraints.begin(); ci != constraints.end(); ci++) {
		(*ci)->pipe = this;
	}

	actor.pipe = this;
}

//////////////////////////////////////////////