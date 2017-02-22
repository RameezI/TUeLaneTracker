
#include "NcursesTUI.h"

#ifndef s32v2xx
NcursesTUI* NcursesTUI::sInstance=NULL;


NcursesTUI::NcursesTUI()
{

	
	/* Setup Signal hadler */
//	struct sigaction lSa;
//	memset(&lSa, 0, sizeof(lSa));
//	lSa.sa_handler = Sighandler;   
	
//	sigaction(SIGWINCH, &lSa, NULL);
		
	
//	initscr();			/* Start curses mode 														 */
//	cbreak();			/* Line buffering disabled, chracters are available without pressing return  */
//	noecho();			/* Keyboard echos disabled   												 */
//	curs_set(FALSE);
	
	
	
	/* Creating Window for StateMachine Info */
	{
		
		
//		int startx, starty, width, height;
//		getmaxyx(stdscr, height, width);

		//height = round((LINES-starty)*0.5);
		//width = round(COLS*0.6);
//		starty = 3;
//		startx = 0;
//		mStateMachine_win = create_newwin(height, width, starty, startx);
//		draw_borders(mStateMachine_win);
		//box(mStateMachine_win, 0 , 0);		/* 0, 0 gives default character for the vertical and horizontal lines */
//		wrefresh(mStateMachine_win);		/* Show that box 													 */
		
	}
	

	
}

void NcursesTUI::updateStateMachine()
{
	
}


NcursesTUI* NcursesTUI::instance()
{
/*	 if (!sInstance) 
		sInstance = new NcursesTUI();   
	return sInstance; */
}






void NcursesTUI::Sighandler(int aSigNo)
{
//	int x, y;
//	getmaxyx(stdscr, y, x);
//	NcursesTUI* THIS = instance();
	
//	wresize(THIS->mStateMachine_win, y, x);
//	wclear(THIS->mStateMachine_win);
	//box(THIS->mStateMachine_win, 0 , 0);		/* 0, 0 gives default character for the vertical and horizontal lines */
//	THIS->draw_borders(THIS->mStateMachine_win);
//	wrefresh(THIS->mStateMachine_win);		   /* Show that box */
	//struct winsize W;
	//ioctl(STDOUT_FILENO,TIOCGWINSZ,&W);	
	
	
	//int Row = W.ws_row;
	//int Col = W.ws_col;
	//resizeterm(W.ws_row, W.ws_col);
	
	
	//printw("Row count: %d and Column Count: %d", Row, Col);
	//refresh();
  //sStatus = SigStatus::STOP;
} 


WINDOW* NcursesTUI::create_newwin(int height, int width, int starty, int startx)
{	
//	WINDOW *local_win;
//	local_win = newwin(height, width, starty, startx);
//	return local_win;
}



void NcursesTUI::draw_borders(WINDOW *screen) {
//  int x, y, i;

//  getmaxyx(screen, y, x);

	// 4 corners
//  mvwprintw(screen, 0, 0, "+");
//  mvwprintw(screen, y - 1, 0, "+");
//  mvwprintw(screen, 0, x - 1, "+");
//  mvwprintw(screen, y - 1, x - 1, "+");

	// sides
//  for (i = 1; i < (y - 1); i++) {
//    mvwprintw(screen, i, 0, "|");
//    mvwprintw(screen, i, x - 1, "|");
//  }

	// top and bottom
//  for (i = 1; i < (x - 1); i++) {
//    mvwprintw(screen, 0, i, "-");
//    mvwprintw(screen, y - 1, i, "-");
//  }
   
}


NcursesTUI::~NcursesTUI()
{
	

	
	
	
}

#endif