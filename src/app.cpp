
#include <stdio.h>
#include <gl/glut.h>
#include "app.h"

Application::Application() : help(false)
{
}

void Application::initGraphics()
{
	g_CGI->init();

    setView();
}

void Application::setView()
{
	g_CGI->setView(width, height);
}

void Application::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
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
	g_CGI->setResize(width, height);
}

void Application::mouse(int button, int state, int x, int y)
{
}

// Implementations

void Application::renderActor(const Location &loc)
{
	g_CGI->drawActor(g_CGI->ORANGE, loc);
	g_CGI->drawText(g_CGI->GREEN, loc.position, GLUT_STROKE_ROMAN, "1234");
}

void Application::renderSpot(const Vector3 &pos)
{
	g_CGI->drawSphere(g_CGI->YELLOW, pos, 1.f);
}

static const char *helpHidden[] = {
    "Press H for help."
};
