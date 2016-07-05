	.text
	.file	"../src/Test/test.cpp"
	.globl	not_null
	.align	16, 0x90
	.type	not_null,@function
not_null:                               # @not_null
	.cfi_startproc
# BB#0:
	cmpl	$100, (%rdi)
	setg	%al
	retq
.Lfunc_end0:
	.size	not_null, .Lfunc_end0-not_null
	.cfi_endproc

	.globl	raw
	.align	16, 0x90
	.type	raw,@function
raw:                                    # @raw
	.cfi_startproc
# BB#0:
	testq	%rdi, %rdi
	je	.LBB1_1
# BB#2:
	cmpl	$100, (%rdi)
	setg	%al
	retq
.LBB1_1:
	xorl	%eax, %eax
	retq
.Lfunc_end1:
	.size	raw, .Lfunc_end1-raw
	.cfi_endproc

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	xorl	%eax, %eax
	retq
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
	.cfi_endproc


	.ident	"clang version 3.8.0 (tags/RELEASE_380/final)"
	.section	".note.GNU-stack","",@progbits
