#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "jobsconst.h"

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

int main(void)
{
    int fd;
    
    //测试代码
    /*fd = open("processInfo", O_CREATE );
    close(fd);
    fd = open("processInfo", O_WRONLY);
    if(fd >= 0) 
    {
        printf(1, "ok: open file succeed\n");
    } 
    else 
    {
        printf(1, "error: open file failed\n");
        exit();
    }

    char str[] = "1 test 1\n2 blabla 2\n";
    if(write(fd, str, sizeof(str)) != sizeof(str))
        printf(1,"err");
    else
        printf(1,str);
    
    close(fd);*/

    fd = open(FILENAME, O_RDONLY);
    if(fd >= 0) 
    {
        printf(1, "ok: open file succeed\n");
    } 
    else 
    {
        printf(1, "error: open file failed\n");
        exit();
    }
    char* line = " ";
    int id = 1;
    int size;
    while(1)//读入每行
    {
        size = readline(fd, line, 100);
        if(size > 100 || size < 0)
            break;
        char res[20];
        int pos = 0;
        pos = partition(line, res, pos);
        {
            printf(1,"%d",id);//输出编号
            printf(1, " ");

            pos = partition(line, res, pos);
            printf(1, res);//输出名称
            printf(1, " ");

            pos = partition(line, res, pos);
            int pid = atoi(res);
            
            int state = getstate(pid);

            switch(state)
            {
                case UNUSED: printf(1,"UNUSED\n"); break;
                case EMBRYO: printf(1,"EMBRYO\n"); break;
                case SLEEPING: printf(1,"SLEEPING\n"); break;
                case RUNNABLE: printf(1,"RUNNABLE\n"); break;
                case ZOMBIE: printf(1,"ZOMBIE\n"); break;
                default: printf(1,"ERR STATE\n"); break;
            }
            id++;
        }
    }
    //printf(1, "read all\n");
    close(fd);
    return exit();
}
