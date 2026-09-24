#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main()
{
    // getpid() is a thin libc wrapper - it eventually does exactly what
    // syscall(SYS_getpid) does below, but through a stable, portable
    // function name instead of a numbered syscall.
    pid_t viaLibc = getpid();

    // syscall() invokes a kernel system call directly by number, bypassing
    // any libc wrapper - useful when libc doesn't expose a wrapper for a
    // given syscall, but numbers aren't portable across architectures/OSes,
    // which is exactly why libc wrappers exist.
    long viaRawSyscall = syscall(SYS_getpid);

    printf("getpid() = %d\n", viaLibc);
    printf("syscall(SYS_getpid) = %ld\n", viaRawSyscall);
    printf("SYS_getpid number on this system = %d\n", SYS_getpid);

    return 0;
}
