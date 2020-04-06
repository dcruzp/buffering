#include <termios.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

/*
  Vamos a intentar crear un programa que haga buffering
  en todo lo que se escriba en el terminal de forma interactiva,
  permitiendo que podamos capturar y agregar funciones a teclas
  como las arrow keys.
 */

#define CTR_KEY(k) ((k) & 0x1f)

int main() {
  // Get the default terminal we are working with.
    struct termios old_term, term;
    char c;

    if (tcgetattr(STDIN_FILENO, &old_term)) {
        perror("Error while getting default terminal\n");
        exit(1);
    }

    //Now lets set default terminal in raw mode
    /*
      This is equivalent to set the following attributes:
      term.c_iflag &= ~ (IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
      term.c_oflag &= ~OPOST
      term.c_lflag &= (ECHO | ECHONL | ICANON | ISIG | IEXTEN);
      term.c_cflag &= ~(CSIZE | PARENB);
      term.c_cflag |= CS8;
     */

    // Save first the old config so we can return it back.
    term = old_term;

    cfmakeraw(&term);

    if(tcsetattr(STDIN_FILENO, TCSANOW, &term)) {
        perror("Error while seting new config\n");
        exit(1);
    }

    /*
      Al crear una terminal RAW, tenemos desactivado todo, incluido el echo, asi que
      cada vez que leamos un caracter tenemos que printearlo para poder verlo.
      No voy a manejar bien los movimientos del cursor aqui, solo les dejo un ejemplo
      para que vean como se puede hacer para escribir una linea de forma interaactiva y como
      si quieren captura las arrows keys. De aqui en adelante es cosa de ustedes.
     */

    for(read(STDIN_FILENO, &c, 1); c != CTR_KEY('c'); read(STDIN_FILENO, &c, 1)) {
        if (c == '\e') {
            char seq[3];
            read(STDIN_FILENO, seq, 2);

            if (seq[0] == '[') {
                switch (seq[1]) {
                case 'A': {
                    printf("Pressed arrow up\r\n");
                    continue;
                }
                case 'B': {
                    printf("Pressed arrow down\r\n");
                    continue;
                }
                case 'D': {
                    printf("Pressed arrow left\r\n");
                    continue;
                }
                case 'C': {
                    printf("Pressed arrow right\r\n");
                    continue;
                }                
                }
            }
        }
        else 
        {
            printf("%c\n\e[1A", c);
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    exit(0);
}
