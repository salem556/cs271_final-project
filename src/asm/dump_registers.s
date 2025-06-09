    .extern _debug_dump_registers
    .globl  dump_registers
    .type   dump_registers, @function
dump_registers:
    /* allocate space for 16 longs on the stack */
    subq    $(16*8), %rsp

    /* spill registers into that buffer at RSP+offset */
    movq    %rax, 0(%rsp)      /* regs[0] = rax */
    movq    %rbx, 8(%rsp)      /* regs[1] = rbx */
    movq    %rcx,16(%rsp)      /* regs[2] = rcx */
    movq    %rdx,24(%rsp)      /* regs[3] = rdx */
    movq    %rsi,32(%rsp)      /* regs[4] = rsi */
    movq    %rdi,40(%rsp)      /* regs[5] = rdi */
    movq    %rbp,48(%rsp)      /* regs[6] = rbp */
    /* regs[7] = rsp will be computed in C */

    movq    %r8,  64(%rsp)     /* regs[8]  = r8  */
    movq    %r9,  72(%rsp)     /* regs[9]  = r9  */
    movq    %r10, 80(%rsp)     /* regs[10] = r10 */
    movq    %r11, 88(%rsp)     /* regs[11] = r11 */
    movq    %r12, 96(%rsp)     /* regs[12] = r12 */
    movq    %r13,104(%rsp)     /* regs[13] = r13 */
    movq    %r14,112(%rsp)     /* regs[14] = r14 */
    movq    %r15,120(%rsp)     /* regs[15] = r15 */

    /* pass pointer to that buffer as first arg */
    movq    %rsp, %rdi
    call    _debug_dump_registers

    /* tear down stack frame */
    addq    $(16*8), %rsp
    ret
