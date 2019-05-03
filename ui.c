#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include "ui.h"
#include "data.h"
#include "gpl.h"

void byebye(int sig)
{
	fprintf(stderr,"*** NMICROCODER %i.%i%i\n",MAJOR_VERSION,MINOR_VERSION,REVISION);
	fprintf(stderr,"*** (C)opyleft 2004 by Dettus\n");
	fprintf(stderr,"*** dettus@matrixx-bielefeld.de\n\n");
	fprintf(stderr,"(start it with nMicrocoder -gpl to see the license)\n\n\n");
	ende();
	exit(sig);
}
int printable(int ch)
{
	if (ch>='a' && ch<='z') return 1;
	if (ch>='A' && ch<='Z') return 1;
	if (ch>='0' && ch<='9') return 1;
	if (ch=='.' || ch==',' || ch==';' || ch==':' || ch=='-' || ch=='_' || ch=='$') return 1;
	if (ch==' ') return 1;
	return 0;
}
int getch2()
{
	int ch=getch();
	if (ch==KEY_CANCEL) ch=KEY_F(11);
	if (ch==KEY_HELP) ch=KEY_F(12);
		if (ch==27)
		{
// TODO: dies ist ein quick and dirty hack.
			ch=getch();
			if (ch==91) {
				timeout(0);
				ch=getch();
				if (ch==55) ch=KEY_A1;
				if (ch==56) ch=KEY_C1;
				timeout(-1);	
			}
			timeout(0);
			getch();
			getch();
			getch();
			getch();
			getch();
			getch();
			timeout(-1);
			
		}
	lastkey=ch;
	return ch;
	
}
void init_colors()
{
	int i,j;
	init_pair(63, COLOR_BLACK, COLOR_BLACK);
	attrs[COLOR_BRACKETS]=COLOR_PAIR(63)+A_BOLD;
	init_pair(7,    COLOR_BLACK, COLOR_WHITE);
	attrs[COLOR_INPUT]=COLOR_PAIR(7);
	init_pair(32,     COLOR_BLUE,  COLOR_BLACK);
	attrs[COLOR_ROWS]=COLOR_PAIR(32)+A_BOLD;
	init_pair(32,     COLOR_BLUE,  COLOR_BLACK);
	attrs[COLOR_COLS]=COLOR_PAIR(32)+A_BOLD;
	init_pair(7,   COLOR_BLACK, COLOR_WHITE);
	attrs[COLOR_CURSOR]=COLOR_PAIR(7);
	init_pair(56,     COLOR_WHITE, COLOR_BLACK);
	attrs[COLOR_TEXT]=COLOR_PAIR(56)+A_BOLD;
	init_pair(48,	  COLOR_CYAN,  COLOR_BLACK);
	attrs[COLOR_MENU]=COLOR_PAIR(48);
	init_pair(6,  COLOR_BLACK, COLOR_CYAN);
	attrs[COLOR_MENU_HI]=COLOR_PAIR(6);
	init_pair(24,   COLOR_YELLOW,COLOR_BLACK);
	attrs[COLOR_MENU_HOTKEY]=COLOR_PAIR(24)+A_BOLD;
	init_pair(30,COLOR_YELLOW,COLOR_CYAN);
	attrs[COLOR_MENU_HOTKEY_HI]=COLOR_PAIR(30)+A_BOLD;
	init_pair(32, COLOR_BLUE,COLOR_BLACK);
	attrs[COLOR_FRAME]=COLOR_PAIR(32)+A_BOLD;
	init_pair(6, COLOR_BLACK, COLOR_CYAN);
	attrs[COLOR_HEADLINE]=COLOR_PAIR(6);
	init_pair(32,  COLOR_BLUE, COLOR_BLACK);
	attrs[COLOR_FOLD]=COLOR_PAIR(32);

/*        for (i=0;i<8;i++)
        {
                for (j=0;j<8;j++)
                {
                        init_pair(16+(i*8)+j,i,j);
                }
        }
*/

}
char *input2(WINDOW *parent,int y,int x,int len,const char *text,int max,int special,int firstch)
{
	int cursorpos=0;
	int offset=0;
	int l=0;
	char *newtext;
	char *textfield;
	char *spaces;
	char *tmp;
	int i;
	int ch=0;
	WINDOW *inputfield=newpad(1,len);
	WINDOW *fulltext=newpad(1,max);
	wattrset(inputfield,attrs[COLOR_INPUT]);
	wattrset(parent,attrs[COLOR_INPUT]);


	l=strlen(text)+1;
		
	newtext=malloc(l);
	strncpy(newtext,text,l);
	spaces=malloc(max+1);
	for (i=0;i<max;i++)
	{
		spaces[i]=' ';
	}
	spaces[max]=0;
	textfield=malloc(l);
	if (strlen(newtext)<len)
	{
		offset=0;
		cursorpos=strlen(newtext);
	} else {
		cursorpos=len;
		offset=strlen(newtext)-len;
	}
	while (ch!=KEY_ESC && ch!=KEY_RETURN)
	{
		free(textfield);	
		textfield=malloc(max+1);
		strncpy(textfield,spaces,max+1);
		strncpy(textfield,newtext,strlen(newtext));

		wattrset(fulltext,attrs[COLOR_INPUT]);
		mvwprintw(fulltext,0,0,"%s",textfield);
		wrefresh(fulltext);	
		
//		copywin(fulltext,parent,0,offset,y,x,y,x+len,1);
		wmove(parent,y,x);
		for (i=0;i<max;i++)
		{
			if (i>=offset && i<=offset+len) waddch(parent,textfield[i]);
		}

		wmove(parent,y,x+cursorpos);
		wrefresh(inputfield);
		wrefresh(parent);
		if (firstch==0) ch=getch2(); else {ch=firstch;firstch=0;}
		if (special==1 && (ch==KEY_UP || ch==KEY_DOWN || ch==KEY_LEFT || ch==KEY_RIGHT || ch==KEY_A1 || ch==KEY_C1)) {ch=KEY_RETURN;}
		if (ch==KEY_A1)
		{
			cursorpos=0;
			offset=0;
			ch=0;
		}
		if (ch==KEY_C1)
		{
			if (strlen(newtext)<len)
			{
				offset=0;
				cursorpos=strlen(newtext);
			} else {
				cursorpos=len;
				offset=strlen(newtext)-len;
			}
	
		}
	
		if (ch==KEY_BACKSPACE && cursorpos+offset!=0) {
			tmp=malloc(strlen(newtext));
			for (i=0;i<=strlen(newtext);i++)
			{
				if (i<cursorpos+offset) tmp[i]=newtext[i];
				else tmp[i-1]=newtext[i];
			}
			tmp[strlen(newtext)-1]=0;
			free(newtext);
			newtext=malloc(strlen(tmp)+1);
			strncpy(newtext,tmp,strlen(tmp)+1);
			free(tmp);
			ch=KEY_LEFT;			
		}
		if (ch==KEY_DELETE && cursorpos+offset!=strlen(newtext)) {
			tmp=malloc(strlen(newtext));
			for (i=0;i<=strlen(newtext);i++)
			{
				if (i<=cursorpos+offset) tmp[i]=newtext[i];
				else tmp[i-1]=newtext[i];
			}
			tmp[strlen(newtext)-1]=0;
			free(newtext);
			newtext=malloc(strlen(tmp)+1);
			strncpy(newtext,tmp,strlen(tmp)+1);
//			ch=KEY_LEFT;			
		}
		if (printable(ch) && strlen(newtext)<max)
		{	
			tmp=malloc(strlen(newtext)+2);
			for (i=0;i<strlen(newtext);i++)
			{
				if (i<cursorpos+offset) tmp[i]=newtext[i];
				else tmp[i+1]=newtext[i];
			}
			tmp[cursorpos+offset]=ch;
			tmp[strlen(newtext)+1]=0;
			free(newtext);
			newtext=malloc(strlen(tmp)+1);
			strncpy(newtext,tmp,strlen(tmp)+1);			
			free(tmp);	
			ch=KEY_RIGHT;
		}
		if (ch==KEY_LEFT) {
			cursorpos--;
			if (cursorpos<0) {
				cursorpos=0;
				offset--;
				if (offset<0) offset=0;
			}
		}
		if (ch==KEY_RIGHT && cursorpos+offset<strlen(newtext)) {
			cursorpos++;
			if (cursorpos>len) {	
				cursorpos=len;
				offset++;
				if (offset>strlen(newtext)-len) offset=strlen(newtext)-len;
			}
		}
		if (ch!=KEY_ESC && ch!=KEY_RETURN) ch=0;
	}
	if (ch==KEY_ESC)
	{
		free(newtext);
		newtext=malloc(strlen(text)+1);
		strncpy(newtext,text,strlen(text)+1);
	}	
	
	wattrset(parent,attrs[COLOR_TEXT]);
	wmove(parent,0,0);
	mvwaddch(parent,0,0,'=');
	for (i=0;i<max;i++)
	{
//		if (i>=0 && i<=len) mvwaddch(parent,y,x+i,' ');
	}
//	wrefresh(parent);
//	mvwaddch(parent,0,0,' ');
//	wrefresh(parent);
//	mvwaddch(parent,1,1,'=');
	
	for (i=0;i<max;i++)
	{
		if (i>=0 && i<=len) mvwaddch(parent,y,x+i,textfield[i]);
	}
	wrefresh(parent);
	mvwaddch(parent,0,0,' ');
	wmove(parent,0,0);
	wrefresh(parent);
	wmove(parent,y,x);
	wmove(parent,y,x);
	wmove(parent,y,x);
	wmove(parent,y,x);
	wmove(parent,y,x);
	wmove(parent,y,x);
	wmove(parent,y,x);
	wmove(parent,y,x);
	wmove(parent,y,x);
	wmove(parent,y,x);
	return newtext;

}
char *input(WINDOW *parent,int y,int x,int len,const char *text,int max)
{
	return input2(parent,y,x,len,text,max,0,0);
}

void new_menu(int updown_nleftright)
{
	MENU_num=0;
	MENU_pos=0;
	MENU_updown_nleftright=updown_nleftright;
}
void menu_item(int pos,int y,int x,char *text, char hotkey1,char hotkey2,char hotkey3)
{
	if (pos>=MENU_num) MENU_num=pos+1;
	MENU_xpos[pos]=x;
	MENU_ypos[pos]=y;
	free(MENU_text[pos]);
	MENU_text[pos]=malloc(strlen(text)+1);
	strncpy(MENU_text[pos],text,strlen(text)+1);
	MENU_hotkey1[pos]=hotkey1;
	MENU_hotkey2[pos]=hotkey2;
	MENU_hotkey3[pos]=hotkey3;
}
int menu_show(WINDOW *parent)
{
	int i;
	int j;
	int old_pos=0;	
	int ch=0;
	for (i=0;i<MENU_num;i++)
	{
		wattrset(parent,attrs[COLOR_MENU]);	
		wmove(parent,MENU_ypos[i],MENU_xpos[i]);
		for (j=0;j<strlen(MENU_text[i]);j++)
		{
			
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU_HOTKEY]); 
			if (MENU_text[i][j]!='%') waddch(parent,MENU_text[i][j]);
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU]); 
		
			
		}		
	}
	while (ch!=KEY_RETURN)
	{
		i=old_pos;
		wattrset(parent,attrs[COLOR_MENU]);	
		wmove(parent,MENU_ypos[i],MENU_xpos[i]);
		for (j=0;j<strlen(MENU_text[i]);j++)
		{
			
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU_HOTKEY]); 
			if (MENU_text[i][j]!='%') waddch(parent,MENU_text[i][j]);
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU]); 
		
				
		}		
		i=MENU_pos;
		wattrset(parent,attrs[COLOR_MENU_HI]);	
		wmove(parent,MENU_ypos[i],MENU_xpos[i]);
		for (j=0;j<strlen(MENU_text[i]);j++)
		{
			
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU_HOTKEY_HI]); 
			if (MENU_text[i][j]!='%') waddch(parent,MENU_text[i][j]);
			if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU_HI]); 
		}		
		old_pos=MENU_pos;
		wrefresh(parent);
		ch=getch2();
		if ((ch==KEY_UP && MENU_updown_nleftright==1) || (ch==KEY_LEFT && MENU_updown_nleftright==0)) {
			MENU_pos=MENU_pos--;
			if (MENU_pos<0) MENU_pos=MENU_num-1;
		}
		if ((ch==KEY_DOWN && MENU_updown_nleftright==1) || (ch==KEY_RIGHT && MENU_updown_nleftright==0)) {
			MENU_pos=MENU_pos++;
			if (MENU_pos>=MENU_num) MENU_pos=0;
		}
		for (i=0;i<MENU_num;i++)
		{
			if (ch==MENU_hotkey1[i] || ch==MENU_hotkey2[i] || MENU_hotkey3[i]) {ch=KEY_RETURN;MENU_pos=i;}
		
		}
	}	
	i=old_pos;
	wattrset(parent,attrs[COLOR_MENU]);	
	wmove(parent,MENU_ypos[i],MENU_xpos[i]);
	for (j=0;j<strlen(MENU_text[i]);j++)
	{
		
		if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU_HOTKEY]); 
		if (MENU_text[i][j]!='%') waddch(parent,MENU_text[i][j]);
		if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU]); 
	
			
	}		
	i=MENU_pos;
	wattrset(parent,attrs[COLOR_MENU_HI]);	
	wmove(parent,MENU_ypos[i],MENU_xpos[i]);
	for (j=0;j<strlen(MENU_text[i]);j++)
	{
		
		if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU_HOTKEY_HI]); 
		if (MENU_text[i][j]!='%') waddch(parent,MENU_text[i][j]);
		if (j!=0) if (MENU_text[i][j-1]=='%') wattrset(parent,attrs[COLOR_MENU_HI]); 
	}		
	wrefresh(parent);
	return MENU_pos;	
}
void draw_frame(WINDOW *parent,int y1,int x1,int y2,int x2,char fillcharacter)
{
	int z;
	int i;
	if (x2<x1) { z=x1; x1=x2; x2=z; }
	if (y2<y1) { z=y1; y1=y2; y2=z; }
	wattrset(parent,attrs[COLOR_FRAME]);
	mvwaddch(parent,y1,x1,ACS_ULCORNER);
	mvwaddch(parent,y2,x1,ACS_LLCORNER);
	mvwaddch(parent,y1,x2,ACS_URCORNER);
	mvwaddch(parent,y2,x2,ACS_LRCORNER);
	wmove(parent,y1,x1+1);
	for (z=x1+1;z<x2;z++) waddch(parent,ACS_HLINE);	
	wmove(parent,y2,x1+1);
	for (z=x1+1;z<x2;z++) waddch(parent,ACS_HLINE);	
	for (z=y1+1;z<y2;z++)
	{
		wmove(parent,z,x1);
		waddch(parent,ACS_VLINE);
		if (fillcharacter!=0) {
			for (i=x1+1;i<x2;i++) waddch(parent,fillcharacter);	
		} else {
			wmove(parent,z,x1);
		}
		waddch(parent,ACS_VLINE);

	
	}	
}
void headline(WINDOW *parent,int y,int x,const char* text)
{
	wattrset(parent,attrs[COLOR_HEADLINE]);
	mvwprintw(parent,y,x+2,"%s",text);
	wattrset(parent,attrs[COLOR_BRACKETS]);
	wmove(parent,y,x+1);
	waddch(parent,'[');
	wmove(parent,y,x+2+strlen(text));
	waddch(parent,']');
}
void print_colnames(WINDOW *parent,int start,int actcol,int mark)
{
	int x=6;
	int i,j,g;
	int folded=0;
	int attr_mark;
	maxcol=start;
	
	for (i=start;i<MAXCOLS && x+col_width[i]<COLS;i++)
	{
		maxcol++;
		wmove(parent,0,x);
		if (i==actcol && mark==1) 
		{
			attr_mark=A_REVERSE;
		} else {
			attr_mark=0;
		}
		if (col_width[i]<0) 
		{
			if (folded==0) 
			{
				wattrset(parent,attrs[COLOR_FOLD]);
				waddch(parent,ACS_VLINE);
				wattrset(parent,attrs[COLOR_COLS]);
				x++;
				folded=1;
			}
		} else {
			wattrset(parent,attrs[COLOR_COLS]^attr_mark);
			folded=0;
			g=col_width[i]-strlen(col_names[i]);
			for (j=0;j<(g)/2;j++)
			{
				waddch(parent,' ');
			}		
			wprintw(parent,"%s",col_names[i]);
			for (j=0;j<(g+1)/2;j++)
			{
				waddch(parent,' ');
			}		
			x=x+col_width[i];
		}
	}
}
void print_rownames(WINDOW *parent,int start)
{
	int y=1;
	int i;
	int folded=0;
	wattrset(parent,attrs[COLOR_ROWS]);
	maxrow=start;
	for (i=start;i<MAXROWS && y<LINES-1;i++)
	{
		maxrow++;
		wmove(parent,y,0);
		if (row_height[i]<0) 
		{
			if (folded==0) 
			{
				wattrset(parent,attrs[COLOR_FOLD]);
				waddch(parent,ACS_HLINE);
				waddch(parent,ACS_HLINE);
				waddch(parent,ACS_HLINE);
				waddch(parent,ACS_HLINE);
				waddch(parent,ACS_HLINE);
				waddch(parent,ACS_HLINE);
				wattrset(parent,attrs[COLOR_COLS]);
				y++;
				folded=1;
			}
		} else {
			folded=0;
			wprintw(parent,"%5i:",i);
			y=y+row_height[i];
		}
	}
}
void print_row(WINDOW *parent,int row,int start,int y)
{
	int x=6;
	int folded=0;
	int i,j,g;
	int markattr=0;
	for (i=start;i<MAXCOLS && x+col_width[i]<COLS;i++)
	{
		wmove(parent,y,x);
		if (markon==1 && mark_leftcol<=i && mark_rightcol>=i && mark_upperrow<=row && mark_lowerrow>=row) {markattr=A_REVERSE;} else {markattr=0;}
		if (col_width[i]<0) 
		{
			if (folded==0) 
			{
				wattrset(parent,attrs[COLOR_FOLD]);
				waddch(parent,ACS_VLINE);
				x++;
				folded=1;
			}
		} else {
			folded=0;
			g=col_width[i]-strlen(cell_content[row][i]);
			wattrset(parent,cell_format[row][i]^markattr);	
			for (j=0;j<(g)/2;j++)
			{
				waddch(parent,' ');
			}
			wprintw(parent,"%s",cell_content[row][i]);
			for (j=0;j<(g+1)/2;j++)
			{
				waddch(parent,' ');
			}
			x=x+col_width[i];
		}
	}		
}
void print_col(WINDOW *parent,int start,int col,int x)
{
	int y=1;
	int folded=0;
	int i,j,g;
	int markattr=0;
	for (i=start;i<MAXROWS && y<LINES-1;i++)
	{
		wmove(parent,y,x);
		if (markon==1 && mark_leftcol<=col && mark_rightcol>=col && mark_upperrow<=i && mark_lowerrow>=i) {markattr=A_REVERSE;} else {markattr=0;}
		if (row_height[i]<0)
		{
			if (folded==0)
			{
				wattrset(parent,attrs[COLOR_FOLD]);
				for (j=0;j<col_width[col];j++) waddch(parent,ACS_HLINE);
				
				y++;
				folded=1;
			}
		} else {
			wattrset(parent,calcattr(cell_format[i][col])^markattr);
			g=col_width[col]-strlen(cell_content[i][col]);
			for (j=0;j<(g)/2;j++)
			{
				waddch(parent,' ');
			}
			wprintw(parent,"%s",cell_content[i][col]);
			for (j=0;j<(g+1)/2;j++)
			{
				waddch(parent,' ');
			}
			y=y+row_height[i];
		}	
	}
}
void print_table(WINDOW *parent,int startrow,int startcol)
{
	int x=6;
	int y=1;
	int i,j;
	int folded=0;
	for (i=startcol;i<MAXCOLS && x+col_width[i]<COLS;i++)
	{
		if (col_width[i]<0)
		{
			if (folded==0)
			{
				wattrset(parent,attrs[COLOR_FOLD]);
				for (j=y;j<LINES-1;j++)	mvwaddch(parent,j,x,ACS_VLINE);		
				folded=1;
				x++;
			}

		} else {
			print_col(parent,startrow,i,x);	
			x=x+col_width[i];
			folded=0;
		}
	}
}
int findx(int startcol,int col)
{
	int y=1;
	int x=6;
	int i,j,g;
	int folded=0;
	for (i=startcol;i<col;i++)
	{
		if (col_width[i]<0) {
			if (folded==0) {
				x++;
				folded=1;
			}
		} else {
			folded=0;
			x=x+col_width[i];
		}
	}
	mvwprintw(stdscr,0,0,"");
	return x;

}
int findy(int startrow,int row)
{
	int y=1;
	int i,j,g;
	int folded=0;

	for (i=startrow;i<row;i++)
	{
		if (row_height[i]<0) {
			if (folded==0)
			{
				y++;
				folded=1;
			}
		} else {
			folded=0;
			y=y+row_height[i];
		}
	}
	mvwprintw(stdscr,1,0,"");
	return y;	
}
void print_cell(WINDOW *parent,int startrow,int startcol,int row,int col,int attrs)
{
	int x,y,i,j,g;
	int folded=0;
	int markattr=0;
	x=findx(startcol,col);
	y=findy(startrow,row);
	if (col_width[col]<0 || row_height[row]<0) return;
	move(0,0);	
	wmove(parent,y,x);
	if (markon==1 && mark_leftcol<=col && mark_rightcol>=col && mark_upperrow<=row && mark_lowerrow>=row) {markattr=A_REVERSE;} else {markattr=0;}
	wattrset(parent,attrs^markattr);
	g=col_width[col]-strlen(cell_content[row][col]);
	for (j=0;j<(g)/2;j++)
	{
		waddch(parent,' ');
	}
	wprintw(parent,"%s",cell_content[row][col]);
	for (j=0;j<(g+1)/2;j++)
	{
		waddch(parent,' ');
	}
}
int calcattr(int format)
{
	if ((format&63)!=63) init_pair((format&63),(format&56)>>3,(format&7));
	
	
	return COLOR_PAIR((format&63))|(A_UNDERLINE*((format&64)/64))|(A_REVERSE*((format&128)/128))|(A_BOLD*((format&256)/256))|(A_BLINK*((format&512)/512))|(A_DIM*((format&1024)/1024));
	
}
void uimain(int argc,char *argv[])
{
	int c;
	int i;
	(void) signal(SIGINT, finish);
	(void) initscr();
//	setupterm("xterm-color",1,(int *)0);
	keypad (stdscr,TRUE);
	(void) nonl();
	(void) cbreak();
	(void) noecho();
	if (has_colors())
	{
		start_color();	
		init_colors();
	} else {
		endwin();
		fprintf(stderr,"sorry... i need colors... (try using export TERM=xterm-color)\n");	
		byebye(1);
	}
	wclear(stdscr);	
	wrefresh(stdscr);
}

void finish(int sig)
{
	endwin();
	byebye(sig);
}
