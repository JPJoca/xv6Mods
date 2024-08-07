#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
void
help(){
    printf("Use this program to deccrypt files written on the disk. \n");
  
    printf("Usage encr [OPTION]  \n\n");
    printf("Command line options:  \n");
    printf("-h -help: show help prompt \n");
    printf("-a -all: Decrypt all files in CWD whit current key \n");
    


}
void 
dekriptuj(char* n){
    int fd = decr(open(n, O_RDWR));
    switch(fd){
        case -1:
        printf("%s %s \n","error[key not found]", n);
        break;
         case -2:
        printf("%s %s \n","error[fail to open]", n);
        break;
         case -3:
        printf("%s %s \n","error[cant decrypt, not encrypted]", n);
        break;
         case -4:
        printf("%s %s \n","error[file not found]", n);
        break;
        case -5:
        printf("%s %s \n","error[cant decrypt]", n);
        break;
        default:
        printf("%s %s\n","sucessfuly decrypted ",n);
        break;

    }
    close(fd);
}
void 
dekriptuj_sve(){
    int fd = open(".", 0);
    struct dirent dr;

    while(read(fd, &dr,sizeof(dr)) == sizeof(dr)){
        if(dr.inum != 0)
            dekriptuj(dr.name);
    }
        close(fd);

}
int
main(int argc, char *argv[])
{
	

if(argc == 2){
        if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"-help") == 0 ){
        help();
    }
     else if(strcmp(argv[1],"-a") == 0 || strcmp(argv[1],"-all") == 0 ){
        dekriptuj_sve();
    }else {
        dekriptuj(argv[1]);
    }
    } else  if(argc > 2)
    printf("%s","error\n");
	
	
	exit();
}
