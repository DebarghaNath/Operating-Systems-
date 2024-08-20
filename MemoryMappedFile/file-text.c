#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
    int fd = open("ABC.txt", O_RDWR);

    // Get file information
    struct stat sb;
    //get the stats of the file with file descripter fd
    fstat(fd,&sb);
    
    // Memory-map the file
    char *block_ptr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    for(int i=0;i<565;i++)
    {
        printf("%c",block_ptr[i]);
    }
    printf("\n");
    // Unmap the file when done
    munmap(block_ptr, sb.st_size);
    // Close the file descriptor
    close(fd);
    
    return 0;
}
