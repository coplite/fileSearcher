#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/stat.h>

#define PATH_MAX 4096

void searchfs(char* path, char* file){
    struct dirent* parts = NULL;
    struct stat st;
    char fullpath[PATH_MAX] = {0};
    snprintf(fullpath, sizeof(fullpath), "%s", path);
    DIR* ddir = opendir(path);
    if(!ddir){
        perror("opendir");
        return;
    }
    while((parts = readdir(ddir)) != NULL){
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, parts->d_name);
        if(stat(fullpath, &st)){
            perror("stat");
            continue;
        }
        if(S_ISREG(st.st_mode)){
            if(!fnmatch(file, parts->d_name, 0)){
                snprintf(fullpath, sizeof(fullpath), "%s/%s", path, parts->d_name);
                printf("type: file\tname: %s\n", fullpath);
            }
        }else if(S_ISDIR(st.st_mode)){
            if(!strcmp(parts->d_name, "..") || !strcmp(parts->d_name, ".")){
                continue;
            }
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, parts->d_name);
            searchfs(fullpath, file);
        }
    }
    if(closedir(ddir)){
        perror("closedir");
        return;
    }
    return;
}
void searchds(char* path, char* file){
    struct dirent* parts = NULL;
    struct stat st;
    char fullpath[PATH_MAX] = {0};
    snprintf(fullpath, sizeof(fullpath), "%s", path);
    DIR* ddir = opendir(path);
    if(!ddir){
        perror("opendir");
        return;
    }
    while((parts = readdir(ddir)) != NULL){
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, parts->d_name);
        if(stat(fullpath, &st)){
            perror("stat");
            continue;
        }
        if(S_ISDIR(st.st_mode)){
            if(!strcmp(parts->d_name, "..") || !strcmp(parts->d_name, ".")){
                continue;
            }
            if(!fnmatch(file, parts->d_name, 0)){
                snprintf(fullpath, sizeof(fullpath), "%s/%s", path, parts->d_name);
                printf("type: dir\tname: %s\n", fullpath);
            }
            searchds(fullpath, file);
        }
    }
    if(closedir(ddir)){
        perror("closedir");
        return;
    }
    return;
}
int main(int argc, char** argv){
    if(argc != 7){
        printf("Usage: %s --root {Absolute/File/Path} --type {file/dir} --name {NameOfFileOrDirectory}\n", argv[0]);
        puts("[!] USE ESCAPE CHARACTERS OR QUOTES WHEN UTILIZING WILDCARDS");
        return -1;
    }
    if(!strcmp(argv[4], "file")){
        searchfs(argv[2], argv[6]);
    }else if(!strcmp(argv[4], "dir")){
        searchds(argv[2], argv[6]);
    }else{
        perror("strcmp");
        return 1;
    }
    return 0;
}
