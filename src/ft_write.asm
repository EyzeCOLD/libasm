%include "src/libasm.inc"

extern __errno_location

section .text
global ft_write
ft_write:	; params: fd, buffer, length
	mov rax, SYS_WRITE
	syscall
	cmp rax, -4096	; -1 - -4095 reserved for errors
	ja .error		; treating return as unsigned
	ret

	.error:
		neg rax
		push rax						; rax on stack
		call __errno_location wrt ..plt	; rax = &errno, indirect call via PLT
		pop rcx							; rcx = old rax (from stack)
		mov [rax], rcx					; write to errno
		mov rax, -1
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
