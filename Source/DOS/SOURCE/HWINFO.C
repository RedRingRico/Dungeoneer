#include <hwinfo.h>
#include <string.h>

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

	HWI_CPUID( 0, &Regs );
	memcpy( p_pInfo->Vendor, &Regs.EBX, sizeof( char )*4 );
	memcpy( p_pInfo->Vendor+8, &Regs.ECX, sizeof( char )*4 );
	memcpy( p_pInfo->Vendor+4, &Regs.EDX, sizeof( char )*4 );
}

void HWI_ShowCPUInformation( const CPU_INFO *p_pInfo )
{
	uint32_t eax, ebx, ecx, edx, CPUID = 0;
	char CPUName[ 13 ];
	memset( CPUName, '\0', 13 );
	printf( "CPU Information\n" );
	printf( "---------------\n" );
	printf( "\tVendor: %s\n", p_pInfo->Vendor );

	printf( "\n" );
}

