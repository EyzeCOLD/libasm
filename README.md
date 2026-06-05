# libasm
In this project I've implemented 6 simple C standard library functions in x86-64 assembly. The code is written to be assembled with [NASM](https://www.nasm.us/), in the Intel syntax and conforms to the [System V ABI](https://wiki.osdev.org/System_V_ABI) calling convention.

This README.md documents what I've learned during the project.
## Table of Contents
- [Registers](#registers)
- [Instructions](#instructions)
  - [mov](#mov)
  - [add, sub](#add-sub)
  - [cmd, test](#cmp-test)
  - [Jumps](#jumps)
  - [Labels](#labels)
  - [ret](#ret)
  - [push, pop](#push-pop)
  - [neg](#neg)
  - [call, syscall](#call-syscall)
- [Directives](#directives)
- [Sections](#sections)
- [Other Concepts](#other-concepts)
## Registers
If you're familiar with programming, you'll be very used to variables.  In assembly, instead of creating your own variables, you mostly just use registers to store data.

In x86-64 architecture each of the registers can hold 64 bits of data. The registers also have smaller "sub-registers" for 32, 16 and 8 bits.
```
| ~~~~~~~~~~~~~~~~ RAX (8 bytes) ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ |
| ................................. | ~~~~~~~~ EAX (4 bytes) ~~~~~~~~~~ |
| ................................................... | AX (2 bytes) ~~ |
| ................................................... | ~ AH ~ | ~ AL ~ |
```
So the 4 bytes of EAX are actually the same as the last 4 bytes of RAX and the first byte of the AX register is the same as the byte in the AH register. The sizes of the registers match up to the C datatypes long, int, short and char.

## Instructions
Instructions are what you use to manipulate the registers.
#### `mov`
```nasm
mov <destination>, <source>
```
`mov` copies the data from source to destination. The source can be a value or a memory location (register or memory address). The destination has to be a memory location. It is quite analogous to variable assignment in higher level languages, if you compare `mov rax, 42` to `rax = 42`.

You can also use indirection. Let's say `rax` contains the address of a variable, you can access it with `mov rcx, [rax]` meaning "assign the value _pointed by_ rax into rcx".

#### `movzx`
Like many of the assembly instructions, `mov` has many variants like `movzx` (zero expand). When you do `movzx rax, byte [rcx]` you only copy one byte from `rcx` but also zero out the upper bytes in `rax`, to ensure a proper overwrite.

⚠️ Note
>Most of the time NASM can infer the size of the operation when you do a `mov`, but with indirection it can become ambiguous. In those situations you should use an operation size specifier, e.g. `mov byte [rax]. The size specifiers are `byte`, `word` (2 bytes), `dword` (4 bytes) and `qword` (8 bytes).

⚠️ Note
> You can't have both operands of `mov` be dereferenced pointers. If you need to `mov [rax], [rcx]` you have to first store the value pointer by rcx into an intermediary register and then copy it from there to `rax`'s address
#### `add`, `sub`
```nasm
add <register>, <value>
sub <register>, <value>
```
`add` and `sub` take the value in our register and modify it by adding or substracting the second operands value from it. They is analogous to `n += 5` or `x -= y` in C. The big difference is that these operations can also set a number of flags in the processor. For example if the result of the operation would be zero, the Zero Flag (ZF) is set. The flags can then be used to make conditional control flow with jumps.
#### `cmp`, `test`
```nasm
cmp <value>, <value>
test <value>, <value>
```
`cmp` does a subtraction on the two operands, setting flags, but throwing away the result. `test` is similar, but instead of subtraction, it does a bitwise AND to the two operands.
#### Jumps
```nasm
jmp <label>
jz <label>
jne <label>
```
The main control flow of assembly programs is done with the jump instructions. `jmp <label>` will find the matching label and the execution of the code goes on from there. Even more useful are the conditional jumps. Mathematical instructions like `add`, `sub` and `cmp` can set off flags, the conditional jumps check those flags and go to the label depending on the condition.
#### Labels
Jumps are paired with labels.
```nasm
test rax, rax ; check if rax is zero
jz .error     ; if zero jump
...
.error:       ; local label
    mov rax, -1
    ret
```
NASM allows creation of local labels via the dot prefix. This way you can reuse the same descriptive names in multiple functions without creating a collision.

⚠️ Note
Functions are just global labels and `call` is just a `jmp` that pushes the return address on the stack for `ret`, so you can carry on after the function call.
#### `ret`
```nasm
ret
```
`ret` returns from the function. By convention the contents of rax are treated as the return value. Execution picks up from where the function was called.
#### `push`, `pop`
```nasm
push <value>
pop <memory location>
```
Push and pop allow you to put things into the stack and pull it back out. It is very useful when doing function calls, since most functions will mess up your registers while they do their thing.
```nasm
push rax   ; push the value of rax into the stack
sub rsp, 8 ; align stack
...
call func  ; the function call will overwrite rax
add rsp, 8 ; align stack
pop rax    ; so we pop the topmost item on the stack and store it in rax
```
Pushing is means writing to a memory address in RAM pointed by `rsp` (the stack pointer) after moving the pointer 8 bytes down the address space.
```nasm
push rax
...
pop rax
; is equivalent to
sub rsp, 8
mov [rsp], rax
...
mov rax, [rsp]
add rsp, 8
```

⚠️ Note
>If we call a C function, we're expected to keep the stack 16-byte aligned. Every push subtracts  `rsp` by 8, so in the above example if we were calling a function that expected 16-byte alignment, we'd have to make a dummy push or explicitly adjust the stack pointer. Same goes for the pops.

⚠️ Note
>Every time you push something to the stack, you're also expected to pop it. Not keeping pushes and pops in balance leads to stack corruption and the world famous stack overflow.

#### `neg`
```nasm
neg <memory location>
```
Flips the sign of the value held by the register.
#### `call`, `syscall`
```nasm
syscall
call <function>
```
Whereas `call` will invoke the named function, `syscall` will check `rax` and then invoke the corresponding system call from the OS system call table (e.g. 1 for `sys_write`, 9 for `sys_mmap`). 

The arguments to the functions are supplied from the registers `rdi`, `rsi`, `rdx`, `rcx`, `r8` and `r9`.  Same for system calls, except `rcx` is replaced by `r10`. These are all conventions which you must follow if you want your code to work with anything else out there.

Also very important thing is the `wrt` operator (With Reference To) that makes your external function calls work position independently. More on that below.

## Directives
Directives are just commands for the assembler that don't become machine code per se.
#### `extern`
Tells the assembler that the symbol is found in some other file and will be handled by the linker later.
#### `global`
Exposes the symbol to other files. Kind of like export to externs import.
#### `equ`
Let's you define constants.
#### `%include`
Analogous to C's #include prerocessor directive. Take the contents of another file and paste it here.

## Sections
Assembly source files are broken into sections. The sections server different roles. By default NASM treats the file as a `section .text`. 
#### `.text`
Most of the action lives here as it's the only executable section
#### `.data`, `.bss`
These sections are for declaring variables. You might not want to use these a lot, since they are basically analogous to global variables. `.data` is for initialized variables where `.bss` is for zero-initialized variables (could be used as buffers).
#### `.rodata`
This section is for read-only data, in other words constants.

| Section   | Contents                           | Writable | Executable |
| --------- | ---------------------------------- | -------- | ---------- |
| `.text`   | Code                               | No       | Yes        |
| `.data`   | Initialized variables              | Yes      | No         |
| `.bss`    | Zero-initialized variables         | Yes      | No         |
| `.rodata` | Read-only constants (strings etc.) | No       | No         |

## Other concepts
#### Address Space Layout Randomization (ASLR)
For security reasons modern operating systems make it impossible for an attacker to predict the exact memory layout of a program, as it is purposefully scrambled. That is why code must be Position-Independent, meaning that it uses relative addresses rather than absolute addresses. The north pole for the addresses is the instruction pointer (`rip`).
#### Position-independent Executable (PIE)
An executable not relying on absolute addresses. This is the expectation these days for the reasons stated above. It makes things like external function calls a bit tricky, since you might only know the address during runtime.
#### Procedure Linkage Table
When calling functions from shared libraries (`.so` files, like libc) you have to use the operator `wrt ..plt` (With Reference To ..Procedure Linkage Table) that allows you to resolve calls to dynamically linked libraries at runtime.
When calling functions from shared libraries (`.so` files, like libc) you have to use the operator `wrt ..plt` (With Reference To ..Procedure Linkage Table) that allows you to resolve calls to dynamically linked libraries at runtime.
