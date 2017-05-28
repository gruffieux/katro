#ifndef CRULESMENU
#define CRULESMENU

#include <cscene.h>

class RulesMenu : public Scene
{
private:
	static const int TOTAL_BOTTOM_DECO = 6;
	Visual *title, *backButton, *text, *bottomDeco[TOTAL_BOTTOM_DECO];
public:
	RulesMenu();
	~RulesMenu();
	void destroyAni();
	void doMouseEvents(Cursor *cursor);
	void init();
	void initAni();
	void play();
	void restoreAni();
	void runAnimations();
};

#endif
