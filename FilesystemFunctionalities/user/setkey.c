#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"
void 
help(){
	    printf("%s","Use this program to set current active key. \n");
		printf("%s","After setting the key, you can use encr and decr whit that key. \n");
		printf("%s","Usage : setkey[OPTION] .. [KEY] \n\n");
		printf("%s","Command line options:  \n");
		printf("%s","-h, -help: Show help  \n");
		printf("%s","-s, -secret: enter the key secretly  \n");

}
int 
ucitaj(){
	char buffer[10];             
 	int num;           
 	read(0, buffer, sizeof(buffer));
 	 return num = atoi(buffer);
}
int
main(int argc, char *argv[])
{
	



	
	if(argv[1] >= 0 && argc == 2 && (atoi(argv[1]) > 0 || strcmp(argv[1],"0") == 0)){
		setkey(atoi(argv[1]));
	}else if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"-help") == 0 ){
        help();
    }
	else if(strcmp(argv[1],"-s") == 0 || strcmp(argv[1],"-secret") == 0 ){
		
        setecho(0);
		printf("%s","Set key : ");
		int n = ucitaj();
		setkey(n);
		printf("\n");
		setecho(1);
    }else if(strcmp(argv[1],"-s") != 0 || strcmp(argv[1],"-secret") != 0 || strcmp(argv[1],"-h") != 0 || strcmp(argv[1],"-help") != 0 ) {
		printf("%s","error\n");
	}
	
	
	exit();
}
