global ft_strcpy
ft_strcpy:	; params: destination, source
	mov rax, rdi
	mov rdx, rsi
	mov rcx, 0

	.loop:
		movzx rdx, byte [rsi + rcx]
		mov byte [rax + rcx], dl
		cmp byte [rsi + rcx], 0
		je .break
		inc rcx
		jmp .loop

	.break:
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
