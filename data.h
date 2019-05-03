unsigned short cell_format[MAXROWS][MAXCOLS]; //farbe, bold, oder nicht bold...
                                   /* code ich das so: 
                                      bit 0-2: vordergrundfarbe
						0= red
						1= green
						2= brown
						3= yellow
						4= blue
						5= magenta
						6= cyan
						7= white
                                      bit 3-5: hintergrundfarbe
                                      bit 6:   underline
                                      bit 7:   invers
                                      bit 8:   bold
                                      bit 9:   blink
                                      bit a:   dim
                                   */
char *cell_content[MAXROWS][MAXCOLS];
char *col_names[MAXCOLS];
short col_width[MAXCOLS];  //wenn <0 --> gefoldet
short row_height[MAXROWS]; //wenn <0 --> gefoldet
unsigned int col_type[MAXCOLS]; /* bit 0: input, bit 1: output, bit 2: freeform, bit 3-13: msb, bit 14-24: lsb. sind bit 0-2 auf 0, so ist der leer */
int maxrow;
int maxcol;
int mark_leftcol;
int mark_rightcol;
int mark_upperrow;
int mark_lowerrow;
int markon;
short mark_cursorpos; /* bit 0: upper row, bit 1: right col, bit 2: lower row, 3: left col */

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define REVISION 5
