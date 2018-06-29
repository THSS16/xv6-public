#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
    int num=nfpgs();
    printf(1, "%d", num);
    //bug:每次稳步减少25页...
    //todo:共享内存使用情况、进程队列情况、空闲内存情况(即nfpgs)、已使用内存情况等
    exit();
}