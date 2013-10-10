#ifndef __DUNGEONEER_HWINFO_H__
#define __DUNGEONEER_HWINFO_H__

#include <stdint.h>

#define	CPU_TYPE_AMD	"AuthenticAMD"
#define CPU_TYPE_INTEL	"GenuineIntel"

/* Dungeoneer for DOS is intended to only run on 32-bit systems */
typedef struct __CPU_REGS
{
	uint32_t	EAX;
	uint32_t	EBX;
	uint32_t	ECX;
	uint32_t	EDX;
}CPU_REGS;

typedef struct __CPU_INFO
{
	char		Vendor[ 13 ];
	char		Name[ 49 ];
	uint32_t	MaxOps;
	CPU_REGS	InfoFeature;
	CPU_REGS	InfoFeatureEx;
	CPU_REGS	CacheTLB;
	CPU_REGS	L1CacheTLB;
	CPU_REGS	L2CacheEx;
}CPU_INFO;

/* This should be called before any CPU-related hardware information is queried
 * for
 */
int HWI_SupportsCPUID( );

void HWI_GetCPUInformation( CPU_INFO *p_pInfo );
void HWI_ShowCPUInformation( const CPU_INFO *p_pInfo );

void HWI_CPUID( const uint32_t p_Operator, CPU_REGS *p_pRegs );

#endif

