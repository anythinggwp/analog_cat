#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void help(){
    fprintf(stdout, "Инструкция к утилите\n\t-b --number-nonblank\t нумеровать непустые строки при выводе\n\t-E --show-ends\t показывать $ в конце каждой строки\n\t-n --number\t нумеровать все строки при выводе\n\t-T --show-tabs\t показывать символы табуляции как ^I\n Если файл не задан или задан как -, читает стандартный ввод\n");
    exit(0);
}
void flag_handler(int *c, bool *flg, int *line_num, bool *nlmark, bool *prev){
    if ((flg[1] == 1 || flg[0] == 1) && (*c) != '\n') {
        (*prev) = 0;
    }
    if(flg[1] == 1 && flg[0] != 1){
        if((*nlmark) == 1){
            printf("    %d\t", (*line_num));
            (*line_num)++;
            (*nlmark) = 0;
        }
    }
    if(flg[0] == 1) {
        if((*nlmark) == 1 && (*prev) == 0){
            printf("   %d  ", (*line_num));
            (*line_num)++;
            (*nlmark) = 0;
            (*prev) = 1;
        }
    }
    if(flg[2] == 1 && (*c) == '\n'){fputc('$',stdout);}
    if(flg[3] == 1 && (*c) == '\t'){
        fputc('^',stdout);
        (*c) = 'I';
    }
    if((flg[1] == 1 || flg[0] == 1) && (*c) == '\n'){
        (*nlmark) = 1;
        (*prev) = 1;
        }
}
int output(bool *flg, int *line_num, char *path){
    int c;
    bool nlmark = 1, prev = 0;
    FILE *input_path;
    if (strcmp(path, "-") == 0) {input_path = stdin;}
    else {input_path = fopen(path, "r");}
    if (input_path != NULL){
        while (true){
            c = fgetc(input_path);
            if (c == EOF){break; }
            else {
                flag_handler(&c, flg, line_num, &nlmark, &prev);
                fputc(c, stdout);
                }
            }
        if (input_path == stdin){ clearerr(stdin);}
        else {fclose(input_path);}
    }
    else {
        fprintf(stderr, "Ошибка нет такого файла или каталога! %s\n", path);
        return -1;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    const int FLAGS_CNT = 5;
    const char FLAGS[4] = {'b', 'n', 'E', 'T'};
    bool cur_flags[4] = {0, 0, 0, 0};
    int line_num = 1;
    int err = 0;
    bool ones_read = 0;
    if (argc == 1) { int err = output(cur_flags, &line_num, "-");}
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i],"--help") == 0){
            help();
        }
        else if(strcmp(argv[i],"--show-tabs") == 0){ cur_flags[3] = 1;}
        else if(strcmp(argv[i],"--number-nonblank") == 0){ cur_flags[0] = 1;}
        else if(strcmp(argv[i],"--show-ends") == 0){ cur_flags[2] = 1;}
        else if(strcmp(argv[i],"--number") == 0){ cur_flags[1] = 1;}
        else if(argv[i][0] == '-'){
            for (int ii= 1; ii < strlen(argv[i]); ii++){
                for (int iii = 0; iii < FLAGS_CNT; iii++){
                    if(FLAGS[iii] == argv[i][ii]){
                        cur_flags[iii] = 1;
                        break;
                    }
                    else if (FLAGS[iii] != argv[i][ii] && iii < (FLAGS_CNT - 1)){
                         continue;
                         }

                    fprintf(stderr,"Ошибка нет такого ключа! -%c\nЕсли это файл используйте следующую конструкцию ./[File_name]\n",
                    argv[i][ii]);
                    exit(1);
                }

            }
        }

    }
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-") == 0){
            int tmp = output(cur_flags, &line_num, argv[i]);
            if (tmp == -1){err = tmp;}
            ones_read = 1;
            }
        else if(strlen(argv[i]) > 1 && argv[i][0] != '-'){
            int tmp = output(cur_flags, &line_num, argv[i]);
            if (tmp == -1){err = tmp;}
            ones_read = 1;
        }
    }
    if(ones_read != 1 && argc > 1){
        int tmp = output(cur_flags, &line_num, "-");
        if (tmp == -1){err = tmp;}
    }
    return err;
}