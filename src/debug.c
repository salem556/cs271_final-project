#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>     // for Dl_info and dladdr()
#include <libgen.h>    // for basename()
#include <string.h>    // for strcmp()
#include "debug.h"

/* pull in the format string from dump_backtrace.s */
extern const char backtrace_format_str[];

/*
 * Called from dump_registers.s:
 *   regs -> an array[16] of longs spilled on the stack.
 *   We reconstruct the original RSP as regs + 16*sizeof(long).
 */
void _debug_dump_registers(long const *regs) {
    static const char *regnames[16] = {
        "rax","rbx","rcx","rdx",
        "rsi","rdi","rbp","rsp",
        "r8", "r9", "r10","r11",
        "r12","r13","r14","r15"
    };

    unsigned long orig_rsp = (unsigned long)regs + 16 * sizeof(long);

    for (int i = 0; i < 16; i++) {
        long val = (i == 7
                    ? (long)orig_rsp
                    : regs[i]);
        printf("%s\t%ld (0x%lx)\n",
               regnames[i],
               val,
               (unsigned long)val);
    }
}

/*
 * Called from dump_backtrace.s. Walk the frame-pointer chain,
 * stopping at “_start”, after 64 frames, or if the chain ever goes
 * non-monotonically “up” the stack.
 */
void _debug_dump_backtrace(void) {
    void **rbp;
    __asm__ volatile("movq %%rbp, %0" : "=r"(rbp));

    long depth      = 0;
    void **last_rbp = rbp;

    while (rbp && *rbp && depth < 64) {
        void *ret_addr = *(rbp + 1);
        Dl_info info;
        const char *sym  = "??";
        const char *file = "??";

        if (dladdr(ret_addr, &info)) {
            if (info.dli_sname) sym  = info.dli_sname;
            if (info.dli_fname) file = basename((char*)info.dli_fname);
        }

        printf(backtrace_format_str,
               depth,
               (unsigned long)ret_addr,
               sym,
               file);

        /* stop once we hit the real entry point */
        if (info.dli_sname && strcmp(info.dli_sname, "_start") == 0) {
            break;
        }

        /* get the next frame and verify it’s strictly deeper */
        void **next_rbp = (void **)*rbp;
        if (!next_rbp || next_rbp <= last_rbp) {
            break;
        }
        last_rbp = next_rbp;
        rbp      = next_rbp;
        depth++;
    }
}
