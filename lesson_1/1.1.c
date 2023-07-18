#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    uid_t uid = getuid();
    uid_t euid = geteuid();
    gid_t gid = getgid();
    pid_t sid = getsid(pid);

    printf("%d", pid);
    printf("%d", ppid);
    printf("%d", uid);
    printf("%d", euid);
    printf("%d", gid);
    printf("%d", sid);

    return 0;
}