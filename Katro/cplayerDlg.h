#ifndef CPLAYERDLG
#define CPLAYERDLG

#include <cpanel.h>
#include <ctextfield.h>
#include <cradiobutton.h>

class PlayerDlg : public Panel
{
private:
	static const int TOTAL_LEVEL = 3;
	TextField *p1Field, *p2Field;
	RadioButton *diffRbn[TOTAL_LEVEL];
	Visual *cancelButton;
public:
	PlayerDlg(int rowCount=5);
	~PlayerDlg();
	bool checkFields();
	void destroyAni();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	TextField* getP1Field() {return p1Field;}
	TextField* getP2Field() {return p2Field;}
	void Init();
	void initAni();
	bool Refresh();
	void restoreAni();
	bool Run(Item *Cursor=NULL);
};

#endif
