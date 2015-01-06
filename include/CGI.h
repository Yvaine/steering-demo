
#ifndef _CGI_H_
#define _CGI_H_

#include <gl/glut.h>
#include "locomotion.h"

#define g_CGI CGI::Instance()

class CGI {
public:
	static GLfloat WHITE[4];
	static GLfloat BLACK[4];
	static GLfloat RED[4];
	static GLfloat YELLOW[4];
	static GLfloat GREEN[4];
	static GLfloat BLUE[4];
	static GLfloat GREY[4];
	static GLfloat ORANGE[4];

public:
	static void init();
	static void setView(float width, float height);
	static void setResize(float width, float height);

	static void drawLine(GLfloat color[4], const Vector3& begin, const Vector3& end);
	static void drawSphere(GLfloat color[4], const Vector3& pos, float r);
	static void	drawCube(GLfloat color[4], const Vector3& pos, float r);
	static void drawActor(GLfloat color[4], const Location &loc);
	static void drawActorTri(GLfloat color[4], const Location &loc);
	static void drawGrid(GLfloat color[4], int min, int max, int step);
	static void drawText(GLfloat color[4], const Vector3& pos, void *font, char *string);

	static CGI* Instance();
	~CGI(){}
private:
	CGI() {}
    CGI(const CGI &) {}
    CGI& operator=(const CGI &) {}
};

#endif