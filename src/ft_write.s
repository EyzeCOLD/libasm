%include "src/libasm.inc"

extern __errno_location

section .text
global ft_write
ft_write:	; params: fd, buffer, length
	mov rax, SYS_WRITE
	syscall
	cmp rax, -4096	; -1 - -4095 reserved for errors
	ja .error		; treating return as unsigned
	ret

	.error:
		neg rax
		push rax						; rax on stack
		call __errno_location wrt ..plt	; rax = &errno
		; wrt ..plt = With Reference To Procedure Linkage Table
		; Basically, errno lives in dynamically linked libc, so the address
		; won't be known until runtime.
		; This has to do with Address Space Layout Randomization (ASLR),
		; a security feature where things are loaded to randomized
		; addresses every run. ASLR requires making our executable to be 
		; a Position Independent Executable (PIE). So we can't use
		; hardcoded addresses
		pop rcx							; rcx = old rax (from stack)
		mov [rax], rcx					; write to errno
		mov rax, -1
		ret

section .note.GNU-stack noalloc noexec nowrite progbits
