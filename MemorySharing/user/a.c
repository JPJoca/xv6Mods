#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"



int
main(int argc, char *argv[])
{
	
	char niz[20] = {"Pozdrav svima"};

	share_data("niz", &niz, 20);
	
	int frk = fork();
	
	if(frk == 0){
		exec("/bin/b", argv);
	}

	
	wait();
	
	printf("Dete izmenilo: %s\n", niz);
	exit();
}
