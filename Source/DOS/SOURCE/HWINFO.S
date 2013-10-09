.section .text
.globl _HWI_SupportsCPUID
_HWI_SupportsCPUID:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset	8
	.cfi_offset	5,	-8
	movl	%esp,	%ebp
	.cfi_def_cfa_register	5
	pushfl
	pop	%eax
	mov	%eax,	%ecx
	xor	$0x200000,	%eax
	push	%eax
	popfl
	pushfl
	pop	%eax
	xor	%ecx,	%eax
	shr	$21,	%eax
	and	$1,	%eax
	push	%ecx
	popfl
	popl	%ebp
	.cfi_restore	5
	.cfi_def_cfa	4,	4
	ret
	.cfi_endproc
