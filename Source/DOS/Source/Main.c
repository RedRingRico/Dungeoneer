#include <dos.h>
#include <sys/nearptr.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define VIDEO_INT			0x10
#define WRITE_PIXEL			0x0C
#define SET_VIDEO_MODE		0x00
#define VGA_256_COLOUR_MODE	0x13
#define TEXT_MODE			0x03

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	200
#define NUM_COLOURS		256

#define Sign( N ) ( ( N < 0 ) ? -1 :( ( N > 0 ) ? 1: 0 ) )

typedef unsigned char	BYTE;
typedef unsigned short	WORD;

typedef struct __POINT
{
	int X;
	int Y;
}POINT;

BYTE *VGA = ( BYTE * )0xA0000;
WORD *Clock = ( WORD * )0x046C;

void SetVideoMode( const BYTE p_Mode )
{
	union REGS Reg;

	Reg.h.ah = SET_VIDEO_MODE;
	Reg.h.al = p_Mode;

	int86( VIDEO_INT, &Reg, &Reg );
}

void PlotPixel( const int p_X, const int p_Y, const BYTE p_Colour )
{
	VGA[ ( p_Y << 8 ) + ( p_Y << 6 ) + p_X ] = p_Colour;
}

void PlotPixelDirect( const int p_Position, const BYTE p_Colour )
{
	VGA[ p_Position ] = p_Colour;
}

void DrawLine( const POINT p_Point1, const POINT p_Point2, BYTE p_Colour )
{
	int i, dX, dY, sdX, sdY, dXAbs, dYAbs, X, Y, PlotX, PlotY;

	dX = p_Point2.X - p_Point1.X;
	dY = p_Point2.Y - p_Point1.Y;
	dXAbs = abs( dX );
	dYAbs = abs( dY );
	sdX = Sign( dX );
	sdY = Sign( dY );
	X = dXAbs >> 1;
	Y = dYAbs >> 1;
	PlotX = p_Point1.X;
	PlotY = p_Point1.Y;

	VGA[ ( PlotY << 8 )+( PlotY << 6 ) + PlotX ] = p_Colour;

	if( dXAbs >= dYAbs )
	{
		for( i = 0; i < dXAbs; ++i )
		{
			Y += dYAbs;
			if( Y >= dXAbs )
			{
				Y -= dXAbs;
				PlotY += sdY;
			}

			PlotX += sdX;

			PlotPixel( PlotX, PlotY, p_Colour );
		}
	}
	else
	{
		for( i = 0; i < dYAbs; ++i )
		{
			X += dXAbs;
			if( X >= dYAbs )
			{
				X -= dYAbs;
				PlotX += sdX;
			}

			PlotY += sdY;

			PlotPixel( PlotX, PlotY, p_Colour );
		}
	}
}

int main( int p_Argc, char **p_ppArgv )
{
	const int Colour = 1;
	int LineColour;
	int Counter = 0;
	float T1;
	WORD i, Start;
	POINT P0, P1;

	if( __djgpp_nearptr_enable( ) == 0 )
	{
		printf( "Failed to get access to the first 640KiB of memory.\n" );

		return 1;
	}

	VGA += __djgpp_conventional_base;
	Clock = ( void * )Clock + __djgpp_conventional_base;
	srand( *Clock );
	P0.X = 0;
	P0.Y = 0;
	P1.X = 200;
	P1.Y = 200;
	LineColour = 15;

	SetVideoMode( VGA_256_COLOUR_MODE );

	Start = *Clock;

	while( T1 < 1.0f )
	{
		for( i = 0; i < ( 320*200 ); ++i )
		{
			PlotPixelDirect( i, Colour );
		}

		DrawLine( P0, P1, LineColour );

		T1 = ( *Clock - Start )/18.2f;
		++Counter;
	}

	SetVideoMode( TEXT_MODE );

	printf( "Rendered %d frames in one second\n", Counter );

	__djgpp_nearptr_disable( );

	return 0;
}

