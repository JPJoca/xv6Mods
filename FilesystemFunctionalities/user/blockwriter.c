#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

    static int block = 150;
    static char txt[26] = "long.txt";
    char niz[512];

void napuniNiz( char* niz){
    int i;
    char karakter = 'a';
    for (i = 0; i < 512; i++) {
        niz[i] = karakter;
        karakter++;
        if (karakter > 'z') {
            karakter = 'a';
        }
    }
}

void
help(){
    printf("Use this program to create a big file filled whit a-z charaktars \n");
    printf("Default filename: long.txt \n");
    printf("Default blocks: 150  \n");
    printf("Usage blockwriter [OPTION]  \n\n");
    printf("Command line options:  \n");
    printf("-h -help: show help prompt \n");
    printf("-b -blocks: Numbre of blocks to write \n");
    printf("-o -output : set output file name  \n"); 


}

void 
ispisi(){
    napuniNiz(niz);
    int fd = open(txt,O_CREATE | O_RDWR);
    for(int i =0; i<block; i++){
            printf("Blockwriter %d\n", i);
            write(fd,niz,512);
        }
        close(fd);
}
int
main(int argc, char *argv[])
{
   
    int flag = 0;
    if((strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"-help") == 0) && argc ==2){
        help();
    }else{
        if(argc == 1){
            ispisi();
        }else{
            for(int i = 1; i<argc;i++){
                if(strcmp(argv[i],"-o") == 0 || strcmp(argv[i],"-output") == 0)
                     strcpy(txt,argv[++i]);
                else if(strcmp(argv[i],"-b") == 0 || strcmp(argv[i],"-block") == 0)
                     block = atoi(argv[++i]);
                else{
                    flag = 1;
                    break;
                    }
            }
            if(flag == 1){
             printf("Error !\n");
            } else if(flag == 0){
                 ispisi();
        }
        }
}
	exit();
}
