/*
    Emirhan Aktürk
    19120205058
    $ ls -1 | cut -d'.' -f1 | sort | uniq | tee dirents
    yukarıdaki terminal komutlarını C programında exec ve pipe sistem çağrısıyla gerçekleştiren bir C programı 
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    pid_t pid;
    int fd[2];
    if(pipe(fd)==-1)//ls ve cut arasındaki pipe
        perror("pipe");

    pid=fork();
    if(pid<0)
        perror("fork");

    if(pid==0){//child process
        close(fd[0]);
        dup2(fd[1],1);
        execlp("ls","ls","-1",(char*)NULL);
    }
    else{//parent process
        int fd2[2];
        close(fd[1]);
        if(pipe(fd2)==-1)//cut ve sort arasındaki pipe
            perror("pipe");

        pid=fork();
        if(pid<0)
            perror("fork");

        if(pid==0){//child process
            dup2(fd[0],0);
            dup2(fd2[1],1);
            execlp("cut","cut","-f1","-d.",(char*)NULL);
        }
        else{//parent process
            int fd3[2];
            close(fd2[1]);
            
            if(pipe(fd3)==-1)//sort ve uniq arasındaki pipe
                perror("pipe");
            
            pid=fork();
            if(pid<0)
              perror("fork");

            if(pid==0){//child process
                dup2(fd2[0],0);
                dup2(fd3[1],1);
                execlp("sort","sort",(char*)NULL);
            }
            else{//parent process
                int fd4[2];
                close(fd3[1]);
            
                if(pipe(fd4)==-1)//uniq ve tee arasındaki pipe
                    perror("pipe");
                      
                pid=fork();
                if(pid<0)
                    perror("fork");

                if(pid==0){//child process
                    dup2(fd3[0],0);
                    dup2(fd4[1],1);
                    execlp("uniq","uniq",(char*)NULL);
                }
                else{//parent process
                    close(fd4[1]);
                    dup2(fd4[0],0);
                    execlp("tee","tee","dirents",(char*)NULL);
                }
            }
        }
    }
    return 0;
}