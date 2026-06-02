global ft_strcmp
ft_strcmp:	; params: s1, s2
	xor rax, rax
	xor rcx, rcx

	.loop:
		movzx rax, byte [rdi + rcx]
		movzx rdx, byte [rsi + rcx]
		sub rax, rdx
		jnz .break
		cmp byte [rdi + rcx], 0
		je .break
		inc rcx
		jmp .loop

	.break:
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
