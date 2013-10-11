#include <dos.h>
#include <hwinfo.h>
#include <string.h>
#include <stdio.h>
#include <curses.h>

void PrintFeature( const CPU_INFO *p_pInfo, const char p_Register,
	const uint32_t p_Feature, const char *p_pFeature, const uint32_t p_Y,
	const uint32_t p_X )
{
	if( p_Register == 'c' )
	{
		if( p_pInfo->Features.ECX & p_Feature )
		{
			mvprintw( p_Y, p_X, "[%s] (%d)\n", p_pFeature, 1 );
		}
		else
		{
			mvprintw( p_Y, p_X, "[%s] (%d)\n", p_pFeature, 0 );
		}
	}
	if( p_Register == 'd' )
	{
		if( p_pInfo->Features.EDX & p_Feature )
		{
			mvprintw( p_Y, p_X, "[%s] (%d)\n", p_pFeature, 1 );
		}
		else
		{
			mvprintw( p_Y, p_X, "[%s] (%d)\n", p_pFeature, 0 );
		}
	}
}

void HWI_CPUID( const uint32_t p_Operator, CPU_REGS *p_pRegs )
{
	asm volatile
	(
		"\t"
		"pushl	%%ebx\n\t"
		"cpuid\n\t"
		"movl	%%ebx,	%1\n\t"
		"popl	%%ebx\n\t"
		:	"=a"( p_pRegs->EAX ),
			"=r"( p_pRegs->EBX ),
			"=c"( p_pRegs->ECX ),
			"=d"( p_pRegs->EDX )
		: "a"( p_Operator )
		: "cc", "%ebx"
	);
}

void HWI_GetCPUInformation( CPU_INFO *p_pInfo )
{
	CPU_REGS Regs;
	uint32_t EAX, EBX, ECX, EDX;

	memset( p_pInfo->Vendor, '\0', sizeof( char )*13 );

	/* Maximum CPUID value and vendor name */
	HWI_CPUID( 0, &Regs );
	memcpy( p_pInfo->Vendor, &Regs.EBX, sizeof( char )*4 );
	memcpy( p_pInfo->Vendor+8, &Regs.ECX, sizeof( char )*4 );
	memcpy( p_pInfo->Vendor+4, &Regs.EDX, sizeof( char )*4 );

	/* CPU Features */
	HWI_CPUID( 1, &Regs );
	p_pInfo->Features = Regs;
}

void HWI_ShowCPUInformation( const CPU_INFO *p_pInfo )
{
	uint32_t eax, ebx, ecx, edx, CPUID = 0;
	char CPUName[ 13 ];
	memset( CPUName, '\0', 13 );

	/* Ideally, this will be split into pages showing the features, extended
	 * features, cache information, and other useful information
	 */
	initscr( );
	raw( );
	noecho( );
	curs_set( 0 );
	printw( "CPU Information\n" );
	printw( "---------------\n" );
	printw( "Vendor: %s | Model: Model here...", p_pInfo->Vendor );

	mvprintw( 4, 0, "ECX" );

	PrintFeature( p_pInfo, 'c', CPU_FEATURE_SSE3, "SSE3", 5, 0 );
	PrintFeature( p_pInfo, 'c', CPU_FEATURE_PCLMUL, "PCLMUL", 6, 0 );
	PrintFeature( p_pInfo, 'c', CPU_FEATURE_DTES64, "DTES64", 7, 0 );
	PrintFeature( p_pInfo, 'c', CPU_FEATURE_MONITOR, "MONITOR", 8, 0 );
	PrintFeature( p_pInfo, 'c', CPU_FEATURE_DS_CPL, "DS_CPL", 9, 0 );
	PrintFeature( p_pInfo, 'c', CPU_FEATURE_MMX, "MMX", 10, 0 );

	mvprintw( 4, 20, "EDX" );
	PrintFeature( p_pInfo, 'd', CPU_FEATURE_FPU, "FPU", 5, 20 );

	mvprintw( 24, 0, "<FEATURES>" );

	refresh( );

	getch( );

	endwin( );

	SetVideoMode( 0x03 );
}


