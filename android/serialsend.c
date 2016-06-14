/* rs232_tx.c */
#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#define BAUDRATE        B19200
#define MODEMDEVICE     "/dev/ttyHS0"
#define _POSIX_SOURCE   1

#define STOP    '@'

int main(int argc, char *argv[])
{

    int    fd, c=0, res;
    struct termios  oldtio, newtio;

    char  ch;
    static char   s1[20];

    printf("Start...\n");
    fd = open(MODEMDEVICE, O_RDWR|O_NOCTTY);

    if (fd < 0)
    {
        perror(MODEMDEVICE);
        exit(1);
    }

    printf("Open...\n");
    tcgetattr(fd, &oldtio);
    bzero(&newtio, sizeof(newtio));

    newtio.c_cflag = BAUDRATE|CS8|CLOCAL|CREAD;
    newtio.c_iflag = IGNPAR;

    newtio.c_oflag = 0;
    /* 設定為正規模式 */
    newtio.c_lflag = ICANON;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    printf("Writing ...\n");
    res=write(fd, "\r\nUart tx test", 15);

    while(1) 
    {
        while((ch=getchar()) != STOP) 
        {

            s1[0]=ch;
            res=write(fd, s1, 1);
        }

        /* 將收到的字元傳送出去 */
        s1[0]=ch;
        s1[1]='\n';

        res = write(fd, s1, 2);
        break;
    }

    printf("Close...\n");
    close(fd);
    tcsetattr(fd, TCSANOW, &oldtio);

    return 0;
}

