#ifndef __DUNGEONEER_HWINFO_H__
#define __DUNGEONEER_HWINFO_H__

#include <stdint.h>

#define	CPU_TYPE_AMD	"AuthenticAMD"
#define CPU_TYPE_INTEL	"GenuineIntel"

/* ECX */
#define CPU_FEATURE_SSE3	1 << 0
#define CPU_FEATURE_PCLMUL	1 << 1
#define CPU_FEATURE_DTES64	1 << 2
#define CPU_FEATURE_MONITOR	1 << 3
#define CPU_FEATURE_DS_CPL	1 << 4
#define CPU_FEATURE_MMX		1 << 23

/* EDX */
#define CPU_FEATURE_FPU	1 << 0

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
	CPU_REGS	Features;
	CPU_REGS	FeaturesEx;
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

