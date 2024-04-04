	.file	"add.c"
# GNU C17 (Ubuntu 11.4.0-1ubuntu1~22.04) version 11.4.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.4.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.section	.rodata
.LC0:
	.string	"%f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$32, %rsp	#,
	movl	%edi, -20(%rbp)	# argc, argc
	movq	%rsi, -32(%rbp)	# argv, argv
# add.c:6:     float a = atof(argv[1]);
	movq	-32(%rbp), %rax	# argv, tmp91
	addq	$8, %rax	#, _1
# add.c:6:     float a = atof(argv[1]);
	movq	(%rax), %rax	# *_1, _2
	movq	%rax, %rdi	# _2,
	call	atof@PLT	#
# add.c:6:     float a = atof(argv[1]);
	cvtsd2ss	%xmm0, %xmm0	# _3, tmp93
	movss	%xmm0, -12(%rbp)	# tmp93, a
# add.c:7:     float b = atof(argv[2]);
	movq	-32(%rbp), %rax	# argv, tmp94
	addq	$16, %rax	#, _4
# add.c:7:     float b = atof(argv[2]);
	movq	(%rax), %rax	# *_4, _5
	movq	%rax, %rdi	# _5,
	call	atof@PLT	#
# add.c:7:     float b = atof(argv[2]);
	cvtsd2ss	%xmm0, %xmm0	# _6, tmp96
	movss	%xmm0, -8(%rbp)	# tmp96, b
# add.c:8:     float c = a + b;
	movss	-12(%rbp), %xmm0	# a, tmp98
	addss	-8(%rbp), %xmm0	# b, tmp97
	movss	%xmm0, -4(%rbp)	# tmp97, c
# add.c:9:     printf("%f\n", c);
	pxor	%xmm1, %xmm1	# _7
	cvtss2sd	-4(%rbp), %xmm1	# c, _7
	movq	%xmm1, %rax	# _7, _7
	movq	%rax, %xmm0	# _7,
	leaq	.LC0(%rip), %rax	#, tmp99
	movq	%rax, %rdi	# tmp99,
	movl	$1, %eax	#,
	call	printf@PLT	#
# add.c:10:     return 0;
	movl	$0, %eax	#, _14
# add.c:11: }
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
