#include <dos.h>
#include <sys/nearptr.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define VIDEO_INT			0x10
#define WRITE_PIXEL			0x0C
#define SET_VIDEO_MODE		0x00
#define VGA_256_COLOUR_MODE	0x13
#define TEXT_MODE			0x03

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	200
#define NUM_COLOURS		256

typedef unsigned char	BYTE;
typedef unsigned short	WORD;

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

int main( int p_Argc, char **p_ppArgv )
{
	const int Colour = 1;
	float T1;
	WORD i, Start;

	if( __djgpp_nearptr_enable( ) == 0 )
	{
		printf( "Failed to get access to the first 640KiB of memory.\n" );

		return 1;
	}

	VGA += __djgpp_conventional_base;
	Clock = ( void * )Clock + __djgpp_conventional_base;

	SetVideoMode( VGA_256_COLOUR_MODE );

	Start = *Clock;
	for( i = 0; i < ( 320*200 ); ++i )
	{
		PlotPixelDirect( i, Colour );
	}

	T1 = ( *Clock - Start )/18.2f;

	sleep( 2 );

	SetVideoMode( TEXT_MODE );

	printf( "Took %f seconds to clear screen\n", T1 );

	__djgpp_nearptr_disable( );

	return 0;
}

