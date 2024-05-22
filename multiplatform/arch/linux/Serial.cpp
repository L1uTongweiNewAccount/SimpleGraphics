#include "../Serial.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <dirent.h>

HardwareSerial::operator bool(){
    return fd != -1;
}

// Find the first of available SimpleGraphics
void HardwareSerial::begin(uint32_t baudrate){
    struct DIR dir = opendir("/dev");
    struct dirent ptr;
    while(ptr = readdir(dir)){
        fd = -1;
        if(ptr.d_type == 2 && !strcmp_n(ptr.d_name, "ttyS", 4)){ // DT_CHR = 2
            if((fd = open(strcat("/dev/", ptr.d_name), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK) == -1)) {end(); continue;}
            if(fcntl(fd, F_SETLK, &fl) == -1) {end(); continue;}
            struct termios Opt = {0};
            tcgetattr(fd, &Opt);
            Opt.c_cflag &= ~PARENB;
            Opt.c_cflag &= ~CSTOPB;
            Opt.c_cflag &= ~CSIZE;
            Opt.c_cflag |= ~CS8;
            cfsetispeed(&Opt, B115200);
            cfsetospeed(&Opt, B115200);
            Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
            Opt.c_oflag &= ~OPOST;
            Opt.c_lflag = 0;
            Opt.c_oflag = 0;
            Opt.c_iflag = 0;
            Opt.c_cc[VINTR] = 0;        /* Ctrl-c*/
            Opt.c_cc[VQUIT] = 0;        /* Ctrl-\ */
            Opt.c_cc[VERASE] = 0;       /* del */
            Opt.c_cc[VKILL] = 0;        /* @ */
            Opt.c_cc[VEOF] = 4;         /* Ctrl-d */
            Opt.c_cc[VTIME] = 5;        /* inter-character timer, timeout VTIME*0.1 */
            Opt.c_cc[VMIN] = 0;         /* blocking read until VMIN character arrives */
            Opt.c_cc[VSWTC] = 0;        /* '\0' */
            Opt.c_cc[VSTART] = 0;       /* Ctrl-q */
            Opt.c_cc[VSTOP] = 0;        /* Ctrl-s */
            Opt.c_cc[VSUSP] = 0;        /* Ctrl-z */
            Opt.c_cc[VEOL] = 0;         /* '\0' */
            Opt.c_cc[VREPRINT] = 0;     /* Ctrl-r */
            Opt.c_cc[VDISCARD] = 0;     /* Ctrl-u */
            Opt.c_cc[VWERASE] = 0;      /* Ctrl-w */
            Opt.c_cc[VLNEXT] = 0;       /* Ctrl-v */
            Opt.c_cc[VEOL2] = 0;        /* '\0' */
            tcsetattr(fd, TCANOW, &Opt);
            if(write(fd, "\0", 1) <= 0) {end(); continue;}
            udelay(100);
            if(read(fd, buf, 20) <= 0) {end(); continue;}
            if(strcmp(buf, "SimpleGraphics v1.0")) {end(); continue;}
        }
    }
    closedir(dir);
}

void HardwareSerial::write(uint8_t* buf, uint16_t length){
    write(fd, buf, length);
}

void HardwareSerial::read(uint8_t* buf, uint16_t length){
    read(fd, buf, length);
}

void HardwareSerial::end(){
    close(fd);
    fd = -1;
}