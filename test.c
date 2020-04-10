#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <termios.h>
#include <error.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ioctl.h>

#define CTR_KEY(k) ((k) & 0x1f)
#define SIZE_HISTORY 50
#define TRUE 1 

struct termios old_term,term;

int head , tail ,cnt=0, cntcmd =0, size_history = 50;
char * history[SIZE_HISTORY]; 
char  cmd [1024]; 

void init_history(void); 
void print_history (void); 
void load_history(void); 
void save_history(void); 

int getCursorPosition(int *rows , int *cols);
int getWindowSize (int *rows , int *cols) ;
void enableRawMode (void);
void disableRawMode(void);  
void enableRawMode(void); 

int main (int argc , char * argv [])
{
    
    init_history();
    load_history();
    
    enableRawMode();

    initializeShell();

    while (TRUE)
    { 
        char c ;
        for (read(STDIN_FILENO,&c ,1); c!=CTR_KEY('c'); read (STDIN_FILENO,&c ,1))
        {            
            if (c=='\e')
            {
                char seq [3];
                read(STDIN_FILENO,seq,2);

                if (seq[0] == '[')
                {
                    switch (seq[1])
                    {
                    case 'A':   
                    {
                        continue; 
                    }                     
                    case 'B':
                    {
                        continue; 
                    }
                    case 'C':
                    {
                        continue; 
                    }
                    case 'D':
                    {                        
                        continue;
                    }
                    default:
                    {
                        break;
                    }
                    }
                }
            }
            
            if (c=='\r')
            {                
                printf("Pressed enter\r\n");
            }
            if (c==127)
            {
                if (cnt>0)
                {                    
                    cmd[--cnt] = '\0';                    
                    //int row ,col ; 
                    //int e = getCursorPosition(&row,&col); 
                    //if (e<0) continue ; 
                   
                    
                    write(STDOUT_FILENO,"\x1b[D",3); 
                    write (STDOUT_FILENO,"\x1b[K",3);                  
                    
                }
            }
            else 
            {
                cmd [cnt++] = c;
                printf("%c\n\e[1A",c);
            }
        }
        

        if (c == CTR_KEY(c))
        {
            tcsetattr(STDIN_FILENO,TCSANOW,&old_term);
            exit(0);
        }
    }
      
}


void init_history (void)
{
    int i ; 
    for (i=0 ; i < SIZE_HISTORY ; i++)
    {
        history[i] = (char *) malloc (sizeof(char)*1024);
    } 
} 

void print_history (void)
{
    int i ; 
    for (i = 0; i < cntcmd ; i++)
    {   
        printf("%d %s\n" , i+1 , history[i]) ;
    }
} 

void save_history (void)
{
    FILE *f = NULL ; 
    f = fopen (".history" , "w"); 

    int cnt = 0 ; 
    while (cnt ++ <  cntcmd )
    {
        fputs(history [cnt], (FILE *) f);
        if(cnt != cntcmd-1) fputs("\n", (FILE *) f);
    }
    
}

void load_history (void)
{
    FILE *f =NULL ; 
    f = fopen (".history", "r"); 
    cntcmd = 0 ; 

    while (fgets(history[cntcmd],1024 ,(FILE*)f ))
    {
        int n = strlen(history[cntcmd]); 
        if (history[cntcmd][n-1] == '\n') history[cntcmd][n-1] = 0; 
        cntcmd ++ ; 
    }
    
} 

int getCursorPosition(int *rows , int *cols)
{
    char buf [32] ; 
    unsigned int i = 0;

    if (write(STDIN_FILENO, "\x1b[6n",4)!=4) return -1; 

    while (i<sizeof(buf)-1)
    {
        if (read(STDIN_FILENO,&buf[i],1)!=1)break ; 
        if (buf[i] == 'R')break; 
        i++; 
    }

    buf[i] = '\0';

    if (buf[0]!= '\x1b' || buf [1]!='[')return -1 ; 
    if (sscanf(&buf[2],"%d;%d", rows,cols)!=2) return -1; 

    return 0 ;    
}


int getWindowSize (int *rows , int *cols)
{
    struct winsize ws ; 
    
    if (ioctl(STDIN_FILENO,TIOCGWINSZ ,&ws)==-1||ws.ws_col==0)
    {
        if (write(STDOUT_FILENO,"\x1b[999C\x1b[999B",12)!=12)return -1;
        return getCursorPosition(rows,cols);
    }
    else 
    {
        *cols = ws.ws_col ; 
        *rows = ws.ws_row ; 
        return 0; 
    }
}

void initializeShell (void)
{
    write(STDOUT_FILENO,"\x1b[2J",4);
    write(STDOUT_FILENO,"\x1b[?25l" , 6);
    write(STDOUT_FILENO,"\x1b[H" , 3);
    

    char buf [7] = "Shell$ ";
    int i = 0 ; 
    while (i<strlen(buf))
    {                  
        printf("%c\n\e[1A",buf[i]);
        i++;
    }

    write(STDOUT_FILENO,"\x1b[?25h" , 6);
}


void enableRawMode (void)
{
    if (tcgetattr(STDIN_FILENO,&old_term))
    {
        perror ("Error while getting default terminal\n");
        exit(1);
    }

    term = old_term;
    cfmakeraw(&term);
    /*
    term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    term.c_oflag &= ~(OPOST);
    term.c_cflag |= (CS8);
    term.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    */

    if (tcsetattr(STDIN_FILENO,TCSANOW,&term))
    {
        perror("Error while seting new config\n");
        exit(1);
    }
}