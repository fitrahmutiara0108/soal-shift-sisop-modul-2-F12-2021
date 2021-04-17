#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

char animals[50][100]={0};
char directory[] = "/home/tiara/modul2/petshop";

void group_images(char *img, char *fileName){
	int status;
	char	*jenis = strtok(fileName, ";"), 
			*nama = strtok(NULL, ";"),
			*umur = strtok(NULL, ";"),
			filePath[300], textPath[400], newName[400], text[200];
	
	sprintf(filePath, "%s/%s", directory, jenis);
    sprintf(textPath, "%s/keterangan.txt", filePath);
    sprintf(newName, "%s/%s.jpg", filePath, nama);
    
	sprintf(text, "nama : %s\numur : %s tahun\n\n", nama, umur);
	
	pid_t child_id = fork();		
	if(child_id < 0)  exit(EXIT_FAILURE);
	if(child_id == 0) execl("/bin/cp", "/bin/cp", img, newName, NULL);
	while(wait(&status) > 0);
	
	FILE *keterangan = fopen(textPath, "a");
	if(keterangan){
		fprintf(keterangan, "%s", text);
        fclose(keterangan);
	}
}

int main () {
	int status;
	
	pid_t child_id = fork();
	
	if (child_id == 0) execl("/bin/mkdir", "/bin/mkdir", "-p", directory, NULL);
	else (wait(&status) > 0);
	
	child_id = fork();
	if (child_id == 0) execl("/bin/unzip", "/bin/unzip", "pets.zip", "-d", directory, NULL);
	else (wait(&status) > 0);
	
	if(child_id == 0) execl("/bin/chmod", "/bin/chmod", "u+w", directory, NULL);
	else (wait(&status) > 0);
	
	struct dirent *direntPtr;
	DIR *dir = opendir(directory);
	
	while((direntPtr = readdir(dir)) != NULL){
		if ((direntPtr->d_type == DT_DIR) && (strcmp(direntPtr->d_name, ".") != 0) && (strcmp(direntPtr->d_name, "..") != 0)){
    		child_id = fork();
			if(child_id < 0){
    			exit(EXIT_FAILURE);
			}
			if(child_id == 0){
				char pathName[300];
				sprintf(pathName, "%s/%s", directory, direntPtr->d_name);
                execl("/bin/rm", "/bin/rm", "-r", pathName, NULL);
			}
		}
	}
	
	int i, count=0;
	while(wait(&status) > 0);
	dir = opendir(directory);
	if(dir != NULL){
		while((direntPtr = readdir(dir))){
			if(direntPtr->d_type == DT_REG){
				char fileName[100], temp[100];
				
				memset(fileName, 0, sizeof(fileName));
				memset(temp, 0, sizeof(temp));
				strcpy(fileName, direntPtr->d_name);
				
				if(direntPtr->d_name[0] != '.'){
					i = 0;
					while (fileName[i] != ';'){
						temp[i] = fileName[i];
						if(fileName[++i] == ';') break;
					}
					
					bool check = false;
					for(i=0; i<count && (!check); i++){
						if(!strcmp(animals[i], temp)) check = true;
					}
					
					if(!check) strcpy(animals[count++], temp);
				}
			}
		}
	}
	
	for(i = 0; i < count; i++){
		child_id = fork();
		if(child_id < 0){
			exit(EXIT_FAILURE);
		}
		if(child_id == 0){
			char folderName[200];
			sprintf(folderName, "%s/%s", directory, animals[i]);
            execl("/bin/mkdir", "/bin/mkdir", folderName, NULL);
		}
	}

	while(wait(&status) > 0);
	dir = opendir(directory);
	if(dir != NULL){
		while((direntPtr = readdir(dir))){
			if(direntPtr->d_type == DT_REG){
				char fileName[300], source[500];
				
				sprintf(source, "%s/%s", directory, direntPtr->d_name);
				strcpy(fileName, direntPtr->d_name);
				
                fileName[strlen(fileName)-4] = '\0';
                
                char *firstAnimal = strtok(fileName, "_");
                char *secondAnimal = strtok(NULL, "_");
				
				if(secondAnimal) group_images(source, secondAnimal);
				group_images(source, firstAnimal);
				
				child_id = fork();
				if(child_id < 0)  exit(EXIT_FAILURE);
				if(child_id == 0) execl("/bin/rm", "/bin/rm", "-rf", source, NULL);
				
				while(wait(&status) > 0);
			}
		}
	}

	return 0;
}
