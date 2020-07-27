#ifndef _OEL_SYSCALL_H
#define _OEL_SYSCALL_H

#include <sys/syscall.h>

#define OEL_SYS_trace 1000
#define OEL_SYS_trace_ptr 1001
#define OEL_SYS_dump_stack 1002
#define OEL_SYS_dump_ehdr 1003

long oel_syscall(long n, long params[6]);

const char* syscall_str(long n);

#endif /* _OEL_SYSCALL_H */
