#ifndef CTITLESCENE
#define CTITLESCENE

#include <cscene.h>

class TitleScene : public Scene
{
private:
	Visual *title;
public:
	TitleScene();
	~TitleScene();
	void destroy();
	void destroyAni();
	void doMouseEvents(Cursor *cursor);
	void init();
	void initAni();
	void play();
	void restoreAni();
	void runAnimations();
};

#endif
