
#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "steering.h"
#include <list>
#include <limits>
using std::list;

//////////////////////////////////////////////

struct Goal {
	Vector3 position;
	float orientation;
	Vector3 velocity;
	float rotation;

public:
	Goal();
	void Clear();
	
	void setGoal(const Goal& goal);
};

//////////////////////////////////////////////

class Path {
public:
	Goal goal;
	float DistanceFromKinematic(const Kinematic &k);
};

//////////////////////////////////////////////

class SteeringPipe;

// base class for components
class SteeringPipeComponent {
protected:
	friend class SteeringPipe;
	SteeringPipe *pipe;
};


// goal target
class GoalTargeter : public SteeringPipeComponent {
public:
	Goal goal;
	virtual Goal getGoal();
};

// base class for obstacle
class BaseObstacleConstraint : public SteeringPipeComponent {
protected:
	Goal suggestion; // obstacle avoidance goal

public:
	virtual float willViolate(const Path& path, float maxDistance) = 0;
	virtual Goal suggest() = 0;
};

// obstacle logic for spheres
class AvoidSpheresConstraint : public BaseObstacleConstraint {

	// check for current obstacle
	float willViolate( const Path& path, float maxDistance, const Sphere &obstacle);

public:
	std::list<Sphere> obstacles;
	
	float avoidMargin; // offset
	
	virtual float willViolate(const Path& path, float maxDistance);
	virtual Goal suggest();
};

// mooving actor
class BasicActor : public SteeringPipeComponent {
public:	
	/** The seek behaviour performs the actuation for us. */
	Seek seek;

	/** kinematic movemet */
	Kinematic kinematic;
	
	/** path to goal */
	Path path;

	/** goal target */
	GoalTargeter targeter;

	/** The maximum acceleration force the character can muster. */
	float maxAcceleration;

	BasicActor() {
	}
	
	virtual void getPath(Path& path, const Goal& goal);
	virtual void getSteering(SteeringRequest& output, const Path& path);
};

//////////////////////////////////////////////
// MAIN STEERING SYSTEM

// This is the size of the world in both directions from 0 (i.e. from
// -WORLD_SIZE to +WORLD_SIZE)
#define WORLD_SIZE 50

// This is the gap between grid lines
#define GRID_SIZE 5

// The number of obstacles in the simulation
#define OBSTACLES 35

class SteeringPipe {
public:	

	BasicActor actor;
	
	std::list<GoalTargeter*> targeters;
	
	AvoidSpheresConstraint *constraint;
	std::list<BaseObstacleConstraint*> constraints;
	
	SteeringPipe();
	~SteeringPipe();
	
	void setActor();
	Kinematic& getKinematic();
	Path& getPath();
	GoalTargeter *targeter;

	void popTarget() {
		targeters.pop_front();
	}

	void initObstacles() {
		constraint = new AvoidSpheresConstraint();
		targeter = new GoalTargeter;
		targeters.push_back(targeter);
	}

	AvoidSpheresConstraint *getObstacles() {
		return constraint;
	}

	GoalTargeter* getGoalTargeter() {
		return targeter;
	}
	
	void getSteering(SteeringRequest& output);

	/**
	* Call this method to initialise all the components after you have
	* added them, and before you call getSteering. This only needs to
	* be done after you change the lists of targeters, decomposers or
	* constraints, or if you change actor.
	*/
	void registerComponents();
};

//////////////////////////////////////////////

#endif // __PIPELINE_H__