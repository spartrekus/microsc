#include <ncurses.h>
#include <signal.h>
#include <string.h>

#define KEY_ESC 27
#define KEY_RETURN 13
#define KEY_DELETE 127
#define COLOR_INPUT 1
#define COLOR_ROWS 2
#define COLOR_COLS 3
#define COLOR_CURSOR 4
#define COLOR_TEXT 5
#define COLOR_MENU 6
#define COLOR_MENU_HI 7
#define COLOR_MENU_HOTKEY 8
#define COLOR_MENU_HOTKEY_HI 9
#define COLOR_FRAME 10
#define COLOR_HEADLINE 11
#define COLOR_BRACKETS 12
#define COLOR_FOLD 13
#define COLOR_CELL 14

#define MAXROWS 1024
#define MAXCOLS 512
int lastkey;
int attrs[255];

void finish(int sig);
#define MENU_MAX 64
int MENU_xpos[MENU_MAX];
int MENU_ypos[MENU_MAX];
char *MENU_text[MENU_MAX];
char MENU_hotkey1[MENU_MAX];
char MENU_hotkey2[MENU_MAX];
char MENU_hotkey3[MENU_MAX];
int MENU_num;
int MENU_pos;
int MENU_updown_nleftright;

int printable(int ch);
int getch2();
void init_colors();
char *input2(WINDOW *parent,int y,int x,int len,const char *text,int max,int special,int lastkey);
char *input(WINDOW *parent,int y,int x,int len,const char *text,int max);
void new_menu(int updown_nleftright);
void menu_item(int pos,int y,int x,char *text, char hotkey1,char hotkey2,char hotkey3);
int menu_show(WINDOW *parent);
void draw_frame(WINDOW *parent,int y1,int x1,int y2,int x2,char fillcharacter);
void headline(WINDOW *parent,int y,int x,const char* text);
void print_colnames(WINDOW *parent,int start,int actcol,int mark);
void print_rownames(WINDOW *parent,int start);
void print_row(WINDOW *parent,int row,int start,int y);
void print_col(WINDOW *parent,int start,int col,int x);
void print_table(WINDOW *parent,int startrow,int startcol);
int findx(int startcol,int col);
int findy(int startrow,int row);
void print_cell(WINDOW *parent,int startrow,int startcol,int row,int col,int attrs);




