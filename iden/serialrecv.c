/* rs232_rx.c */
#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#define BAUDRATE        B115200
#define MODEMDEVICE     "/dev/ttymxc0"
#define _POSIX_SOURCE   1

int main(int argc, char *argv[])
{

    int    fd, c=0, res;
    struct termios  oldtio, newtio;

    char  buf[256];

    printf("Start...\n");
    fd = open(MODEMDEVICE, O_RDWR|O_NOCTTY);

    if (fd < 0) {
        perror(MODEMDEVICE);
        exit(1);
    }

    printf("Open...\n");
    tcgetattr(fd, &oldtio);
    bzero(&newtio, sizeof(newtio));

    newtio.c_cflag = BAUDRATE|CS8|CLOCAL|CREAD|ECHO;

    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* 設定為正規模式 */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    printf("Reading...\n");
    while(1) {
        res = read(fd, buf, 255);

        if(res==0)
            continue;
        buf[res]=0;

        printf("res=%d  buf=%s\n", res, buf);
        if (buf[0] == '@') break;
    }

    printf("Close...\n");
    close(fd);
    tcsetattr(fd, TCSANOW, &oldtio);

    return 0;
}

