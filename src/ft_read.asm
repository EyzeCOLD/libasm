%include "src/libasm.inc"

extern __errno_location

section .text
global ft_read
ft_read:	; params: fd, buf, count
	mov rax, SYS_READ
	syscall
	cmp rax, -4096
	ja .error
	jmp .return

	.error:
		neg rax
		push rax
		call __errno_location wrt ..plt
		pop rcx
		mov [rax], rcx
		mov rax, -1

	.return:
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
