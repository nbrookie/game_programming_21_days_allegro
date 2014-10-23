
// I N C L U D E S ///////////////////////////////////////////////////////////

#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <memory.h>
#include <math.h>
#include <string.h>

// D E F I N E S  ////////////////////////////////////////////////////////////

#define VGA256            0x13
#define TEXT_MODE         0x03

#define PALETTE_MASK        0x3c6
#define PALETTE_REGISTER_RD 0x3c7
#define PALETTE_REGISTER_WR 0x3c8
#define PALETTE_DATA        0x3c9

#define SCREEN_WIDTH      (unsigned int)320
#define SCREEN_HEIGHT     (unsigned int)200

// S T R U C T U R E S ///////////////////////////////////////////////////////

// this structure holds a RGB triple in three bytes

typedef struct RGB_color_typ
        {

        unsigned char red;    // red   component of color 0-63
        unsigned char green;  // green component of color 0-63
        unsigned char blue;   // blue  component of color 0-63

        } RGB_color, *RGB_color_ptr;


// P R O T O T Y P E S ///////////////////////////////////////////////////////

void Set_Palette_Register(int index, RGB_color_ptr color);

void Get_Palette_Register(int index, RGB_color_ptr color);

void Create_Cool_Palette(void);

void H_Line(int x1,int x2,int y,unsigned int color);

// G L O B A L S  ////////////////////////////////////////////////////////////

unsigned char far *video_buffer = (char far *)0xA0000000L; // vram byte ptr

// F U N C T I O N S /////////////////////////////////////////////////////////

void Set_Palette_Register(int index, RGB_color_ptr color)
{

// this function sets a single color look up table value indexed by index
// with the value in the color structure

// tell VGA card we are going to update a pallete register

_outp(PALETTE_MASK,0xff);

// tell vga card which register we will be updating

_outp(PALETTE_REGISTER_WR, index);

// now update the RGB triple, note the same port is used each time

_outp(PALETTE_DATA,color->red);
_outp(PALETTE_DATA,color->green);
_outp(PALETTE_DATA,color->blue);

} // end Set_Palette_Color

///////////////////////////////////////////////////////////////////////////////

void Get_Palette_Register(int index, RGB_color_ptr color)
{

// this function gets the data out of a color lookup regsiter and places it
// into color

// set the palette mask register

_outp(PALETTE_MASK,0xff);

// tell vga card which register we will be reading

_outp(PALETTE_REGISTER_RD, index);

// now extract the data

color->red   = _inp(PALETTE_DATA);
color->green = _inp(PALETTE_DATA);
color->blue  = _inp(PALETTE_DATA);

} // end Get_Palette_Color

///////////////////////////////////////////////////////////////////////////////

void Create_Cool_Palette(void)
{
// this function creates a cool palette. 64 shades of gray, 64 of red,
// 64 of green and finally 64 of blue.

RGB_color color;

int index;

// swip thru the color registers and create 4 banks of 64 colors

for (index=0; index < 64; index++)
    {

    // grays

    color.red   = index;
    color.green = index;
    color.blue  = index;

    Set_Palette_Register(index, (RGB_color_ptr)&color);

    // reds

    color.red   = index;
    color.green = 0;
    color.blue  = 0;

    Set_Palette_Register(index+64, (RGB_color_ptr)&color);

    // greens

    color.red   = 0;
    color.green = index;
    color.blue  = 0;

    Set_Palette_Register(index+128, (RGB_color_ptr)&color);

    // blues

    color.red   = 0;
    color.green = 0;
    color.blue  = index;

    Set_Palette_Register(index+192, (RGB_color_ptr)&color);

    } // end index

// make color 0 black

color.red   = 0;
color.green = 0;
color.blue  = 0;

Set_Palette_Register(0, (RGB_color_ptr)&color);

} // end Create_Cool_Palette

//////////////////////////////////////////////////////////////////////////////

void Set_Video_Mode(int mode)
{

// use the video interrupt 10h to set the video mode to the sent value

union REGS inregs,outregs;

inregs.h.ah = 0;                    // set video mode sub-function
inregs.h.al = (unsigned char)mode;  // video mode to change to

_int86(0x10, &inregs, &outregs);

} // end Set_Video_Mode

//////////////////////////////////////////////////////////////////////////////

void H_Line(int x1,int x2,int y,unsigned int color)
{
// draw a horizontal line useing the memset function
// note x2 > x1

_fmemset((char far *)(video_buffer + ((y<<8) + (y<<6)) + x1),color,x2-x1+1);

} // end H_Line

//M A I N /////////////////////////////////////////////////////////////////////

void main(void)
{

int index,           // loop var
    x1=150,          // x1 & x2 are the edges of the current piece of the road
    x2=170,
    y=0,             // y is the current y position of the piece of road
    curr_color=1;    // the current color being drawn

RGB_color color,color_1;

// set video mode to 320x200 256 color mode

Set_Video_Mode(VGA256);

// create the color palette

Create_Cool_Palette();

printf("Press any key to exit.");

// draw a road to nowhere

for (y=80; y<200; y++)
    {
    // draw next horizontal piece of road

    H_Line(x1,x2,y,curr_color);

    // make the road wider

    if (--x1 < 0)
       x1=0;

    if (++x2 > 319)
       x2=319;

    // next color please

    if (++curr_color>255)
       curr_color=1;

    } // end for

// wait for user to hit a key

while(!kbhit())
     {
     Get_Palette_Register(1,(RGB_color_ptr)&color_1);

     for (index=1; index<=254; index++)
         {
         Get_Palette_Register(index+1,(RGB_color_ptr)&color);
         Set_Palette_Register(index,(RGB_color_ptr)&color);

         } // end for

         Set_Palette_Register(255,(RGB_color_ptr)&color_1);

     } // end while

// go back to text mode

Set_Video_Mode(TEXT_MODE);

} // end main
