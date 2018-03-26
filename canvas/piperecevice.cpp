#include "piperecevice.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <canvasscene.h>
#include <QDebug>
#include <canvasview.h>

extern QReadWriteLock pipeLock;

pipeRecevice * m_pipeRecevice = 0;

nodePosition * position;

pipeRecevice::pipeRecevice()
{

}

void
pipeRecevice::recevicePosition()
{
    const char *fifo_name = "/tmp/my_fifo";
        int pipe_fd = -1;
        int res = 0;
        int open_mode = O_RDONLY;
        double buffer[4];
        //清空缓冲数组
        memset(buffer, '\0', sizeof(buffer));

//        printf("Process %d opening FIFO O_RDONLY\n", getpid());
        //以只读阻塞方式打开管道文件，注意与fifowrite.c文件中的FIFO同名
        pipe_fd = open(fifo_name, open_mode);

        if(pipe_fd != -1)
        {
            do
            {
                if(flag == 0)
                {
                //读取FIFO中的数据
                res = read(pipe_fd, buffer, 32);
//                pipeLock.lockForWrite();
                position.nodeId=buffer[1];
                position.time=buffer[0];
                position.x=buffer[2];
                position.y=buffer[3];
                flag = 1;
                }

            }while(res > 0);
            close(pipe_fd);
        }
}


nodePosition
pipeRecevice::getPosition()
{

    return position;
}

pipeRecevice *
pipeRecevice::getInstance()
{
    if(!m_pipeRecevice)
    {
        m_pipeRecevice = new pipeRecevice;
    }
    return m_pipeRecevice;
}

int
pipeRecevice::getflag()
{
    return flag;
}

void
pipeRecevice::setFlagZero()
{
    flag = 0;
}
