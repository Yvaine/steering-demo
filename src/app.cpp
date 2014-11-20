
#include <stdio.h>
#include <gl/glut.h>
#include "app.h"

Application::Application() : help(false)
{
}

void Application::initGraphics()
{
    glClearColor(0.7,0.7,0.7,1.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    setView();
}

void Application::setView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, float(width)/float(height), 1, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void Application::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    glVertex2i(1, 1);
    glVertex2i(639, 319);
    glEnd();
}

const char* Application::getTitle()
{
    return "Artificial Intelligence for Games Demo N";
}

void Application::deinit()
{
}

void Application::update()
{
    glutPostRedisplay();
}

bool Application::key(unsigned char key)
{
    switch(key)
    {
    case 'h': case 'H':
        help = !help;
        return true;
    }

    return false;
}

void Application::resize(int width, int height)
{
    // Avoid the divide by zero.
    if (height <= 0) height = 1;

    // Set the internal variables and update the view
    Application::width = width;
    Application::height = height;
    glViewport(0, 0, width, height);
    setView();
}

void Application::mouse(int button, int state, int x, int y)
{
}

// Implementations

void Application::renderActor(const Location &loc)
{
	glColor3f(1.0f, 0.8f, 0.0f);
    glPushMatrix();
    glTranslatef(loc.position.x, loc.position.y, loc.position.z);
    glRotatef(loc.orientation * _180_PI, 0.0f, 1.0f, 0.0f);
	glutWireCube(3.0f);

	glTranslatef(0, 0, 1.5);
	glutSolidCube(1.5f);

    glPopMatrix();
}

void Application::renderSpot(const Vector3 &pos)
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(0.5f, 12, 12);
	glPopMatrix();
}

static const char *helpHidden[] = {
    "Press H for help."
};
