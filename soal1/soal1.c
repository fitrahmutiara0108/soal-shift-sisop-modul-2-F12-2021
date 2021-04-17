#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

void moveFiles(char *folderName, char *stevFolderName)
{
    int status;
    struct dirent *dp;
    DIR *dir = opendir(folderName);

    if (dir != NULL)
    {
        while ((dp = readdir(dir)))
        {
            pid_t childMove = fork();
            if (childMove == 0 && (strcmp(dp->d_name, ".") == 0 && strcmp(dp->d_name, "..") == 0))
            {
                exit(EXIT_SUCCESS);
            }
            if (childMove == 0)
            {
                //printf("%s\n", dp->d_name);
                char path[200];
                // Construct new path from our base path
                strcpy(path, folderName);
                strcat(path, "/");
                strcat(path, dp->d_name);
                //move files
                char *move[] = {"mv", path, stevFolderName, NULL};
                execv("/bin/mv", move);
            }
            while (wait(&status) > 0);
        }

        (void)closedir(dir);
    }
}

int main() 
{
    pid_t pid, sid;        // Variabel untuk menyimpan PID
    pid = fork();     // Menyimpan PID dari Child Process
    int daemonStatus;
    int status;

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }
 
 // KALO NYOBA JGN LUPA GANTI DIREKTORI NYA WOI
  if ((chdir("/home/miqbdi/Sisop/soal-shift-sisop-modul-2-F12-2021/soal1")) < 0) {
    exit(EXIT_FAILURE);
  }
  
  //printf("anjay soeharto\n");
  
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  int stev_bhour = 22;
  int stev_bmin = 22;
  int stev_bday = 9;
  int stev_bmonth = 3;

  while (1) 
  {

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    //Jika sudah waktunya Ultah
    if (stev_bhour == t->tm_hour && stev_bmin == t->tm_min && t->tm_sec == 0 
        && stev_bday == t->tm_mday && stev_bmonth == t->tm_mon)
    {
        pid_t child_id;
        if ((child_id = fork()) == 0)
        {
            char *zipFile[] = {"zip", "-vmqr", "Lopyu_Stevany.zip", "Musyik", "Pyoto", "Fylm", NULL};
            execv("/bin/zip", zipFile);
        }
        while((wait(&status)) > 0);
    }

    //Jika masih 6 jam sebelum ultah
    else if (stev_bhour - 6 == t->tm_hour && stev_bmin == t->tm_min && t->tm_sec == 0 
            && stev_bday == t->tm_mday && stev_bmonth == t->tm_mon)
    {

        pid_t child1, child2, child3, child4;
        
        if ((child1 = fork()) == 0)
        {
            pid_t pid1 = fork();
            pid_t pid2 = fork();
            if (pid1 == 0)
            {
                if (pid2 == 0)
                {
                    char *linkfilm[] = {"wget", "--no-check-certificate", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", 
                    "-O", "Film_for_Stevany.zip","-q", NULL};
                    execv("/bin/wget", linkfilm);
                }
                else
                {
                    while((wait(&status)) > 0);
                    char *extfilm[] = {"unzip","-qq","Film_for_Stevany.zip",NULL};
                    execv("/bin/unzip", extfilm);
                }
            }
            else
            {
                if (pid2 == 0)
                {
                    char *makefolder1[] = {"mkdir","Fylm", NULL};
                    execv("/bin/mkdir", makefolder1);
                }
                else
                {
                    while((wait(&status)) > 0);
                    moveFiles("FILM", "Fylm");
                    char *rmFilm[] = {"rm", "-fr", "FILM", NULL};
                    execvp("rm", rmFilm);
                }
            }
        }
        else if ((child2=fork()) == 0)
        {
            pid_t pid1 = fork();
            pid_t pid2 = fork();
            if (pid1 == 0)
            {
                if (pid2 == 0)
                {
                    char *linkmusic[] = {"wget", "--no-check-certificate", "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download", 
                    "-O", "Musik_for_Stevany.zip","-q", NULL};
                    execv("/bin/wget", linkmusic);
                }
                else
                {
                    while((wait(&status)) > 0);
                    char *extmusic[] = {"unzip","-qq","Musik_for_Stevany.zip",NULL};
                    execv("/bin/unzip", extmusic);
                }  
            }
            else
            {
                if (pid2 == 0)
                {
                    char *makefolder2[] = {"mkdir","Musyik", NULL};
                    execv("/bin/mkdir", makefolder2);
                }
                else
                {
                    while((wait(&status)) > 0);
                    moveFiles("MUSIK", "Musyik");
                    char *rmMusic[] = {"rm", "-fr", "MUSIK", NULL};
                    execvp("rm", rmMusic);
                }
                
            }
        }
        else if ((child3=fork()) == 0)
        {
            pid_t pid1 = fork();
            pid_t pid2 = fork();
            if (pid1 == 0)
            {
                if (pid2 == 0)
                {
                    char *linkfoto[] = {"wget", "--no-check-certificate", "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download", 
                    "-O", "Foto_for_Stevany.zip","-q", NULL};
                    execv("/bin/wget", linkfoto);
                }
                else
                {
                    while((wait(&status)) > 0);
                    char *extfoto[] = {"unzip","-qq","Foto_for_Stevany.zip",NULL};
                    execv("/bin/unzip", extfoto);
                }
            }
            else
            {
                if (pid2 == 0)
                {
                    char *makefolder3[] = {"mkdir","Pyoto", NULL};
                    execv("/bin/mkdir", makefolder3);
                }
                else
                {
                    while((wait(&status)) > 0);
                    moveFiles("FOTO", "Pyoto");
                    char *rmFoto[] = {"rm", "-fr", "FOTO", NULL};
                    execvp("rm", rmFoto);
                }
            }
        }

        while((wait(&status)) > 0);
    }

    while((wait(&daemonStatus)) > 0);
    sleep(1);
  }

}