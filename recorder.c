#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int fd = open("record", 1);
    if (fd < 3) {
        printf(1, "Open file error\n");
        exit();
    }

    char buffer[1];
    int rt;
    while ((rt = read(1, buffer, 1)) >= 0) {
        if(rt == 1){
            if(write(fd, buffer, 1) != 1){
                printf(1,"Write error\n");
                break;
            }
        }
    }
    exit();
}