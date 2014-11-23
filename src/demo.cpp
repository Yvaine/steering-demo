#include <stdlib.h>
#include <stdio.h>

#include <gl/glut.h>
#include "steering_incl.h"

#include "app.h"

/**
 * The main demo class.
 */
class SteeringPipelineDemo : public Application
{
	/** Holds the obstacles */
	Sphere *obstacles;
	///////////////////////////////////////////

    /** Holds the steering pipeline. */
    SteeringPipe *pipe;
	///////////////////////////////////////////

public:
    SteeringPipelineDemo();
    virtual ~SteeringPipelineDemo();

    virtual bool key(unsigned char key);
    virtual void update();
    virtual void display();
    virtual const char* getTitle();

	virtual void createRandomGoal();
	virtual void skipObstacles();
};


void SteeringPipelineDemo::skipObstacles() 
{
	for (unsigned i = 0; i < OBSTACLES; i++) {
		obstacles[i].position.x = -WORLD_SIZE + 4 + randomReal(2*WORLD_SIZE);
		obstacles[i].position.y = (float)0;
		obstacles[i].position.z = -WORLD_SIZE + 4 + randomReal(2*WORLD_SIZE);
		obstacles[i].radius = randomReal((float)2.0) + (float)2.0;
	}

	pipe->getObstacles()->obstacles.clear();

	for (unsigned i = 0; i < OBSTACLES; i++) 
	{
		pipe->getObstacles()->obstacles.push_back(*(obstacles+i));
	}
}

SteeringPipelineDemo::SteeringPipelineDemo()
    :
    Application()
{
	// Create the obstacles
	obstacles = new Sphere[OBSTACLES];
	// Set up the steering pipe
	pipe = new SteeringPipe;
	pipe->initObstacles();

	skipObstacles();

	pipe->setActor();

	pipe->getObstacles()->avoidMargin = (float)2.0;
	pipe->constraints.push_back(pipe->getObstacles());

	pipe->registerComponents();

	createRandomGoal();
}

SteeringPipelineDemo::~SteeringPipelineDemo()
{
	delete pipe;

	delete[] obstacles;
}

static const char *defaultHelp[] = {
    "Obstacles Demo, B - skip Obstacles, N - Choose a new goal",
    "H - Toggle help.",
	"B - skip Obstacles",
	"N - Choose a new goal"
};

const char* SteeringPipelineDemo::getTitle()
{
    return defaultHelp[0];
}

bool SteeringPipelineDemo::key(unsigned char key)
{
    if (Application::key(key)) return true;

    switch(key) {

	case 'n': case 'N':
		createRandomGoal();
		return true;

	case 'b' : case 'B':
		skipObstacles();
		return true;
	}

    return false;
}


inline void vectorAsGLVertex(const Vector3 &v)
{
	glVertex3f(v.x, v.y, v.z);
}


void SteeringPipelineDemo::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(-53.0f, 53.0f, 0.0f,
              0.0f, -30.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    // Draw the characters.
    renderActor(pipe->getKinematic());

	// Draw the current goal
	renderSpot(pipe->getGoalTargeter()->goal.position);

	// Draw the path
	const Path & path = pipe->getPath();
	glColor3f(0.0f, 0.6f, 0.0f);
	glBegin(GL_LINES);
	vectorAsGLVertex(path.goal.position);
	vectorAsGLVertex(pipe->getKinematic().position);
	glEnd();

	// Draw the obstacles
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColor3f(0.4f, 0.4f, 0.4f);
	for (unsigned i = 0; i < OBSTACLES; i++) {
		const Sphere &o = obstacles[i];

		glPushMatrix();
		glTranslatef(o.position.x, o.position.y, o.position.z);
		glutSolidSphere(o.radius, 12, 12);
		glPopMatrix();
	}
	glDisable(GL_LIGHTING);

    // Draw some scale lines
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    for (int i = -WORLD_SIZE; i <= WORLD_SIZE; i += GRID_SIZE) {

        glVertex3i(-WORLD_SIZE, -1, i);
        glVertex3i(WORLD_SIZE, -1, i);

        glVertex3i(i, -1, WORLD_SIZE);
        glVertex3i(i, -1, -WORLD_SIZE);
    }
    glEnd();
}

#define TRIM_WORLD(var) \
    if (var < -WORLD_SIZE) var = WORLD_SIZE; \
    if (var > WORLD_SIZE) var = -WORLD_SIZE;

void SteeringPipelineDemo::update()
{
    float dt = TimingData::get().lastFrameDuration * 0.001f;
    SteeringRequest steer;

    pipe->getSteering(steer);

    // Update the kinematic
    pipe->getKinematic().integrate(steer, (float)0.1, dt);
    pipe->getKinematic().setOrientationFromVelocity();

    // Check for maximum speed
    pipe->getKinematic().trimMaxSpeed((float)20.0);

    // Keep in bounds of the world
    TRIM_WORLD(pipe->getKinematic().position.x);
    TRIM_WORLD(pipe->getKinematic().position.z);

	// Avoid penetration of obstacles
	for (unsigned i = 0; i < OBSTACLES; i++)
	{
		Sphere &o = obstacles[i];
		float distance = pipe->getKinematic().position.distance(o.position);
		if (distance < o.radius + 1.0f)
		{
			Vector3 offset = o.position - pipe->getKinematic().position;
			offset.Normalise();
			offset *= o.radius + 1.0f;
			pipe->getKinematic().position = o.position - offset;
		}
	}

	// Check for proximity to the goal, and create a new one if we are near.
	if (pipe->getKinematic().position.distance(pipe->getGoalTargeter()->goal.position) < 2.0f)
	{
		createRandomGoal();
	}

    glutPostRedisplay();
}

void SteeringPipelineDemo::createRandomGoal()
{
	bool okay;
	do 
	{
		okay = true;

		// Pick a random location
		pipe->getGoalTargeter()->goal.position = Vector3(
			randomBinomial(WORLD_SIZE),
			0,
			randomBinomial(WORLD_SIZE)
			);

		// Make sure it is out of reach of any obstacle
		for (unsigned i = 0; i < OBSTACLES; i++)
		{
			float distance = 
				(pipe->getGoalTargeter()->goal.position - obstacles[i].position).Length();

			if (distance < obstacles[i].radius) 
			{
				okay = false;
				break;
			}
		}
	} while (!okay);
}


Application* getApplication()
{
    return new SteeringPipelineDemo();
}
