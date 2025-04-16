#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid;
    
    printf("Início\n");

    pid = fork();

    if (pid < 0){
        fprintf(stderr, "Fork falhou");
        return 1;
    }else if (pid == 0){
        printf("Pai em execução\n");
        execlp("/bin/ls", "ls", NULL);
    }else{
        printf("Filho em execução\n");
        wait(NULL);
        printf("Filho completou!\n");
    }
    printf("Pai Finalizou\n");
    return 0;
}