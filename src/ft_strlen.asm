global ft_strlen
ft_strlen:	; params: string
	mov rax, 0
	.loop:
		cmp byte [rdi + rax], 0
		je .break
		inc rax
		jmp .loop
	.break:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
