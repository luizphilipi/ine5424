# 1 "ia32_crt0.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "ia32_crt0.S"
 .file "ia32_crt0.S"

 .section .init
 .align 4
 .globl _start
 .type _start,@function
_start:
 call _init
 .align 4
 .globl __epos_library_app_entry
        .type __epos_library_app_entry,@function
__epos_library_app_entry:
 call main
 push %eax
 call _fini
 call _exit
