global ft_strcpy
ft_strcpy:	; params: destination, source
	mov rax, rdi	; rax = dst
	mov rdx, rsi	; rdx = src
	mov rcx, -1		; rcx = 0

	.loop:
		inc rcx						; rcx++
		movzx rdx, byte [rsi + rcx] ; rdx = src[rcx]
		mov byte [rax + rcx], dl	; dst[rcx] = dl (rdx)
		cmp byte [rsi + rcx], 0		; if src[rcx] == '\0'
		jne .loop

	ret

section .note.GNU-stack noalloc noexec nowrite progbits
