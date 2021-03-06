#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int pfds[2]; //arreglo de lectura y escritura
    char buf[30];

    pipe(pfds);

    if (!fork()) {
        printf(" CHILD: writing to the pipe\n");
        write(pfds[1], "test", 5); //escribe en fd[1] -> (donde_escribe, texto, num_caracteres)
        printf(" CHILD: exiting\n");
        exit(0);
    } else {
        printf("PARENT: reading from pipe\n");
        read(pfds[0], buf, 5); //lee en fd[0]
        printf("PARENT: read \"%s\"\n", buf);
        wait(NULL);
    }

    return 0;
}
