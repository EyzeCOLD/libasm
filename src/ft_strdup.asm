%include "src/libasm.inc"

section .rodata
; prot flags
PROT_READ	equ 00000001b
PROT_WRITE	equ 00000010b
; map flags
MAP_ANON	equ 00100000b
MAP_PRIVATE	equ 00000010b

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
