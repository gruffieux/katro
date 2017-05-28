#ifndef CMAINMENU
#define CMAINMENU

#include <cscene.h>

class MainMenu : public Scene
{
private:
	static const int TOTAL_BOTTOM_DECO = 6;
	Visual *title, *palm, *board, *coconuts, *playButton, *rulesButton, *bottomDeco[TOTAL_BOTTOM_DECO];
public:
	MainMenu();
	~MainMenu();
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
