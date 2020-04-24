
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        imul rdx, rsi ; rdx = rdx * rsi (row = row*row_width)
        add rdx, rcx ; rdx represents index. rdx = rdx + rcx (row = row + column)
        mov rbx, rdx ; rbx = rdx, rbx is assigned the index value
        shr rbx, 3 ; rbx represents Byte offset. rbx is shifted right 3 (rbx/8)
        mov rsi, rbx ; rsi = rbx, rsi is assigned the Byte offset
        mov rcx, rdx ; rcx = rdx, rcx is assigned the index value
        shl rsi, 3 ; the Byte offset is shifted left 3 (rsi*8)
        sub rcx, rsi ; rcx represents Bit offset. rcx = rcx - rsi (index = index - (ByteOffset*8))
        mov rax, 7 ; assigns 7 to the rax register
        mov rsi, 1 ; assigns 1 to the rsi register
        sub rax, rcx ; rax = rax - rcx. This gets the proper position of the bit
        mov cl, al ; moves the position to rcx register
        shl rsi, cl ; rsi represents the mask. rsi is shifted left by the byteOffset value to the correct position

        ;The code from lecture did not work for he BitArray2DSetGet tests so the array access was moved to the 'or'
        mov rax, rdi ; moves the pointer of the char array into rax
        or [rax], rsi ; compares rax to the mask and then returns the result of the 'or' compare

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax



get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here - for now returning 0
        ; mov rax, 0

        imul rdx, rsi ; rdx = rdx * rsi (row = row*row_width)
        add rdx, rcx ; rdx represents index. rdx = rdx + rcx (row = row + column)
        mov rbx, rdx ; rbx = rdx, rbx is assigned the index value
        shr rbx, 3 ; rbx represents Byte offset. rbx is shifted right 3 (rbx/8)
        mov rsi, rbx ; rsi = rbx, rsi is assigned the Byte offset
        mov rcx, rdx ; rcx = rdx, rcx is assigned the index value
        shl rsi, 3 ; the Byte offset is shifted left 3 (rsi*8)
        sub rcx, rsi ; rcx represents Bit offset. rcx = rcx - rsi (index = index - (ByteOffset*8))
        mov rax, 7 ; assigns 7 to the rax register
        mov rsi, 1 ; assigns 1 to the rsi register
        sub rax, rcx ; rax = rax - rcx. This gets the proper position of the bit
        mov cl, al ; moves the position to rcx register
        shl rsi, cl ; rsi represents the mask. rsi is shifted left by the byteOffset value to the correct position

        ;This code is from the bit filtering lecture
        mov rax, [rdi] ; move the contents of the array pointer in rax
        and rax, rsi
        cmp rax, 0
        setg al
        movsx rax, al

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
