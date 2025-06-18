#define _GNU_SOURCE
#include <seccomp.h>

__attribute__((visibility ("hidden"))) void forbidden() {
    write(1, "[-] Security check initialization failed :(\n", 40);
    _exit(1);
}

void nice() {
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (!ctx) {
        forbidden();
    }

    // Block execution-related syscalls
    if (seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0) ||
        seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execveat), 0)) {
        forbidden();
    }

    if (seccomp_load(ctx)) {
        forbidden();
    }

    seccomp_release(ctx);
    write(1, "[*] Security check initialized\n", 31);
}
