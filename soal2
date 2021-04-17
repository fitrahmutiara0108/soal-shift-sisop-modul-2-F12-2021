#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <syslog.h>
#include <sys/wait.h>
#include <dirent.h>

int main(){

//Poin a
	int stats;
	pid_t child_id=fork();

	if(child_id==0) execl ("/bin/mkdir", "mkdir", "-p", "/home/tiara/modul2/petshop", NULL);
	else ((wait(&stats))>0);
	
	child_id=fork();
	if(child_id==0) execl ("/bin/unzip", "unzip", "pets.zip","-d", "/home/tiara/modul2/petshop", NULL);
	else ((wait(&stats))>0);

return 0;
}
