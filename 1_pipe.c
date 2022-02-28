/*  28 Feb 2022 PIPE
    to implement pipe in linux

    #include <unistd.h>
    int pipe(int pipefd[2]);

    ---- pipe - overview of pipes and FIFOs -------
    Pipes and FIFOs (also known as named pipes) provide a
       unidirectional interprocess communication channel.  A pipe has a
       read end and a write end.  Data written to the write end of a
       pipe can be read from the read end of the pipe.

       A pipe is created using pipe(), which creates a new pipe and
       returns two file descriptors, one referring to the read end of
       the pipe, the other referring to the write end.  Pipes can be
       used to create a communication channel between related processes;
    
    fifo - first-in first-out special file, named pipe

       A FIFO (short for First In First Out) has a name within the
       filesystem (created using mkfifo()), and is opened using
       open().  Any process may open a FIFO, assuming the file
       permissions allow it.  The read end is opened using the O_RDONLY
       flag; the write end is opened using the O_WRONLY flag.

    int mkfifo(const char *pathname, mode_t mode);
    // FIFO file path
    char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);

    int fd1 = open(myfifo,O_RDONLY);
    read(fd1, str1, 80);

    ------ creating a pipe using pipe() and pipe2() -----------
    pipe() creates a pipe, a unidirectional data channel that can be
       used for interprocess communication.  The array pipefd is used to
       return two file descriptors referring to the ends of the pipe.
       pipefd[0] refers to the read end of the pipe.  pipefd[1] refers
       to the write end of the pipe.  Data written to the write end of
       the pipe is buffered by the kernel until it is read from the read
       end of the pipe. 
    
    On success, zero is returned.  On error, -1 is returned, errno is
    set to indicate the error, and pipefd is left unchanged.


    fd[1] -> write end
    fd[0] -> read end
    Usually for related processes like parent child
    After fork() the child process inherits the same file descriptors.
    It looks like this.
     ________________
    | parent proces  |
   _|                |__
  | | fd[1]     fd[0]|  |
  | |________________|  |
  |     __________      |
  ---->()__pipe___)----->
  |  ________________   |
  |_|fd[1]      fd[0]|__|
    |                | 
    | child process  |
    |________________|

    Close the unused end.
    Ex. to pass the data from parent to child process
    close the read end of the pipe for the parent process
    And close the write end of the pipe for the child process


   Pipe is unidirectional
   Pipe is like a buffer (limit is 64k bytes)
   We can't write more bytes to the pipe if it is full.
   First we need to read the data then we can write more.

   Pipes behave FIFO(First in First out), 
   Pipe behave like a queue data structure. 
   Size of read and write don’t have to match here. 
   We can write 512 bytes at a time but we can read only 1 byte at a time in a pipe.


   #include <unistd.h>
    write - write to a file descriptor
    ssize_t write(int fd, const void *buf, size_t count);
    On success, the number of bytes written is returned.  On error,
    -1 is returned, and errno is set to indicate the error.

    read - read from a file descriptor
    ssize_t read(int fd, void *buf, size_t count);

*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main()
{
    int fd[2]; //  file descriptors   fd[0] ---> reading end  fd[1] --> writing end
    char buff[50];
    char data[50] = {};
    int ret = 0;

    //open a pipe
    if(pipe(fd) == -1)
    {
        perror("pipe");// error in pipe
        exit(1); // exit from the program
    }
    sprintf(buff, "PIPE data flow demo:");

    
    // writing to pipe
    ret = write(fd[1], buff, strlen(buff));
    if(ret !=-1)
        printf("%d bytes written\n", ret);
    printf("\n");

    // reading to pipe, and storing data in data buffer
    ret = read(fd[0], data , 5); // read 5 bytes
    assert(ret == 5);
    printf("%d bytes read\n", ret);
    printf("%s\n", data);

    ret = read(fd[0], data , 5);
    assert(ret == 5);
    printf("%d bytes read\n", ret);
    printf("%s\n", data);

    ret = read(fd[0], data , 10); // read 10 bytes
    assert(ret == 10);
    printf("%d bytes read\n", ret);
    printf("%s\n", data);

    return 0;
}

