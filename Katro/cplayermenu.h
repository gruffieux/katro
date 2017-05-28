#ifndef CPLAYERMENU
#define CPLAYERMENU

#include <cscene.h>
#include <cradiobutton.h>
#include "cplayerdlg.h"

class PlayerMenu : public Scene
{
private:
	static const int TOTAL_BOTTOM_DECO = 6;
	static const int TOTAL_BUTTON = 2;
	static const int TOTAL_HOLES_CHOICE = 2;
	Visual *title, *palm, *coconuts, *button[TOTAL_BUTTON], *backButton, *bottomDeco[TOTAL_BOTTOM_DECO], *holeText, *holeNb[TOTAL_HOLES_CHOICE];
	RadioButton *holesChoice[TOTAL_HOLES_CHOICE];
	PlayerDlg *playerDlg;
public:
	PlayerMenu();
	~PlayerMenu();
	void destroy();
	void destroyAni();
	void destroyPlayerDlg();
	void doMouseEvents(Cursor *cursor);
	void init();
	void initAni();
	void initPlayerDlg(int rowCount, Str p1="", Str p2="");
	void play();
	void restoreAni();
	void runAnimations();
};

#endif
