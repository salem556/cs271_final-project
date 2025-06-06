.globl _dump_backtrace
.globl _debug_backtrace_helper

_dump_backtrace:
    jmp _debug_backtrace_helper
