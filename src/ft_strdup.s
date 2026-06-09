%include "src/libasm.inc"

extern malloc
extern ft_strlen
extern ft_strcpy

section .text
global ft_strdup
ft_strdup:	; params: string
	push rdi		; original string address on the stack
	sub rsp, 8		; 16-bit stack alignment
	call ft_strlen

	mov rdi, rax
	inc rdi
	call malloc wrt ..plt
	test rax, rax
	add rsp, 8
	pop rsi
	jz .error

	mov rdi, rax
	call ft_strcpy
	ret

	.error:
		xor rax, rax
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
