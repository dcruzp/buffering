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

#define CTR_KEY(k) ((k) & 0x1f)
#define SIZE_HISTORY 50
#define TRUE 1 

int head , tail , cntcmd =0, size_history = 50;
char * history[SIZE_HISTORY]; 
char  cmd [1024]; 

void init_history(void); 
void print_history (void); 
void load_history(void); 
void save_history(void); 

int main (int argc , char * argv [])
{

    __fpurge(stdin); 
    
    init_history() ; 
    load_history(); 

   while (TRUE)
   {
       printf("%s", "SHELL>"); 
       __fpurge(stdin); 
       memset(cmd , '\0' , 1024);   
       scanf("%[^\n]s", cmd); 
       strcpy (history[cntcmd++], cmd); 

       if (strcmp(cmd , "exit") ==0 ) 
       {
           save_history (); 
           break;
       } 
       else if (strncmp(cmd , "history" , 7)==0 ) print_history();        
   }
   

    /*

    struct termios old_term , term ;
    char c ; 

    if (tcgetattr(STDIN_FILENO, &old_term))
    {
        perror ("Error while getting default terminal\n"); 
        exit(1); 
    }

    term = old_term; 

    cfmakeraw(&term);

    if (tcsetattr(STDIN_FILENO,TCSANOW,&term))
    {   
        perror ("Error while seting new config \n");
        exit(1); 
    }

    for (read(STDIN_FILENO, &c , 1); c != CTR_KEY('c'); read (STDIN_FILENO,&c ,1))
    {
        if (c =='\e')
        {
            char seq [3]; 
            read(STDIN_FILENO,seq,2);

            if (seq[0] == '[')
            {
                switch (seq[1])
                {
                case 'A':
                {
                    printf("Pessed Arrow up\r\n");
                    continue ; 
                }
                case 'B':
                {
                    printf("Pessed Arrow down\r\n");
                    continue ; 
                }
                case 'D':
                {
                    printf("Pessed Arrow left\r\n");
                    continue ; 
                }
                case 'C':
                {
                    printf("Pessed Arrow right\r\n");
                    continue ; 
                }
                }
            }
        }
        else 
        {
            printf("%c\n\e[1A", c);
        }
    }

    tcsetattr(STDIN_FILENO,TCSANOW,&old_term);
    exit (0);

    */
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
        //printf("%s\n",history[cntcmd]); 
        cntcmd ++ ; 
    }
    
} 

