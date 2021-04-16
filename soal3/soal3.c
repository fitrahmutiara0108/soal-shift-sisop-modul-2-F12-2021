#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

char* getNameByTime() {
	time_t now = time(NULL);
	struct tm* timePtr = localtime(&now);

	char *name = malloc(50 * sizeof(char));

	strftime(name, 50, "%Y-%m-%d_%X", timePtr);
	return name;
}

char* caesar(char* in, int shift) {
	char* out = malloc(20 * sizeof(char));
	int i = 0;

	while (i < strlen(in)) {
		if (in[i] >= 'a' && in[i] <= 'z') {
			out[i] = 'a' + (in[i] - 'a' + shift) % 26;
		} else if (in[i] >= 'A' && in[i] <= 'Z') {
			out[i] = 'A' + (in[i] - 'A' + shift) % 26;
		} else {
			out[i] = in[i];
		}
		i++;
	}
	return out;
}

int main(int argc, char *argv[]) {
	if (argc != 2 || (strcmp(argv[1], "-z") != 0 && strcmp(argv[1], "-x") != 0)) {
		printf("Wrong argument");
		return 1;
	}
	
	pid_t pid = fork(), sid = setsid();
	
	if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    
    umask(0);

	FILE *killer = fopen("Killer.sh", "w");
	fprintf(killer, "#!/bin/bash\n");

	if (strcmp(argv[1], "-z") == 0)
		fprintf(killer, "rm $0\nkillall -9 ./soal3\n");
	else if (strcmp(argv[1], "-x") == 0)
		fprintf(killer, "rm $0\nkill %d\n", getpid());

	fclose(killer);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	while (1) {
		char *pathName = getNameByTime();
		
		printf("%s\n", pathName);

		pid_t child_id = fork();

		if (child_id == 0) {
			execl("/bin/mkdir", "/bin/mkdir", pathName, NULL);
		}

		child_id = fork();

		if (child_id == 0) {
			int images = 10;
			while (images--) {
				child_id = fork();

				if (child_id == 0) {
					char dir[160], url[64];

					int ext = 50 + (int)time(NULL) % 1000;

					sprintf(dir, "%s/%s.jpg", pathName, getNameByTime());
					sprintf(url, "https://picsum.photos/%d", ext);

					execl("/bin/wget", "/bin/wget", "-q", "-O", dir, url, NULL);
				}
				sleep(5);
			}

			while (wait(NULL) > 0);
			child_id = fork();

			if (child_id == 0) {
				char *fileName = malloc(100 * sizeof(char));
				sprintf(fileName, "%s/status.txt", pathName);

				FILE *status = fopen(fileName, "w");
				fputs(caesar("Download Success", 5), status);
				fclose(status);
				
				char zipName[100];
				
				sprintf(zipName, "%s.zip", pathName);
				execl("/bin/zip", "/bin/zip", "-r", zipName, pathName, NULL);
			}
			while (wait(NULL) != child_id);
			
            execl("/bin/rm", "/bin/rm", "-r", pathName, NULL);
		}
		sleep(40);
	}
}
