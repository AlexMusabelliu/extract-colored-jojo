#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char *argv[]){
    DIR *d = opendir(argv[1]);
    DIR *d2;
    char argName [strlen(argv[1]) + 9];
    char *finName;
    char cwdbuf[PATH_MAX];
    strcat(strcpy(argName, argv[1]), " Combined");
    struct dirent *entry;
    struct stat filestat;
    struct dirent *entry2;
    struct stat filestat2;
    char *locator;
    mkdir(argName);
    if (d != NULL){
        while (entry=readdir(d))
        {
            stat(entry->d_name, &filestat);
            if (S_ISDIR(filestat.st_mode)){
                d2 = opendir(entry->d_name);
                while (entry2=readdir(d2)){
                    if (entry2->d_name[0] != '.' && entry->d_name[0] != '.'){
                        locator = malloc(sizeof(entry2->d_name) + sizeof(entry->d_name));
                        strcpy(locator, entry->d_name);
                        strcat(locator, "/");
                        strcat(locator, entry2->d_name);
                        stat(locator, &filestat2);
                        if (S_ISREG(filestat2.st_mode)){
                            finName = malloc(sizeof(entry2->d_name) + sizeof(argName));
                            strcpy(finName, argName);
                            strcat(finName, "/");
                            strcat(finName, entry2->d_name);
                            int f = rename(locator, finName);
                            if (f == -1){
                                printf("Moving %s to %s failed.\n", locator, finName);
                            }
                            free(finName);
                        }
                        free(locator);
                    }
                }
            }
        }
    }
    closedir(d);

    return 0;
}