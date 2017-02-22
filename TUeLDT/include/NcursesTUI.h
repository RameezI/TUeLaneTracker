
#ifndef NCURSESTUI_H
#define NCURSESTUI_H

#include<ncurses.h>
#include <signal.h>
#include "State.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#ifndef s32v2xx
class NcursesTUI
{
	
private:
	NcursesTUI();
	static NcursesTUI* sInstance;
	WINDOW *mStateMachine_win, mLog_win, mProfiler_win;
	WINDOW* create_newwin(int height, int width, int starty, int startx);
	void draw_borders(WINDOW *screen);
	static void	Sighandler(int aSigNo);
	
public:
static NcursesTUI* instance();
void updateStateMachine();
void updateProfile();
void addLog();




~NcursesTUI();

};

#endif
#endif  // NCURSESTUI_H