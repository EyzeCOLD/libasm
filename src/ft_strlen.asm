global ft_strlen
ft_strlen:	; params: string
	mov rax, -1
	.loop:
		inc rax
		cmp byte [rdi + rax], 0
		jne .loop
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
