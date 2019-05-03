#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "ui.h"
#include "gpl.h"
#include "data.h"

#define MAXBOTTOMMENU 1
char *filename;
int bottommenu=0;
int sort_order[MAXROWS];
int maxcol;
int maxrow;
char *itoa(signed int num)
{
	int i;
	int j=1;
	char *tmp;

	if (num<0) {
		i=-num;
		j++;
	} else i=num;
	for (;i>0;i=i/10) {j++;}
	tmp=malloc(j);	
	j--;
	tmp[j--]=0;
	if (num<0) {
		i=-num;
		tmp[0]='-';
	} else i=num;
	for (;i>0;i=i/10) {
		tmp[j]=48+(i%10);
		j--;
	}
	return tmp;
			
}
int atoi2(const char* a)
{
	int num=0;
	int i;
	int n=1;
	if (strlen(a)==0) return -2147483648;

	for (i=0;i<strlen(a);i++)
	{
		if (a[i]=='-') {n=-n;}
		else if (a[i]>='0' && a[i]<='9') {num=num*10+(a[i]-48);}
		else if (a[i]!=' ') return -2147483648;	
	}	
	return n*num;
}
void init()
{
        int i;
        int j;
        int c;
	mark_leftcol=-1;
	mark_rightcol=-1;
	mark_upperrow=-1;
	mark_lowerrow=-1;
	markon=0;
	mark_cursorpos=0;

        c=312;
        attrset(attrs[COLOR_TEXT]);

        for (i=0;i<MAXCOLS;i++)
        {
                for (j=0;j<MAXROWS;j++)
                {
                        cell_format[j][i]=c;
//                        cell_content[j][i]=malloc(2);
//                        cell_content[j][i][0]=48;
//                        cell_content[j][i][1]=0;
                        cell_content[j][i]=malloc(1);
                        cell_content[j][i][0]=0;
                }
   /*             col_names[i]=malloc(4);
                col_names[i][0]=65+(i/676);
                col_names[i][1]=65+((i%676)/26);
                col_names[i][2]=65+(i%26);
                col_names[i][3]=0;
		*/
		col_names[i]=malloc(1);
		col_names[i][0]=0;
                col_width[i]=5;
                col_type[i]=0;
                mvwprintw(stdscr,1,1,"%4i%% initialised...",i*100/MAXCOLS);
		mvwprintw(stdscr,2,1,"if you can read this then you have a slower computer than me ;-)");
                wrefresh(stdscr);

        }
        for (j=0;j<MAXROWS;j++)
        {
                row_height[j]=1;
        }
}
void show_bottommenu(int num)
{
	int i;
	wattrset(stdscr,attrs[COLOR_MENU_HOTKEY]);
	for (i=0;i<9;i++)
	{
		if (i*8<COLS) {mvwaddch(stdscr,LINES-1,i*8,49+i);}
	}
	if (73<COLS) {mvwaddstr(stdscr,LINES-1,72,"10");}
	if (81<COLS) {mvwaddstr(stdscr,LINES-1,80,"11");}
	if (89<COLS) {mvwaddstr(stdscr,LINES-1,88,"12");}
	wattrset(stdscr,attrs[COLOR_MENU]);
	if (num==0) {
		if ( 7<COLS) {mvwaddstr(stdscr,LINES-1, 1,"sort   ");}
		if (15<COLS) {mvwaddstr(stdscr,LINES-1, 9,"edit   ");}
		if (23<COLS) {mvwaddstr(stdscr,LINES-1,17,"mark   ");}
		if (31<COLS) {mvwaddstr(stdscr,LINES-1,25,"fold   ");}
		if (39<COLS) {mvwaddstr(stdscr,LINES-1,33,"unfold ");}
		if (47<COLS) {mvwaddstr(stdscr,LINES-1,41,"cut    ");}
		if (55<COLS) {mvwaddstr(stdscr,LINES-1,49,"copy   ");}
		if (63<COLS) {mvwaddstr(stdscr,LINES-1,57,"paste  ");}
		if (71<COLS) {mvwaddstr(stdscr,LINES-1,65,"format ");}
	}
	if (num==1) {
		if ( 7<COLS) {mvwaddstr(stdscr,LINES-1, 1,"in/out ");}
		if (15<COLS) {mvwaddstr(stdscr,LINES-1, 9,"delrow ");}
		if (23<COLS) {mvwaddstr(stdscr,LINES-1,17,"delcol ");}
		if (31<COLS) {mvwaddstr(stdscr,LINES-1,25,"insrow ");}
		if (39<COLS) {mvwaddstr(stdscr,LINES-1,33,"inscol ");}
		if (47<COLS) {mvwaddstr(stdscr,LINES-1,41,"output ");}
		if (55<COLS) {mvwaddstr(stdscr,LINES-1,49,"       ");}
		if (63<COLS) {mvwaddstr(stdscr,LINES-1,57,"       ");}
		if (71<COLS) {mvwaddstr(stdscr,LINES-1,65,"       ");}
		
	}
	if (81<COLS) {mvwaddstr(stdscr,LINES-1,74,"quit  ");}
	if (88<COLS) {mvwaddstr(stdscr,LINES-1,82,"redraw");}
	if (99<COLS) {mvwaddstr(stdscr,LINES-1,90,"next menu");}
}
void merge(int colnum,int left,int q,int right)
{
	int left2=left;
	int q2=q+1;
	int i,j;
	int b[MAXROWS];
	for (i=left;i<=right;i++) {
		if (left2<=q && q2<=right)
		{
			if ((strcmp(cell_content[sort_order[left2]][colnum],cell_content[sort_order[q2]][colnum])<=0 && strlen(cell_content[sort_order[left2]][colnum])!=0)|| strlen(cell_content[sort_order[q2]][colnum])==0)		

			{
				b[i]=sort_order[left2];
				left2++;
			} else {
				b[i]=sort_order[q2];
				q2++;
			}
		} else {
			if (left2<=q)
			{
				b[i]=sort_order[left2];
				left2++;
			} else {
				b[i]=sort_order[q2];
				q2++;
			}
		}
	}
	for (i=left;i<=right;i++)
	{
		sort_order[i]=b[i];
	}
}
void mymergesort(int colnum,int left,int right)
{
	int q;
	if (left<right) {
		q=(left+right)/2;
		mymergesort(colnum,left,q);
		mymergesort(colnum,q+1,right);
		merge(colnum,left,q,right);
	}
}
void sort_by(int colnum)
{
	int i;
	int j;
	int g;
	int h;
	char *tmp;
	char *tmp2;
	for (i=0;i<MAXROWS;i++)
	{
		sort_order[i]=i;
	}
	mymergesort(colnum,0,MAXROWS-1);	
//	fprintf(stderr,"\n");
	for (i=0;i<MAXROWS;i++)
	{
		j=sort_order[i];
		for (g=0;g<MAXROWS;g++) {
			if (sort_order[g]==i) sort_order[g]=j;
		}
		if (j!=i) for (g=0;g<MAXCOLS;g++)
		{
			tmp=malloc(strlen(cell_content[i][g])+1);
			tmp2=malloc(strlen(cell_content[j][g])+1);
			strncpy(tmp,cell_content[i][g],strlen(cell_content[i][g])+1);
			strncpy(tmp2,cell_content[j][g],strlen(cell_content[j][g])+1);
			free(cell_content[i][g]);
			free(cell_content[j][g]);
			cell_content[i][g]=malloc(strlen(tmp2)+1);
			cell_content[j][g]=malloc(strlen(tmp)+1);
			strncpy(cell_content[i][g],tmp2,strlen(tmp2)+1);
			strncpy(cell_content[j][g],tmp,strlen(tmp)+1);
			free(tmp);
			free(tmp2);
			
			h=cell_format[i][g];
			cell_format[i][g]=cell_format[j][g];
			cell_format[j][g]=h;

			
		}
		h=row_height[i];
		row_height[i]=row_height[j];
		row_height[j]=h;
	}
}
void format_menu(WINDOW *parent,int colnum)
{
	int i;
	int t;
	char *tmp;
	i=0;
	draw_frame(parent,5,5,20,60,' ');
	headline(parent,5,5,"FORMAT");
	while (i!=5) {
		wattrset(parent,attrs[COLOR_BRACKETS]);
		mvwaddch(parent,7,20,'[');
		mvwaddch(parent,8,20,'[');
		mvwaddch(parent,10,20,'[');
		mvwaddch(parent,7,59,']');
		mvwaddch(parent,8,24,']');
		mvwaddch(parent,10,29,']');
		wrefresh(parent);
		new_menu(1);
		menu_item(0,7,6,"%Name:      ",'n','N',0);
		menu_item(1,8,6,"%Width:     ",'w','W',0);
		menu_item(2,10,6,"%Type:      ",'t','T',0);
		wattrset(parent,attrs[COLOR_TEXT]);
		mvwaddstr(parent,7,21,col_names[colnum]);
		mvwprintw(parent,8,21,"%3i",col_width[colnum]);
		if ((col_type[colnum]&1)==1) mvwaddstr(parent,10,21,"Input   ");	
		else if ((col_type[colnum]&2)==2) mvwaddstr(parent,10,21,"Output  ");
		else if ((col_type[colnum]&4)==4) mvwaddstr(parent,10,21,"Freeform");
		else mvwaddstr(parent,10,21,"None    ");
			
	
		if ((col_type[colnum]&3)!=0) {
			menu_item(3,11,6,"%MSB:       ",'m','M',0);
			menu_item(4,12,6,"%LSB:       ",'l','L',0);
			menu_item(5,19,6,"%Cancel:    ",'c','C',0);
			wattrset(parent,attrs[COLOR_BRACKETS]);
			mvwaddch(parent,11,20,'[');
			mvwaddch(parent,12,20,'[');
			mvwaddch(parent,11,25,']');
			mvwaddch(parent,12,25,']');
			wattrset(parent,attrs[COLOR_TEXT]);
			mvwprintw(parent,11,21,"%4i",(col_type[colnum]&16376)>>3);
			mvwprintw(parent,12,21,"%4i",(col_type[colnum]&33538048)>>14);
				
		} else {
			mvwaddstr(parent,11, 6,"                                  ");
			mvwaddstr(parent,12, 6,"                                  ");
			menu_item(3,19,6,"%Cancel",'c','C',0);
		}
		i=menu_show(parent);
		if (i==0) 
		{
			tmp=input(parent,7,21,37,col_names[colnum],127);
			free(col_names[colnum]);
			col_names[colnum]=malloc(strlen(tmp)+1);
			strncpy(col_names[colnum],tmp,strlen(tmp)+1);
			free(tmp);
		}
		if (i==1)
		{
			tmp=input(parent,8,21,3,itoa(col_width[colnum]),3);
			col_width[colnum]=atoi(tmp);
			free(tmp);
		}
		if (i==2)
		{
			new_menu(0);
			menu_item(0,10,20,"<%Freeform>",'f','F',0);
			menu_item(1,10,31,"<%Input>",'i','I',0);
			menu_item(2,10,39,"<%Output>",'i','I',0);
			menu_item(3,10,48,"<%None>",'n','N',0);
			if ((col_type[colnum]&1)==1) {MENU_pos=1;}
			else if ((col_type[colnum]&2)==2) {MENU_pos=2;}
			else if ((col_type[colnum]&4)==4) {MENU_pos=0;}
			else MENU_pos=3; 
			i=menu_show(parent);
			wattrset(parent,attrs[COLOR_FRAME]);
			mvwaddstr(parent,10,20,"                                  ");
			col_type[colnum]=col_type[colnum]&33554428;
			if (i==1) {col_type[colnum]=col_type[colnum]|1;}
			else if (i==2) {col_type[colnum]=col_type[colnum]|2;}
			else if (i==0) {col_type[colnum]=col_type[colnum]|4;}
			i=-1;
			
		}	
		if ((col_type[colnum]&3)!=0) {
			if (i==3)
			//TODO: da muessen noch abfragen rein, dass das nicht groesser wird als 10 bit!
			{
				
				t=(col_type[colnum]&16376)>>3;
				tmp=input(parent,11,21,4,itoa(t),4);
				col_type[colnum]=(col_type[colnum]&33538055)|(atoi(tmp)<<3);
				free(tmp);
			}
			if (i==4)
			{
				t=(col_type[colnum]&33538048)>>14;
				tmp=input(parent,12,21,4,itoa(t),4);
				col_type[colnum]=(col_type[colnum]&16383)|(atoi(tmp)<<14);
				free(tmp);
			}
							
		} else {
			if (i==3) i=5;
		}		
	}
}
void colmenu(WINDOW *parent,int mark_leftcol,int mark_rightcol,int mark_upperrow,int mark_lowerrow,int markon, int actrow, int actcol)
{
	int i,j,m;
	int beispiel;
	char *colornames[8]={ " BLACK ", "  RED  ", " GREEN ", " YELLOW", "  BLUE ", "MAGENTA", "  CYAN ", " WHITE "};
	if (markon==0)
	{
		mark_leftcol=actcol;
		mark_rightcol=actcol;
		mark_upperrow=actrow;
		mark_lowerrow=actrow;
	}
	beispiel=cell_format[mark_upperrow][mark_leftcol];
	while (m!=7) 
	{
		draw_frame(parent,10,10,21,50,' ');
		headline(parent,10,10,"FORMAT CELLS");
		new_menu(1);
		wattrset(parent,attrs[COLOR_BRACKETS]);
		for (i=12;i<=18;i++) mvwaddch(parent,i,27,'[');
		for (i=14;i<=18;i++) mvwaddch(parent,i,29,']');
		mvwaddch(parent,12,35,']');
		mvwaddch(parent,13,35,']');
		wattrset(parent,attrs[COLOR_TEXT]);
		mvwaddstr(parent,12,28,colornames[(beispiel&56)>>3]);
		mvwaddstr(parent,13,28,colornames[beispiel&7]);
		for (i=0;i<7;i++)
		{
			if ((beispiel&(64<<i))==(64<<i)) mvwaddch(parent,i+14,28,'X'); else mvwaddch(parent,i+14,28,' ');
		}
		menu_item(0,12,11,"<%Foreground>",'f','F',0);
		menu_item(1,13,11,"<%Background>",'b','B',0);
		menu_item(2,14,11,"<%Underline>",'u','U',0);
		menu_item(3,15,11,"<%Reverse>",'r','R',0);
		menu_item(4,16,11,"<%Bright/Bold>",'b','B',0);
		menu_item(5,17,11,"<B%link>",'l','L',0);
		menu_item(6,18,11,"<%Dim>",'d','D',0);
		menu_item(7,20,11,"<%Cancel>",'c','C',0);
		m=menu_show(parent);
		if (m<2) {
			new_menu(1);
			menu_item(0,12,37,"<%Black>",'b','B',0);
			menu_item(1,13,37,"<%Red>",'r','R',0);
			menu_item(2,14,37,"<%Green>",'g','G',0);
			menu_item(3,15,37,"<%Yellow>",'y','Y',0);
			menu_item(4,16,37,"<B%lue>",'l','L',0);
			menu_item(5,17,37,"<%Magenta>",'m','M',0);
			menu_item(6,18,37,"<%Cyan>",'c','C',0);
			menu_item(7,19,37,"<%White>",'w','W',0);
		}
		if (m==0) {
			m=menu_show(parent);
			beispiel=(beispiel&1991)|(m<<3);
			m=0;
			for (i=mark_leftcol;i<=mark_rightcol;i++) for (j=mark_upperrow;j<=mark_lowerrow;j++) cell_format[j][i]=beispiel;
		}
		if (m==1) {
			m=menu_show(parent);
			beispiel=(beispiel&2040)|m;
			m=0;
			for (i=mark_leftcol;i<=mark_rightcol;i++) for (j=mark_upperrow;j<=mark_lowerrow;j++) cell_format[j][i]=beispiel;
		}
		if (m>1 && m<7) {
			beispiel=beispiel^(16<<m);
			for (i=mark_leftcol;i<=mark_rightcol;i++) for (j=mark_upperrow;j<=mark_lowerrow;j++) cell_format[j][i]=beispiel;
		}
	}
}
void asciiload(char *filename)
{
	FILE *f;
	int i;
	char buffer[2000];
	signed char ch=-1;
	int zeile=0;
	int spalte=0;
	int ignore=0;
	int interessante_spalte=0;
	int interessante_zeile=0;
	int interessant=0;
	char *tmp;
	char *tmp2;
/*
	das dateiformat fuer die asciitabelle:
	1. zeile: zeilenzahl+tab+spaltenzahl
	2. zeile: spaltentyp, bit 0=input, bit 1=output, bit 2=freeform, bit 3-13: msb, bit 14-24: lsb
	3. zeile: ueberschriften
	4.-n.zeile: die tabelle


	spalten werden durch tabulatoren voneinander getrennt. zeilen durch 
	#10 oder #13, ganz nach wunsch. ein # am anfang sorgt dafuer, dass die
	komplette zeile nicht mehr eingelesen wird.
*/
	tmp=malloc(1);
	tmp[0]=0;
	wattrset(stdscr,attrs[COLOR_TEXT]|A_BLINK);
	mvwprintw(stdscr,16,3,"Loading...");
	f=fopen(filename,"ro");
	if (f!=NULL) {
		while (!feof(f))
		{
			mvwprintw(stdscr,16,58,"%4i%%",((zeile*100)/MAXROWS));
			wrefresh(stdscr);
			fgets(buffer,sizeof(buffer),f);
			i=0;
			while (ch!=0 && !feof(f))
			{
				ch=buffer[i];
				if (ch==13) ch=10;
				if (zeile<=1 && ignore==0) {interessant=1;}
				if (zeile<=1 && ch=='#') interessant=0;
//				fprintf(stderr,"type  %i %i %i   [%s]  %i   %i %i (%c) %i\n",interessante_zeile,zeile,spalte,tmp,atoi(tmp),ignore,interessant,ch,ch);
				if (ignore==0) 
				{
					if (ch==9 || ch==10 || (ch=='#'	&& interessant==0))
					{
						if (zeile==0) 
						{
							// maxzeile und maxspalte einlesen, TODO
							if (spalte==0) maxrow=atoi(tmp);
							if (spalte==1) maxcol=atoi(tmp);
		
						} else
						if (zeile==1) 
						{
							if (spalte<MAXCOLS) col_type[spalte]=atoi(tmp);
						} else
						if (zeile==2)
						{
							if (spalte<MAXCOLS) 
							{
								free(col_names[spalte]);
								col_names[spalte]=malloc(strlen(tmp)+1);
								strncpy(col_names[spalte],tmp,strlen(tmp)+1);
							}
						} else
						{
							if (spalte<MAXCOLS && zeile-3<MAXROWS) {
								free(cell_content[zeile-3][spalte]);
								cell_content[zeile-3][spalte]=malloc(strlen(tmp)+1);
								strncpy(cell_content[zeile-3][spalte],tmp,strlen(tmp)+1);
							}
	
						}
						if (interessante_spalte==1) {
							spalte++;	
							interessante_spalte=0;
						}
						if (ch==10 || ch=='#')
						{
							if (interessante_zeile==1) {
								zeile++;
							}	
							interessante_zeile=0;
							interessante_spalte=0;
							spalte=0;
						}
						if (ch=='#') ignore=1;
						if (zeile>1) interessant=0;
						ch=-1;
						free(tmp);
						tmp=malloc(1);
						tmp[0]=0;
					}
					if (ch==34) {
						interessant=1-interessant;
						interessante_zeile=1;
						interessante_spalte=1;
						ch=-1;
					}
					if (interessant==1 && ch!=-1 && ch!=0) 
					{
						interessante_spalte=1;
						interessante_zeile=1;
						tmp2=malloc(strlen(tmp)+2);
						strncpy(tmp2,tmp,strlen(tmp));
						tmp2[strlen(tmp)]=ch;
						tmp2[strlen(tmp)+1]=0;
						
						free(tmp);
						tmp=malloc(strlen(tmp2)+1);
						strncpy(tmp,tmp2,strlen(tmp2)+1);
						free(tmp2);
					}
				} else {
					if (ch==10) {
						spalte=0;
						if (interessante_zeile==1) zeile++;
						interessante_spalte=0;
						interessante_zeile=0;
						interessant=0;
						ignore=0;
					}
				}
				if (ch=='#') {ignore=1;}
				i++;	
			}
			ch=-1;
		}	
	
		fclose(f);
	}
}
void formatload(char *filename)
{
	FILE *f;
	char *tmp;
	int i;
	int j;	
	short k;
	int l;
	unsigned short s;

	short endian;
	l=strlen(filename);
	tmp=malloc(l+5);
	strncpy(tmp,filename,l);
	tmp[l]='.';
	tmp[l+1]='f';
	tmp[l+2]='r';
	tmp[l+3]='m';
	tmp[l+4]=0;
	f=fopen(tmp,"ro");
	if (f!=NULL) {
		fread(&col_width,sizeof(col_width[0]),maxcol,f);
		fread(&row_height,sizeof(row_height[0]),maxrow,f);
		
//		fread(&cell_format,sizeof(cell_format[0][0]),maxrow*maxcol,f);
		for (i=0;i<=maxrow;i++)
			for (j=0;j<=maxcol;j++)
				fread(&cell_format[i][j],sizeof(cell_format[i][j]),1,f);

		#ifdef BIGENDIAN
		if (BIGENDIAN==1)
		{
			for (i=0;i<=maxrow;i++)
			{
				for (j=0;j<=maxcol;j++)
				{
					s=((cell_format[i][j]&65280)>>8)+((cell_format[i][j]&255)<<8);
					cell_format[i][j]=s;	
				}
				if (i<maxrow) 
				{
					if (row_height[i]<0) {
						row_height[i]=-row_height[i];
						j=-1;
					} 
					else 
					{
						j=1;
					}
					
					s=((row_height[i]&65280)>>8)+((row_height[i]&255)<<8);
					row_height[i]=s;	
					row_height[i]=j*row_height[i];
				}
			}
				
			for (i=0;i<maxcol;i++)
			{
				if (col_width[i]<0) {
					col_width[i]=-col_width[i];
					j=-1;
				} 
				else 
				{
					j=1;
				}
				s=((col_width[i]&65280)>>8)+((col_width[i]&255)<<8);
				col_width[i]=s;	
				col_width[i]=j*col_width[i];
			}
		}
		#endif
/*
		for (i=0;i<maxcol && !feof(f);i++) {
			fgets(col_width[i],sizeof(col_width[i]),f);
		}
		for (i=0;i<maxrow && !feof(f);i++) {
			fgets(row_height[i],sizeof(row_height[i]),f);
		}
		for (i=0;i<maxrow && !feof(f);i++) {
			for (j=0;j<maxcol;j++) {
				fgets(cell_format[i][j],sizeof(cell_format[i][j]),f);
			}
		}
*/
		fclose(f);
	}
	free(tmp);	
}
void formatsave(char *filename,int maxrow,int maxcol)
{
	FILE *f;
	char *tmp;
	int i;
	int j;	
	short k;
	int l;
	unsigned short s;
	l=strlen(filename);
	tmp=malloc(l+5);
	strncpy(tmp,filename,l);
	tmp[l]='.';
	tmp[l+1]='f';
	tmp[l+2]='r';
	tmp[l+3]='m';
	tmp[l+4]=0;
	#ifdef BIGENDIAN
		if (BIGENDIAN==1)
		{
			for (i=0;i<=maxrow;i++)
			{
				for (j=0;j<=maxcol;j++)
				{
					s=((cell_format[i][j]&65280)>>8)+((cell_format[i][j]&255)<<8);
					cell_format[i][j]=s;	
				}
				if (i<maxrow) 
				{
					if (row_height[i]<0) {
						row_height[i]=-row_height[i];
						j=-1;
					} 
					else 
					{
						j=1;
					}
					s=((row_height[i]&65280)>>8)+((row_height[i]&255)<<8);
					row_height[i]=s;	
					row_height[i]=j*row_height[i];
				}
				
			}
			for (i=0;i<maxcol;i++)
			{
				if (col_width[i]<0) {
					col_width[i]=-col_width[i];
					j=-1;
				} 
				else 
				{
					j=1;
				}
				s=((col_width[i]&65280)>>8)+((col_width[i]&255)<<8);
				col_width[i]=s;	
				col_width[i]=j*col_width[i];
			}
		}
	#endif
	f=fopen(tmp,"w");
	if (f!=NULL) {
		fwrite(&col_width,sizeof(col_width[0]),maxcol,f);
		fwrite(&row_height,sizeof(row_height[0]),maxrow,f);
		for (i=0;i<=maxrow;i++)
			for (j=0;j<=maxcol;j++)
				fwrite(&cell_format[i][j],sizeof(cell_format[i][j]),1,f);
		fclose(f);
	}
	#ifdef BIGENDIAN
		if (BIGENDIAN==1)
		{
			for (i=0;i<=maxrow;i++)
			{
				for (j=0;j<=maxcol;j++)
				{
					s=((cell_format[i][j]&65280)>>8)+((cell_format[i][j]&255)<<8);
					cell_format[i][j]=s;	
				}
				if (i<maxrow) 
				{
					if (row_height[i]<0) {
						row_height[i]=-row_height[i];
						j=-1;
					} 
					else 
					{
						j=1;
					}
					s=((row_height[i]&65280)>>8)+((row_height[i]&255)<<8);
					row_height[i]=s;	
					row_height[i]=j*row_height[i];
				}
				
			}
			for (i=0;i<maxcol;i++)
			{
				if (col_width[i]<0) {
					col_width[i]=-col_width[i];
					j=-1;
				} 
				else 
				{
					j=1;
				}
				s=((col_width[i]&65280)>>8)+((col_width[i]&255)<<8);
				col_width[i]=s;	
				col_width[i]=j*col_width[i];
			}
		}
	#endif
	free(tmp);	
}
void asciisave(char *filename,int maxrow,int maxcol)
{
	FILE *f;
	int i,j;
	
	f=fopen(filename,"w");
	fprintf(f,"#!/usr/bin/nMicrocoder\n");
	fprintf(f,"### nMicrocoder\n");
	fprintf(f,"### (C)opyleft 2004 by Dettus\n");
	fprintf(f,"### dettus@matrixx-bielefeld.de\n");
	fprintf(f,"#\n");
	fprintf(f,"# the fileformat so far:\n");
	fprintf(f,"# 1st line:  maximum number of rows + <tab> + maximum number of cols\n");
	fprintf(f,"# 2nd line:  format for every coloumn, \n");
        fprintf(f,"#            bit 0: inputcol\n");
	fprintf(f,"#            bit 1: outputcol\n");
	fprintf(f,"#            bit 2: freeform\n");
	fprintf(f,"#            bit 3-13: most significant bit\n");
	fprintf(f,"#            bit 4-24: least significant bit\n");
	fprintf(f,"# 34d line:  headlines for every col.\n");
	fprintf(f,"# 4th-nth:   content\n");
	fprintf(f,"#\n#\n# lines after a # are ignored, lines are seperated by linefeed or carriage\n");
	fprintf(f,"# return, text is written in \" \". but i guess you already got that ;-) \n");
	fprintf(f,"%i\t%i\n",maxrow,maxcol);
	for (i=0;i<maxcol;i++) fprintf(f,"%i\t",col_type[i]);
	fprintf(f,"\n");
	for (i=0;i<maxcol;i++) fprintf(f,"\"%s\"\t",col_names[i]);
	fprintf(f,"\n");
	for (i=0;i<maxrow;i++) {
		for (j=0;j<maxcol;j++) {
			fprintf(f,"\"%s\"\t",cell_content[i][j]);
		}
		fprintf(f,"\n");
		mvwprintw(stdscr,16,58,"%4i%%",((i*100)/maxrow));
		wrefresh(stdscr);
	}
	
	fclose(f);
}
void exit_menu()
{	
	int i,j,k;
	int maxcol=0;
	int maxrow=0;
	char *tmp;
	while (i!=4) 
	{
		draw_frame(stdscr,15,2,23,79,' ');
		headline(stdscr,15,2,"EXIT&SAVE");
		wattrset(stdscr,attrs[COLOR_BRACKETS]);
		mvwaddch(stdscr,17,15,'[');
		mvwaddch(stdscr,17,78,']');
		wattrset(stdscr,attrs[COLOR_TEXT]);
		mvwaddstr(stdscr,17,16,filename);
		new_menu(1);
		menu_item(0,17,3,"<%Filemame:>",'f','F',0);
		menu_item(1,18,3,"<%Save>",'s','S',0);
		menu_item(2,19,3,"<Save & %Exit>",'e','E',0);
		menu_item(3,20,3,"<Don't Save and E%xit>",'x','X',0);
		menu_item(4,22,3,"<%Cancel>",'c','C',0);
		i=menu_show(stdscr);
		if ((i==1 || i==2) && strlen(filename)==0) {i=0;}
		if (i==0) 
		{
			tmp=input(stdscr,17,16,61,filename,61);
			free(filename);
			filename=malloc(strlen(tmp)+1);
			strncpy(filename,tmp,strlen(tmp)+1);	
		}
		if (i==1 || i==2 )
		{
			wattrset(stdscr,attrs[COLOR_TEXT]);
			mvwprintw(stdscr,16,3,"Finding the lower right corner...");
			for (j=0;j<MAXCOLS;j++)
			{
				if (strlen(col_names[j])!=0 && maxcol<j) maxcol=j;
				for (k=0;k<MAXROWS;k++)
				{
					if (strlen(cell_content[k][j])!=0) {
						if (maxcol<j) maxcol=j;
						if (maxrow<k) maxrow=k;

					}
				}
				mvwprintw(stdscr,16,58,"%4i%%",((j*100)/MAXCOLS));
				wrefresh(stdscr);
			}
			mvwprintw(stdscr,16,3,"Saving...                        ");
			asciisave(filename,maxrow+1,maxcol+1);
			formatsave(filename,maxrow+1,maxcol+1);
		}
		if (i==3 || i==2) 
		{
			finish(0);
		}
	}
}
void htmloutput (char *filename)
{
	int i,j;
	FILE *f;
	char *tmp;
	char *tmp2;
	char *cols[]={"#000000","#aa0000","#00D200","#AA5522","#0000AA","#AA00AA","#00AAAA","#AAAAAA","#444444","#FF4444","#44FF44","#FFFF44","#4444FF","#FF44FF","#44FFFF","#FFFFFF"};

	int fgcol,bgcol;	
	int xchng;
	int maxrow=0;
	int maxcol=0;	
	int fo;
	tmp=malloc(strlen(filename)+6);
	strncpy(tmp,filename,strlen(filename));
	tmp[strlen(filename)  ]='.';
	tmp[strlen(filename)+1]='h';
	tmp[strlen(filename)+2]='t';
	tmp[strlen(filename)+3]='m';
	tmp[strlen(filename)+4]='l';
	tmp[strlen(filename)+5]=0;
	
	f=fopen(tmp,"w");
	fprintf(f,"<html>\n");
	fprintf(f,"<table border=1>\n");
	for (i=0;i<MAXCOLS;i++)
	{
		if (strlen(col_names[i])!=0 && maxcol<i) maxcol=i;
		for (j=0;j<MAXROWS;j++)
		{
			if (strlen(cell_content[j][i])!=0) 
			{
				if (maxcol<i) maxcol=i;
				if (maxrow<j) maxrow=j;
			}		
		}
	}	
	fprintf(f,"<tr>");
	for (j=0;j<=maxcol;j++)
	{
		fprintf(f,"<td bgcolor=#000000><font color=#4444FF>%s</td>",col_names[j]);	
	}
	fprintf(f,"</tr>\n");
	for (i=0;i<=maxrow;i++)
	{
		fprintf(f,"<tr>");
		for (j=0;j<=maxcol;j++)
		{
			//cell_format [i][j]
			//cell_content [i][j]
			fo=cell_format[i][j];
			fgcol=(fo&56)>>3;
			bgcol=fo&7;
			
			if ((fo&256)==256) fgcol=fgcol|8;
			if ((fo&128)==128) 
			{
				xchng=fgcol;
				fgcol=bgcol;
				bgcol=xchng;
			}	
			fprintf(f,"<td bgcolor=%s><font color=%s>%s</td>",cols[bgcol],cols[fgcol],cell_content[i][j]);
		//TODO:
/*
          for (i=0;i<7;i++)
                {
                        if ((beispiel&(64<<i))==(64<<i)) mvwaddch(parent,i+14,28,'X'); else mvwaddch(parent,i+14,28,' ');
                }
                menu_item(0,12,11,"<%Foreground>",'f','F',0);
                menu_item(1,13,11,"<%Background>",'b','B',0);
                menu_item(2,14,11,"<%Underline>",'u','U',0);
                menu_item(3,15,11,"<%Reverse>",'r','R',0);
                menu_item(4,16,11,"<%Bright/Bold>",'b','B',0);
                menu_item(5,17,11,"<B%link>",'l','L',0);
                menu_item(6,18,11,"<%Dim>",'d','D',0);


*/								
		}
		fprintf(f,"</tr>\n");
	}
	fprintf(f,"</table>");
	fprintf(f,"</html>");
	fclose(f);	
}

void verilog2output(char *filename)
{
	char *tmp;
	char *tmp2;
	int g,h,i,j,k,l,y,x,z;
	FILE *f;
	int bitbreite[MAXCOLS];
	int msb[MAXCOLS];
	int lsb[MAXCOLS];
	int von[MAXCOLS];
	int bis[MAXCOLS];
	int tmpbreite=0;
	int and=0;
	int or=0;
	int maxcol=0;
	int maxrow=0;
	tmp=malloc(strlen(filename)+3);
	tmp2=malloc(strlen(filename)+1);
	strncpy(tmp2,filename,strlen(filename)+1);
	strncpy(tmp,filename,strlen(filename));
	tmp[strlen(filename)  ]='.';
	tmp[strlen(filename)+1]='v';
	tmp[strlen(filename)+2]=0;
	for (i=0;i<strlen(tmp2);i++) if (tmp2[i]=='.') tmp2[i]='_';		
	f=fopen(tmp,"w");
	fprintf(f,"module %s(",tmp2);
	j=-1;
	for (i=0;i<MAXCOLS;i++)
	{
		if ((col_type[i]&2)==2)
		{
			if (j!=-1) fprintf(f,",");
			fprintf(f,"%s",col_names[i]);
			j=i;
		}
	}
	for (i=0;i<MAXCOLS;i++)
	{
		if ((col_type[i]&1)==1)
		{
			if (j!=-1) fprintf(f,",");
			fprintf(f,"%s",col_names[i]);
			j=i;
		}
	}
	fprintf(f,",clk);\n");
	for (i=0;i<MAXCOLS;i++)
	{
		if (strlen(col_names[i])!=0 && maxcol<i) maxcol=i;
		for (j=0;j<MAXROWS;j++)
		{
			if (strlen(cell_content[j][i])!=0) 
			{
				if (maxcol<i) maxcol=i;
				if (maxrow<j) maxrow=j;
			}		
		}
	}	

	for (i=0;i<MAXCOLS;i++)
	{
		if ((col_type[i]&2)==2)
		{
			msb[i]=(col_type[i]&16376)>>3;
			lsb[i]=(col_type[i]&33538048)>>14;
			if (msb[i]<lsb[i]) {von[i]=msb[i];bis[i]=lsb[i];} else {von[i]=lsb[i];bis[i]=msb[i];}
			bitbreite[i]=msb[i]-lsb[i];
			tmpbreite=tmpbreite+bitbreite[i]+1;
			fprintf(f,"\toutput [%i:%i] %s;\n",msb[i],lsb[i],col_names[i]);
		}
	}
	for (i=0;i<MAXCOLS;i++)
	{
		if ((col_type[i]&1)==1)
		{
			msb[i]=(col_type[i]&16376)>>3;
			lsb[i]=(col_type[i]&33538048)>>14;
			if (msb[i]<lsb[i]) {von[i]=msb[i];bis[i]=lsb[i];} else {von[i]=lsb[i];bis[i]=msb[i];}
			bitbreite[i]=msb[i]-lsb[i];
			fprintf(f,"\tinput [%i:%i] %s;\n",msb[i],lsb[i],col_names[i]);
		}
	}
	fprintf(f,"\tinput\tclk;\n");
	fprintf(f,"\treg [%i:0] tmp;\n",(tmpbreite-1));
	fprintf(f,"\n\n");	
	j=0;
	k=0;
        for (i=0;i<MAXCOLS;i++)
        {
                if ((col_type[i]&2)==2)
                {
                        msb[i]=(col_type[i]&16376)>>3;
                        lsb[i]=(col_type[i]&33538048)>>14;
                        if (msb[i]<lsb[i]) {von[i]=msb[i];bis[i]=lsb[i];} else {von[i]=lsb[i];bis[i]=msb[i];}
                        bitbreite[i]=msb[i]-lsb[i];
			for (j=lsb[i];j<=msb[i];j++)
			{		
	                        fprintf(f,"\tassign %s[%i]=tmp[%i];\n",col_names[i],j,k);
				k++;
			}
                }
        }
	fprintf(f,"\talways @(posedge clk)\n\tbegin\n\t\tcase ({");
	k=0;
        for (i=0;i<MAXCOLS;i++)
        {
                if ((col_type[i]&1)==1)
                {
			if (k!=0) fprintf(f,",");
                        msb[i]=(col_type[i]&16376)>>3;
                        lsb[i]=(col_type[i]&33538048)>>14;
                        fprintf(f,"%s[%i:%i]",col_names[i],msb[i],lsb[i]);
			k=k+bitbreite[i]+1;
                }
        }
	fprintf(f,"})\n");
	for (g=0;g<=maxrow;g++)
	{
		fprintf(f,"\t\t %i'b",k);
		for (i=0;i<MAXCOLS;i++) 
		{
			if ((col_type[i]&1)==1) { // erstmal alle inputs auf die linke seite schreiben
				y=atoi(cell_content[g][i]);  // - wird automatisch zu 0. der wird dann nicht beachtet. ganz einfach.
				for (j=von[i];j<=bis[i];j++)
				{
					if ((y==0)&&(cell_content[g][i][0]!='0')) {fprintf(f,"x"); }else {
				
						x=(1<<j); // gehen wir erstmal von little endian aus TODO
						if ((y&x)==x) { fprintf(f,"1");} else fprintf(f,"0");
					}
				}
			}
		}
		fprintf(f,":\t");
		fprintf(f,"\t\t%i'b",tmpbreite);
		for (i=0;i<MAXCOLS;i++) 
		{
			if ((col_type[i]&2)==2) { // erstmal alle inputs auf die linke seite schreiben
				y=atoi(cell_content[g][i]);  // - wird automatisch zu 0. der wird dann nicht beachtet. ganz einfach.
				for (j=von[i];j<=bis[i];j++)
				{
					x=(1<<j); // gehen wir erstmal von little endian aus TODO
					if ((y&x)==x) { fprintf(f,"1");} else fprintf(f,"0");
				}
			}
		}
		fprintf(f,";\n");	
	}
	fprintf(f,"\tendcase\n\tend\n");
	fprintf(f,"endmodule\n\n");	
	fclose(f);

	free(tmp);
}
void verilogoutput(char *filename)
{
	char *tmp;
	char *tmp2;
	int g,h,i,j,k,l,y,x,z;
	FILE *f;
	int bitbreite[MAXCOLS];
	int msb[MAXCOLS];
	int lsb[MAXCOLS];
	int von[MAXCOLS];
	int bis[MAXCOLS];
	int and=0;
	int or=0;
	tmp=malloc(strlen(filename)+3);
	tmp2=malloc(strlen(filename)+1);
	strncpy(tmp2,filename,strlen(filename)+1);
	strncpy(tmp,filename,strlen(filename));
	tmp[strlen(filename)  ]='.';
	tmp[strlen(filename)+1]='v';
	tmp[strlen(filename)+2]=0;
	for (i=0;i<strlen(tmp2);i++) if (tmp2[i]=='.') tmp2[i]='_';		
	f=fopen(tmp,"w");
	fprintf(f,"module %s(",tmp2);
	j=-1;
	for (i=0;i<MAXCOLS;i++)
	{
		if ((col_type[i]&2)==2)
		{
			if (j!=-1) fprintf(f,",");
			fprintf(f,"%s",col_names[i]);
			j=i;
		}
	}
	for (i=0;i<MAXCOLS;i++)
	{
		if ((col_type[i]&1)==1)
		{
			if (j!=-1) fprintf(f,",");
			fprintf(f,"%s",col_names[i]);
			j=i;
		}
	}
	fprintf(f,");\n");

	for (i=0;i<MAXCOLS;i++)
	{
		if ((col_type[i]&2)==2)
		{
			msb[i]=(col_type[i]&16376)>>3;
			lsb[i]=(col_type[i]&33538048)>>14;
			if (msb[i]<lsb[i]) {von[i]=msb[i];bis[i]=lsb[i];} else {von[i]=lsb[i];bis[i]=msb[i];}
			bitbreite[i]=msb[i]-lsb[i];
			fprintf(f,"\toutput [%i:%i] %s;\n",msb[i],lsb[i],col_names[i]);
		}
	}
	for (i=0;i<MAXCOLS;i++)
	{
		if ((col_type[i]&1)==1)
		{
			msb[i]=(col_type[i]&16376)>>3;
			lsb[i]=(col_type[i]&33538048)>>14;
			if (msb[i]<lsb[i]) {von[i]=msb[i];bis[i]=lsb[i];} else {von[i]=lsb[i];bis[i]=msb[i];}
			bitbreite[i]=msb[i]-lsb[i];
			fprintf(f,"\tinput [%i:%i] %s;\n",msb[i],lsb[i],col_names[i]);
		}
	}
	fprintf(f,"\n\n");	
	for (i=0;i<MAXCOLS;i++) if ((col_type[i]&2)==2)
	{
		for (j=von[i];j<=bis[i];j++)
		{
			fprintf(f,"\tassign %s[%i]=",col_names[i],j);
			x=(1<<j); // gehen wir erstmal von little endian aus TODO
			for (g=0;g<MAXROWS;g++)
			{
				y=atoi(cell_content[g][i]);  // - wird automatisch zu 0. der wird dann nicht beachtet. ganz einfach.
				if ((y&x)==x) // hier ist die ausabe positiv.
				{
					if (or==1) fprintf(f,"\n\t\t|");
					fprintf(f,"(");
					and=0;
					for (h=0;h<MAXCOLS;h++) if ((col_type[h]&1)==1)
					{
						if (strcmp(cell_content[g][h],"-")!=0) {
							z=atoi(cell_content[g][h]);			
							for (k=von[h];k<=bis[h];k++)
							{
								if (and==1) fprintf(f,"&");
								if ((z&(1<<k))!=(1<<k)) fprintf(f,"~"); else fprintf(f," ");
								and=1;
								fprintf(f,"%s[%i]",col_names[h],k);
							} 
						} 
					}	
					fprintf(f,")");
					or=1;
				}
			}
			if (or==0) fprintf(f,"0");
			fprintf(f,";\n");
			or=0;	
		}
	}

	fprintf(f,"endmodule\n\n");	
	fclose(f);

	free(tmp);
}



long bin2long(const char* s)
{
	long l=strlen(s);
	int i;
	long r=0;
	char c[2];
	c[1]=0;
	for (i=0;i<l;i++)
	{
		c[0]=s[i];
		r=r+((atoi(c))<<(l-1-i));
	}	
	return r;
}
void inputbin2long(const char* s,long o,FILE *f)
{
	char *s2;
	int l,i;
	int strich=0;
	int num;
	char c[2];
	l=strlen(s);
	
	s2=malloc(l+2);
	strncpy(s2,s,l+1);
	c[1]=0;
	for (i=0;i<l && strich==0;i++)
	{
		c[0]=s2[i];
		num=atoi(c);
		if (num==0 && s2[i]!='0') //dontcare
		{
			strich=1;
			s2[i]='0';
			inputbin2long(s2,o,f);
			s2[i]='1';
			inputbin2long(s2,o,f);	
		}
	}
	if (strich==0)
	{
		fprintf(f,"./batcher w 3 %i %i\n",((bin2long(s2))<<2),o);
	}
}	
void batcheroutput()
{
	char *tmp;
	char *tmp2;
	int g,h,i,j,k,l,y,x,z;
	FILE *f;
	int bitbreite[MAXCOLS];
	int msb[MAXCOLS];
	int lsb[MAXCOLS];
	int von[MAXCOLS];
	int bis[MAXCOLS];
	int maxcol=0;
	int maxrow=0;
	int inputbits=0;
	int outputbits=0;
	int and=0;
	int or=0;
	int num=0;
	int rowempty=0;
	long o;
	char *inputbitweise;
	char *outputbitweise;
	tmp=malloc(strlen(filename)+4);
	strncpy(tmp,filename,strlen(filename));
	tmp[strlen(filename)  ]='.';
	tmp[strlen(filename)+1]='s';
	tmp[strlen(filename)+2]='h';
	tmp[strlen(filename)+3]=0;
	f=fopen(tmp,"w");
//coltype[i]&1=1: input. coltype[i]&2=2: output
//cellcontent[row][col]
//	
//
	fprintf(f,"#!/bin/bash\n");	
	for (i=0;i<MAXCOLS;i++)
	{
		msb[i]=(col_type[i]&16376)>>3;		
		lsb[i]=(col_type[i]&33538048)>>14;
		bitbreite[i]=msb[i]-lsb[i];
		if ((col_type[i]&3)!=0) maxcol=i;
		if ((col_type[i]&2)==2) outputbits=outputbits+bitbreite[i]+1;
		if ((col_type[i]&1)==1) inputbits=inputbits+bitbreite[i]+1;
		for (j=maxrow;j<MAXROWS;j++)
		{
			if (strlen(cell_content[j][i])!=0 && maxrow<j) maxrow=j;	
		}
	}
	maxcol++;
	maxrow++;
	for (i=0;i<maxrow;i++)
	{
		inputbitweise=malloc(inputbits+1);
		inputbitweise[inputbits-1]=0;	
		inputbitweise[inputbits]=0;	
		outputbitweise=malloc(outputbits+1);
		outputbitweise[outputbits-1]=0;	
		outputbitweise[outputbits]=0;	
		g=0;
		h=0;
		for (j=0;j<maxcol;j++)
		{
			if ((col_type[j]&1)==1)
			{
				num=atoi(cell_content[i][j]);
				if (cell_content[i][j][0]!='0' && num==0) // ein don't care!
				{
					for (k=0;k<=bitbreite[j];k++) 
					{
						inputbitweise[g+k]='-';	
					}
				} else {
					for (k=0;k<=bitbreite[j];k++) 
					{
						inputbitweise[g+k]=48+((num&(1<<(bitbreite[j]-k)))>>(bitbreite[j]-k));
					}
				}
				g=g+bitbreite[j]+1;
			}
			if ((col_type[j]&2)==2)
			{
				num=atoi(cell_content[i][j]);
				if (cell_content[i][j][0]!='0' && num==0) // ein don't care!
				{
					for (k=0;k<=bitbreite[j];k++) outputbitweise[h+k]='-';
				} else {
					for (k=0;k<=bitbreite[j];k++) outputbitweise[h+k]=48+((num&(1<<(bitbreite[j]-k)))>>(bitbreite[j]-k));
				}
				h=h+bitbreite[j]+1;
			}
		}
		o=bin2long(outputbitweise);
		fprintf(f,"#%s  -> %s %i\n",inputbitweise,outputbitweise,o);
		inputbin2long(inputbitweise,o,f);
		free(inputbitweise);
		free(outputbitweise);
			
	}	
	fclose(f);
}
void show_outputmenu(WINDOW *parent)
{
	int i;
	char *tmp;
	while (i!=6) 
	{
		new_menu(1);
		draw_frame(parent,2,2,23,50,' ');
		headline(parent,2,2,"OUTPUT as...");
		menu_item(0,3,3,"<%Name>",'n','N',0);
		menu_item(1,5,3,"<%Postscript>",'p','P',0);
		menu_item(2,6,3,"<%Verilog>",'v','V',0);
		menu_item(3,7,3,"<%Html>",'h','H',0);
		menu_item(4,8,3,"<%Batcher>",'b','B',0);
		menu_item(5,9,3,"<V%erilog 2>",'e','E',0);
		menu_item(6,22,3,"<%Cancel>",'c','C',0);
		wattrset(parent,attrs[COLOR_BRACKETS]);
		mvwaddch(parent,3,10,'[');
		mvwaddch(parent,3,49,']');
		wattrset(parent,attrs[COLOR_TEXT]);
		mvwaddstr(parent,3,11,filename);
		i=menu_show(parent);
		if (i==0) 
		{
			tmp=input(stdscr,3,11,38,filename,38);
			free(filename);
			filename=malloc(strlen(tmp)+1);
			strncpy(filename,tmp,strlen(tmp)+1);	
		}
		if (i==2)
		{
			verilogoutput(filename);		
		}
		if (i==3)
		{
			htmloutput(filename);
		}
		if (i==4)
		{
			batcheroutput(filename);
		}
		if (i==5)
		{
			verilog2output(filename);
		}
	
	}

	
}
int main(int argc, char *argv[])
{
	int ch;
	int i,j,k;
	signed int startrow=0;
	signed int startcol=0;
	signed int actrow=0;
	signed int actcol=0;
	signed int newrow=0;
	signed int newcol=0;
	signed int oldrow=0;
	signed int oldcol=0;
	int redraw=0;
	char *tmp;

	if (argc>1)
	{
		if (strcmp(argv[1],"-gpl")==0) {print_gpl();exit(0);}
		filename=malloc(1+strlen(argv[1]));
		strncpy(filename,argv[1],1+strlen(argv[1]));
	} else {
		filename=malloc(1);
		filename[0]=0;
	}
	uimain(argc,argv);
	init();
	wclear(stdscr);
	wrefresh(stdscr);
	redraw=1;
	if (strlen(filename)!=0) {
		asciiload(filename);
		formatload(filename);
	}

	for (;;) {
		if (actrow<startrow) {print_colnames(stdscr,startcol,actcol,1);} else print_colnames(stdscr,startcol,actcol,0);
		if (redraw) {
			show_bottommenu(bottommenu);
			print_rownames(stdscr,startrow);
			print_table(stdscr,startrow,startcol);
//			fprintf(stderr,"maxrow: %i\n",maxrow);
//			fprintf(stderr,"maxcol: %i\n",maxcol);
			touchwin(stdscr);
			wrefresh(stdscr);
		} else {
			if (oldrow>=startrow) print_cell(stdscr,startrow,startcol,oldrow,oldcol,calcattr(cell_format[oldrow][oldcol]));
		}
		if (actrow>=startrow) print_cell(stdscr,startrow,startcol,actrow,actcol,calcattr(cell_format[actrow][actcol])^A_REVERSE);
		oldrow=actrow;
		oldcol=actcol;
		ch=getch2();
		redraw=0;
	
		if ((printable(ch) || ch==KEY_BACKSPACE || ch==KEY_DELETE) && actrow<startrow) 
		{
			if (ch==KEY_DELETE) tmp=input(stdscr,0,findx(startcol,actcol),col_width[actcol],"",col_width[actcol]); else tmp=input2(stdscr,0,findx(startcol,actcol),col_width[actcol],col_names[actcol],col_width[actcol],0,ch);
			free(col_names[actcol]);
			col_names[actcol]=malloc(strlen(tmp)+1);
			strncpy(col_names[actcol],tmp,strlen(tmp)+1);
			free(tmp);
			redraw=1;
			if (lastkey==KEY_RETURN) ch=KEY_RIGHT;
		}
		if ((printable(ch) || ch==KEY_RETURN) && actrow>=startrow) 
		{
			if (ch==KEY_RETURN) ch=0;
			//tmp=input2(stdscr,findy(startrow,actrow),findx(startcol,actcol),col_width[actcol],cell_content[actrow][actcol],col_width[actcol],1,ch);
			tmp=input2(stdscr,findy(startrow,actrow),findx(startcol,actcol),col_width[actcol],"",col_width[actcol],1,ch);
//			fprintf(stderr,"%s",tmp);
			free(cell_content[actrow][actcol]);
			cell_content[actrow][actcol]=malloc(strlen(tmp)+1);
			strncpy(cell_content[actrow][actcol],tmp,strlen(tmp)+1);
	//		redraw=1;
			if (markon==1 && ch!=KEY_ESC) 
			{
				for (i=mark_upperrow;i<=mark_lowerrow;i++)
				{
					for (j=mark_leftcol;j<=mark_rightcol;j++)
					{
						cell_content[i][j]=malloc(strlen(tmp)+1);
						strncpy(cell_content[i][j],tmp,strlen(tmp)+1);
						
					}
				}	
				markon=0;
				redraw=1;
			}
			free(tmp);
			if (lastkey==KEY_RETURN) {ch=KEY_DOWN;} else ch=lastkey;
		}
		if (ch==KEY_RETURN) ch=0;
		if (bottommenu==0) {
			if (ch==KEY_F(1)) {
				sort_by(actcol);
				wclear(stdscr);
				wrefresh(stdscr);
				redraw=1;
			}
			if (ch==KEY_F(2)) {
				tmp=input(stdscr,findy(startrow,actrow),findx(startcol,actcol),col_width[actcol],cell_content[actrow][actcol],col_width[actcol]);
//				fprintf(stderr,"%s",tmp);
				free(cell_content[actrow][actcol]);
				cell_content[actrow][actcol]=malloc(strlen(tmp)+1);
				strncpy(cell_content[actrow][actcol],tmp,strlen(tmp)+1);
		//		redraw=1;
				free(tmp);
				if (lastkey==KEY_RETURN) {ch=KEY_DOWN;} else ch=lastkey;
			}
			if (ch==KEY_F(3) && markon==1)
			{
				markon=0;
				ch=0;
				redraw=1;
			}
			if (ch==KEY_F(3) && markon==0 && actrow>=startrow)
			{
				markon=1;
				mark_upperrow=actrow;
				mark_lowerrow=actrow;
				mark_leftcol=actcol;
				mark_rightcol=actcol;
				mark_cursorpos=0;
				redraw=1;
			}
			if (ch==KEY_F(4) && actcol!=MAXCOLS-1 && actcol!=0) {
				col_width[actcol]=-col_width[actcol];
				redraw=1;
				if (markon==1)
				{
					for (j=mark_leftcol;j<=mark_rightcol;j++)
					{
						if (col_width[j]>0) col_width[j]=-col_width[j];
					}

				}
				markon=0;
				ch=KEY_RIGHT;
			}
			if (ch==KEY_F(5)) {
				j=startcol;
				k=actcol;
				for (i=0;i<MAXCOLS;i++) 
				{
					if (col_width[i]<0) {
						col_width[i]=-col_width[i];
						if (i<k && i>=j && i<=maxcol) {
							startcol++;
						}
					}
				}
				j=startrow;
				k=actrow;
				for (i=0;i<MAXROWS;i++) 
				{
					if (row_height[i]<0) {
						row_height[i]=-row_height[i];
						if (i<k && i>=j && i<=maxrow) {
							startrow++;
						}
	
					}
				}
				redraw=1;
			}
			if (ch==KEY_F(9))
			if (actrow<startrow) 
			{
				format_menu(stdscr,actcol);
				redraw=1;
			} else {
				colmenu(stdscr,mark_leftcol,mark_rightcol,mark_upperrow,mark_lowerrow,markon,actrow,actcol);
				redraw=1;	
			}
		}
		if (bottommenu==1)
		{
			if (ch==KEY_F(2))
			{
				//delrow
				for (j=0;j<MAXCOLS;j++)
				{
					for (i=actrow;i<MAXROWS-1;i++)
					{
						free(cell_content[i][j]);
                                                cell_content[i][j]=malloc(strlen(cell_content[i+1][j])+1);
                                                strncpy(cell_content[i][j],cell_content[i+1][j],strlen(cell_content[i+1][j])+1);
                                                cell_format[i][j]=cell_format[i+1][j];
					}
					free(cell_content[MAXROWS-1][j]);
					cell_content[MAXROWS-1][j]=malloc(1);
					cell_content[MAXROWS-1][j][0]=0;
					cell_format[MAXROWS-1][j]=312;
				}
				
				redraw=1;
			}
			if (ch==KEY_F(3))
			{
				// delcol

				for (j=actcol;j<MAXCOLS-1;j++)
				{
					free(col_names[j]);
					col_names[j]=malloc(strlen(col_names[j+1])+1);
					strncpy(col_names[j],col_names[j+1],strlen(col_names[j+1])+1);	
					col_width[j]=col_width[j+1];
					col_type[j]=col_type[j+1];
				}
				col_type[MAXCOLS-1]=0;
				col_width[MAXCOLS-1]=5;
				free(col_names[MAXCOLS-1]);
				col_names[MAXCOLS-1]=malloc(1);
				col_names[MAXCOLS-1][0]=0;
				for (i=0;i<MAXROWS;i++)
				{
					for (j=actcol;j<MAXCOLS-1;j++)
					{
						free(cell_content[i][j]);
						cell_content[i][j]=malloc(strlen(cell_content[i][j+1])+1);
						strncpy(cell_content[i][j],cell_content[i][j+1],strlen(cell_content[i][j+1])+1);	
						cell_format[i][j]=cell_format[i][j+1];
					}
					free(cell_content[i][MAXCOLS-1]);
					cell_content[i][MAXCOLS-1]=malloc(1);
					cell_content[i][MAXCOLS-1][0]=0;
					cell_format[i][MAXCOLS-1]=312;
				}
				redraw=1;
			}
			if (ch==KEY_F(4))
			{
				//insrow
				for (j=0;j<MAXCOLS;j++)
				{
					for (i=MAXROWS-1;i>actrow;i--)
					{
						free(cell_content[i][j]);
                                                cell_content[i][j]=malloc(strlen(cell_content[i-1][j])+1);
                                                strncpy(cell_content[i][j],cell_content[i-1][j],strlen(cell_content[i-1][j])+1);
                                                cell_format[i][j]=cell_format[i-1][j];
					}
					free(cell_content[actrow][j]);
					cell_content[actrow][j]=malloc(1);
					cell_content[actrow][j][0]=0;
					cell_format[actrow][j]=312;
				}
				
				redraw=1;
			}
			if (ch==KEY_F(5))
			{
				// inscol
				for (j=MAXCOLS-1;j>actcol;j--)
				{
					free(col_names[j]);
					col_names[j]=malloc(strlen(col_names[j-1])+1);
					strncpy(col_names[j],col_names[j-1],strlen(col_names[j-1])+1);	
					col_width[j]=col_width[j-1];
					col_type[j]=col_type[j-1];
				}
				free(col_names[actcol]);
				col_names[actcol]=malloc(1);
				col_names[actcol][0]=0;
				col_type[actcol]=0;
				col_width[actcol]=5;
				for (i=0;i<MAXROWS;i++)
				{
					for (j=MAXCOLS-1;j>actcol;j--)
					{
						free(cell_content[i][j]);
						cell_content[i][j]=malloc(strlen(cell_content[i][j-1])+1);
						strncpy(cell_content[i][j],cell_content[i][j-1],strlen(cell_content[i][j-1])+1);	
						cell_format[i][j]=cell_format[i][j-1];
					}
					free(cell_content[i][actcol]);
					cell_content[i][actcol]=malloc(1);
					cell_content[i][actcol][0]=0;
					cell_format[i][actcol]=312;
				}
				redraw=1;
			}
			if (ch==KEY_F(6))
			{
				show_outputmenu(stdscr);
				redraw=1;
			}
		}
		if (ch==KEY_DELETE)
		{
			free(cell_content[actrow][actcol]);
			cell_content[actrow][actcol]=malloc(1);
			cell_content[actrow][actcol][0]=0;
			if (markon==1) {
				for (i=mark_upperrow;i<=mark_lowerrow;i++)
				{
					for (j=mark_leftcol;j<=mark_rightcol;j++)
					{
						free(cell_content[i][j]);
						cell_content[i][j]=malloc(1);
						cell_content[i][j][0]=0;
						
					}
				}
				markon=0;
				redraw=1;
			}
		}
		if (ch==KEY_UP && actrow>=0 && markon==0) 
		{
			
			newrow=actrow-1;
			while (newrow>=0 && row_height[abs(newrow)]<0) newrow--;
			if (newrow>=startrow) {
				if (row_height[newrow]>0) actrow=newrow;
			} else {actrow=newrow;}
			if (actrow<startrow-1) 
			{
				startrow=actrow+1;
				redraw=1;
			}

		}
		if (ch==KEY_UP && actrow>0 && markon==1) 
		{
			newrow=actrow-1;
			while (newrow>0 && row_height[newrow]<0) newrow--;
			if (row_height[newrow]>0) actrow=newrow;
			if (actrow<startrow) 
			{	
				startrow=actrow;
				redraw=1;
			}
			if (mark_upperrow>actrow && markon==1 && ((mark_cursorpos&1)==1 || mark_upperrow==mark_lowerrow)) {
				mark_upperrow=actrow;
				redraw=1;
				if (mark_upperrow!=mark_lowerrow) {mark_cursorpos=mark_cursorpos|1;} else {mark_cursorpos=mark_cursorpos&126;}
				
			}
			if (mark_lowerrow>actrow && markon==1 && ((mark_cursorpos&4)==4 || mark_upperrow==mark_lowerrow)) {
				mark_lowerrow=actrow;
				redraw=1;
				if (mark_upperrow!=mark_lowerrow) {mark_cursorpos=mark_cursorpos|4;} else {mark_cursorpos=mark_cursorpos&123;}
			}
	

		}
		if (ch==KEY_DOWN) 
		{
			newrow=actrow+1;
			while (newrow<MAXROWS && row_height[newrow]<0) newrow++;
			if (newrow<MAXROWS) actrow=newrow;
			if (actrow>=maxrow)
			{	
				startrow=startrow+(actrow-newrow+1);
				redraw=1;
			}
				
			if (mark_lowerrow<actrow && markon==1 && ((mark_cursorpos&4)==4 || mark_upperrow==mark_lowerrow))
			{
				mark_lowerrow=actrow;
				redraw=1;
				if (mark_upperrow!=mark_lowerrow) {mark_cursorpos=mark_cursorpos|4;} else {mark_cursorpos=mark_cursorpos&123;}
			}
			if (mark_upperrow<actrow && markon==1 && ((mark_cursorpos&1)==1 || mark_upperrow==mark_lowerrow))
			{
				mark_upperrow=actrow;
				redraw=1;
				if (mark_upperrow!=mark_lowerrow) {mark_cursorpos=mark_cursorpos|1;} else {mark_cursorpos=mark_cursorpos&126;}
			}
			
	
		}
		if (ch==KEY_LEFT && actcol>0) {
			newcol=actcol-1;
			while (newcol>0 && col_width[newcol]<0) newcol--;
			if (col_width[newcol]>0) actcol=newcol;
			if (actcol<startcol)
			{
				startcol=actcol;
				redraw=1;
			}
			if (mark_leftcol>actcol && markon==1 && ((mark_cursorpos&8)==8 || mark_leftcol==mark_rightcol)) {
				mark_leftcol=actcol;
				redraw=1;
				if (mark_leftcol!=mark_rightcol) {mark_cursorpos=mark_cursorpos|8;} else {mark_cursorpos=mark_cursorpos&119;}
				
			}
			if (mark_rightcol>actcol && markon==1 && ((mark_cursorpos&2)==2 || mark_leftcol==mark_rightcol)) {
				mark_rightcol=actcol;
				redraw=1;
				if (mark_leftcol!=mark_rightcol) {mark_cursorpos=mark_cursorpos|2;} else {mark_cursorpos=mark_cursorpos&125;}
			}
		}
		if (ch==KEY_RIGHT) 
		{
			newcol=actcol+1;
			while (newcol<MAXCOLS && col_width[newcol]<0) newcol++;
			if (newcol<MAXCOLS) actcol=newcol;
			if (actcol>=maxcol)
			{
				startcol=startcol+(actcol-newcol+1);
				redraw=1;
			}
			if (mark_rightcol<actcol && markon==1 && ((mark_cursorpos&2)==2 || mark_leftcol==mark_rightcol))
			{
				mark_rightcol=actcol;
				redraw=1;
				if (mark_leftcol!=mark_rightcol) {mark_cursorpos=mark_cursorpos|2;} else {mark_cursorpos=mark_cursorpos&125;}
			}
			if (mark_leftcol<actcol && markon==1 && ((mark_cursorpos&8)==8 || mark_leftcol==mark_rightcol))
			{
				mark_leftcol=actcol;
				redraw=1;
				if (mark_leftcol!=mark_rightcol) {mark_cursorpos=mark_cursorpos|8;} else {mark_cursorpos=mark_cursorpos&119;}
			}
		}
		if (ch==KEY_F(10))
		{
			exit_menu();
			redraw=1;
		}
		if (ch==KEY_F(11))
		{
			wclear(stdscr);
			wrefresh(stdscr);
			redraw=1;
		}
		if (ch==KEY_F(12))
		{
			bottommenu++;
			redraw=1;
			if (bottommenu>MAXBOTTOMMENU) bottommenu=0;
		}
		
	}
	finish(0);
}
