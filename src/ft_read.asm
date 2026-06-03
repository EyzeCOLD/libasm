extern __errno_location

section .text
global ft_read
ft_read:	; params: fd, buf, count
	mov rax, 0	; sys_write
	syscall
	cmp rax, -4095
	jae .error
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
