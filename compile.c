#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
struct lists {
    char array[50][100];
};

struct lists get_directory(char*, int*);
void get_hint(char*, int);
int compile();
int complete();
void choppy(char*);
int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Enter a valid argument verify/watch \n");
        exit(-1);
    }
    char* directory = malloc(128 * sizeof(char));
    struct lists dir_list;
    int exe_cnt = 0;
    dir_list = get_directory((char*)directory, &exe_cnt);

    if (strcmp("watch", argv[1]) == 0) {
        int j = 0;
        while (j < exe_cnt) {
            sprintf(directory, "%s", dir_list.array[j]);
            struct lists prob_list;
            int counts = 0;
            printf("directoy_data: %s \n", directory);
            prob_list = get_directory((char*)directory, &counts);
            printf("inside while %s", directory);
            int i = 0;
            while (i < counts) {
                if (strstr(prob_list.array[i], "hint.txt") != NULL) {
                    i++;
                    continue;
                }
                if (compile(prob_list.array[i]) == 0) {
                    if (complete(prob_list.array[i]) != 1) {
                        printf(
                            "Remove //I AM NOT DONE comment to continue "
                            "compilation \n");
                        char u;
                        scanf("%c", &u);
                    } else {
                        i++;
                    }
                } else {
                    get_hint((char*)directory, i);
                }
            }
            j++;
        }
    } else if (strcmp("verify", argv[1]) == 0) {
        printf("\n Inside verify %s \n", directory);
        for (int j = 1; j < exe_cnt; j++) {
            sprintf(directory, "%s", dir_list.array[j]);
            struct lists prob_list;
            int counts = 0;
            prob_list = get_directory((char*)directory, &counts);
            for (int k = 0; k < counts; k++) {
                compile(prob_list.array[k]);
            }
        }
    } else {
        printf("Commnad not found \n");
    }
}
struct lists get_directory(char* path, int* count) {
    char cwd[PATH_MAX] = {0}, command[128] = {0}, *line = NULL;
    size_t len = 0;
    struct lists result;
    char* file_name;
    printf("Value passed in get_directory: :%s \n", path);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("problem- \n");
    } else {
        perror("getcwd() error");
    }
    if (strcmp(path, "") == 0) {
        sprintf(command, "%s%s%s", "ls ", cwd, "/problems/ > file.txt");
        file_name = "file.txt";
    } else {
        choppy(path);
        strcat(path, "/");
        sprintf(command, "%s%s%s", "ls ", path, " > problem.txt");
        file_name = "problem.txt";
    }
    printf("Inside get_directoy \n");
    printf("\n Total command after strcats: %s \n", command);
    system(command);

    FILE* fp;
    if ((fp = fopen(file_name, "r")) == NULL) {
        perror("Unable to Open file");
        exit(1);
    }
    printf("\n path %s, cwd: %s \n", path, cwd);
    if (strcmp(path, "") == 0) {
        sprintf(path, "%s%s", cwd, "/problems/");
    }
    printf("late path: %s \n", path);
    int j = 0;
    while (getline(&line, &len, fp) != -1) {
        sprintf(result.array[j], "%s%s", path, line);
        printf("%s", result.array[j]);
        j++;
    }
    *count = j;
    fclose(fp);
    return result;
    // sprintf(path,"%s%s%d%s",cwd,"/problems/",i,".c");
    // printf("%s\n", path);
}
int compile(char* paths) {
    char* gcc = malloc(128 * sizeof(char));

    sprintf(gcc, "%s%s", "gcc ", paths);
    int j = system(gcc);
    // printf("%d", j);

    if (j == 0) {
        printf("\nCONGRATULATIONS! problem compiled\n");
        return j;
    } else {
        printf("\n solve error to compile");
        j = -1;
    }
    return j;
}

int complete(char* files) {
    char temp[512] = {0};
    char* file_name = malloc(128 * sizeof(char));
    FILE* fp;
    char* str = "//I AM NOT DONE";
    sprintf(file_name, "%s", strtok(files, "\n"));
    printf("%s \n", files);
    if ((fp = fopen(files, "r")) == NULL) {
        perror("File Opening Error");
        return (-1);
    }
    while (fgets(temp, 512, fp) != NULL) {
        if (strstr(temp, str) != NULL) {
            return (0);
        }
    }
    if (fp) {
        fclose(fp);
    }
    return (1);
}
void get_hint(char* hint_directory, int numb) {
    FILE* fp;
    char temp[512] = {0};
    char h[64] = {0};
    char hint[64] = {0};
    printf("for hint type <problem.hint> \n");
    strcpy(h, "problem.hint");
    scanf("%s", hint);
    if (strcmp(h, hint) == 0) {
        if (strstr(hint_directory, "hint.txt") == NULL) {
            strcat(hint_directory, "hint.txt");
            printf("%s \n", hint_directory);
        }
        if ((fp = fopen(hint_directory, "r")) == NULL) {
            perror("File Opening Error");
        }
        int i = 0;
        while (fgets(temp, 512, fp) != NULL) {
            if (i == numb) {
                if (fp) {
                    fclose(fp);
                }
                printf("Hint: %s \n", temp);
                return;
            }
            if (fp) {
                fclose(fp);
            }
            printf("No hint for the problem \n");
        }
    } else {
        printf("Was that a typo? or you didn't want a hint? \n");
    }
}
void choppy(char* s) {
    s[strcspn(s, "\n")] = '\0';
}
