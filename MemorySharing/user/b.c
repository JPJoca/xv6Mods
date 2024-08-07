#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"



int
main(int argc, char *argv[])
{
	char str[20]= {"Cao ja sam dete"};
	char *niz;
	get_data("niz", &niz);
	printf("Povuceno iz roditelja: %s\n", niz);

	strcpy(niz,str);
	exit();
}
