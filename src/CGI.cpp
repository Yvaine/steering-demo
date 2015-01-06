
#include "CGI.h"
#include <windows.h>

GLfloat  CGI::WHITE[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  CGI::BLACK[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat  CGI::RED[4]    = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  CGI::YELLOW[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
GLfloat  CGI::GREEN[4]  = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat  CGI::BLUE[4]   = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat  CGI::GREY[4]   = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  CGI::ORANGE[4] = { 1.0f, 0.8f, 0.0f, 1.0f };

inline void vectorAsGLVertex(const Vector3 &v)
{
	glVertex3f(v.x, v.y, v.z);
}

inline void drawTriangle (const Vector3& a,
                           const Vector3& b,
                           const Vector3& c)
{
    glBegin (GL_TRIANGLES);
    {
        vectorAsGLVertex (a);
        vectorAsGLVertex (b);
        vectorAsGLVertex (c);
    }
    glEnd ();
}

CGI* CGI::Instance()
{
  static CGI instance;
  return &instance;
}

void CGI::init() {
    glClearColor(0.7f,0.7f,0.7f,1.0f);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
}

void CGI::setView(float width, float height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, float(width)/float(height), 1, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

void CGI::setResize(float width, float height) {
    glViewport(0, 0, width, height);
    setView(width, height);
}

void CGI::drawLine(GLfloat color[4], const Vector3& begin, const Vector3& end) {
	glColor4fv(color);
	glBegin(GL_LINES);
	vectorAsGLVertex(begin);
	vectorAsGLVertex(end);
	glEnd();
}

void CGI::drawSphere(GLfloat color[4], const Vector3& pos, float r) {
	glColor4fv(color);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(r, 12, 12);
	glPopMatrix();
}

void CGI::drawCube(GLfloat color[4], const Vector3& pos, float r) {
	glColor4fv(color);
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(r, 12, 12);
	glPopMatrix();
}

void CGI::drawActor(GLfloat color[4], const Location &loc) {
	glColor4fv(color);
    glPushMatrix();
    glTranslatef(loc.position.x, loc.position.y, loc.position.z);
    glRotatef(loc.orientation * _180_PI, 0.0f, 1.0f, 0.0f);
	glutWireCube(3.0f);

	glTranslatef(0, 0, 1.5);
	glutSolidCube(1.5f);

    glPopMatrix();
}

void CGI::drawGrid(GLfloat color[4], int min, int max, int step) {
	glColor4fv(color);
    glBegin(GL_LINES);
    for (int i = min; i <= max; i += step) {

        glVertex3i(min, -1, i);
        glVertex3i(max, -1, i);

        glVertex3i(i, -1, max);
        glVertex3i(i, -1, min);
    }
    glEnd();
}

void CGI::drawText(GLfloat color[4], const Vector3& pos, void *font, char *string) {
	glPushMatrix();
	glColor4fv(color);
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(-90.f, 0.f, 1.f,0.f);
	glScalef(0.02f, 0.02f, 1.0f);
	for (char *c = string; *c != '\0'; c++) {
			glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}
