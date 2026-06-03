extern __errno_location

section .text
global ft_write
ft_write:	; params: fd, buffer, length
	mov rax, 1		; sys_write
	syscall
	cmp rax, -4095	; -1 - -4095 is the conventional range for error returns
	jae .error		; we treat the return as unsigned so the errors are at the
					; tippity-top of the range
	jmp .return

	.error:
		neg rax
		push rax						; rax on stack
		call __errno_location wrt ..plt	; rax = &errno, indirect call via PLT
		pop rcx							; rcx = old rax (from stack)
		mov [rax], rcx					; write to errno
		mov rax, -1

	.return:
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
