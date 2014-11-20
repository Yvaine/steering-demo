
#include "steering_incl.h"

class Application
{
protected:
    int height;
    int width;
    bool help;

public:
    Application();

    virtual const char* getTitle();
    virtual void initGraphics();
    virtual void setView();
    virtual void deinit();
    virtual void display();
    virtual void update();

    virtual bool key(unsigned char key);
    virtual void resize(int width, int height);
    virtual void mouse(int button, int state, int x, int y);

    void renderActor(const Location &loc);
	void renderSpot(const Vector3 &position);
};

