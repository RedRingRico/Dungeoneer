#include <dos.h>
#include <sys/nearptr.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <dpmi.h>

#define VIDEO_INT			0x10
#define WRITE_PIXEL			0x0C
#define SET_VIDEO_MODE		0x00
#define VGA_256_COLOUR_MODE	0x13
#define TEXT_MODE			0x03

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	200
#define NUM_COLOURS		256
#define SCAN_QUEUE		256

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

BYTE g_ScanCode;
BYTE g_ScanCodeQueue[ SCAN_QUEUE ];
BYTE g_ScanCodeHead;
BYTE g_ScanCodeTail;

_go32_dpmi_seginfo	OldKeyboard, NewKeyboard;

void GetScanCode( void )
{
	g_ScanCode = 66;
	asm
	(
		"CLI;\n"
		/* Read scan code */
		"IN 	$0x60,	%%AL;\n"
		"MOV 	%%AL,	%0;\n"
		/* Read keyboard status */
		"IN 	$0x61,	%%AL;\n"
		"MOV	%%AL,	%%BL;\n"
		/* Set bit 7 */
		"OR		$0x80,	%%AL;\n"
		"OUT	%%AL,	$0x61;\n"
		/* Clear bit 7 */
		"MOV	%%BL,	%%AL;\n"
		"OUT	%%AL,	$0x61;\n"
		/* Reset PIC */
		"MOV	$0x20,	%%AL;\n"
		"OUT	%%AL,	$0x20;\n"
		"STI;\n"
		: "=r"( g_ScanCode )
		:
		: "%eax", "%ebx"
	);

	*( g_ScanCodeQueue + g_ScanCodeTail ) = g_ScanCode;
	++g_ScanCodeTail;
}

void StartKeyboard( void )
{
	NewKeyboard.pm_offset = ( int )GetScanCode;
	NewKeyboard.pm_selector = _go32_my_cs( );
	_go32_dpmi_get_protected_mode_interrupt_vector( 0x09, &OldKeyboard );

	g_ScanCodeHead = 0;
	g_ScanCodeTail = 0;
	g_ScanCode = 0;

	_go32_dpmi_allocate_iret_wrapper( &NewKeyboard );
	_go32_dpmi_set_protected_mode_interrupt_vector( 0x09, &NewKeyboard );
}

void StopKeyboard( void )
{
	_go32_dpmi_free_iret_wrapper( &NewKeyboard );
	_go32_dpmi_set_protected_mode_interrupt_vector( 0x09, &OldKeyboard );
}

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
	int Key;
	int LineColour;
	int Counter = 0;
	float T1;
	WORD i, Start;
	POINT P0, P1;
	__dpmi_free_mem_info FreeHeap;

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
	StartKeyboard( );

	Start = *Clock;

	Key = 999;

	/* The main loop should use some kind of timer to control when to run the
	 * update and also when to render to the screen
	 */
	do
	{
		while( g_ScanCodeHead != g_ScanCodeTail )
		{
			Key = *( g_ScanCodeQueue + g_ScanCodeHead );
			++g_ScanCodeHead;
		}

		for( i = 0; i < ( 320*200 ); ++i )
		{
			PlotPixelDirect( i, Colour );
		}

		DrawLine( P0, P1, LineColour );

		T1 = ( *Clock - Start )/18.2f;
		++Counter;
	}
	while( Key != 1 ); /* ESC */

	SetVideoMode( TEXT_MODE );

	__dpmi_get_free_memory_information( &FreeHeap );

	printf( "Rendered %d frames in %f seconds\n", Counter, T1 );
	printf( "Free memory: %i [heap]\n",
		FreeHeap.largest_available_free_block_in_bytes );

	StopKeyboard( );

	__djgpp_nearptr_disable( );

	return 0;
}

