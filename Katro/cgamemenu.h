#ifndef CGAMEMENU
#define CGAMEMENU

#include <cscene.h>

class GameMenu : public Scene
{
private:
	static const int TOTAL_BOTTOM_DECO = 6;
	Visual *title, *palm, *coconuts, *backButton, *bottomDeco[TOTAL_BOTTOM_DECO];
	List buttons;
public:
	GameMenu();
	~GameMenu();
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
