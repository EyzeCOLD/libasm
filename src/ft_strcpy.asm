global ft_strcpy
ft_strcpy:	; params: destination, source
	mov rax, rdi	; rax = dst
	mov rdx, rsi	; rdx = src
	mov rcx, 0		; rcx = 0

	.loop:
		movzx rdx, byte [rsi + rcx] ; rdx = src[rcx]
		mov byte [rax + rcx], dl	; dst[rcx] = dl (rdx)
		cmp byte [rsi + rcx], 0		; if src[rcx] == '\0'
		je .break					; break
		inc rcx						; rcx++
		jmp .loop

	.break:
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
