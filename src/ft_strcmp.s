global ft_strcmp
ft_strcmp:	; params: s1, s2
	xor rax, rax
	mov rcx, -1

	.loop:
		inc rcx
		movzx rax, byte [rdi + rcx]
		movzx rdx, byte [rsi + rcx]
		sub rax, rdx
		jnz .end
		cmp byte [rdi + rcx], 0
		jne .loop

	.end:
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
