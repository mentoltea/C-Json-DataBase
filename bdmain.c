#include "bd.h"
#include "bdinterpret.h"
#include "assert.h"

int main(int argc, char** argv) {
    init_json();
    if (argc==2) {
        FILE* fd = fopen(argv[1], "r");
        if (!fd) {
            fprintf(stderr, "Cannot open file %s!\n", argv[1]);
            return 1;
        }
        char** commands = new_vec(sizeof(char*), 10);
        char linebuffer[1024];
        char buffer[256];
        bool flag = false;
        int offset = 0;

        while (true) {
            if (feof(fd)) break;
            // fputs(">> ", stdout);
            fgets(linebuffer, 1024, fd);
            while (offset < strlen(linebuffer)-1)
            {            
                sscanf(linebuffer + offset, "%s", buffer);
                // printf(buffer);
                offset += strlen(buffer)+1;
                if (strcmp(buffer, "QUIT")==0) {
                    flag=true;
                    break;
                }
                else if (buffer[0]=='~') {
                    execute(commands);
                    vector_metainfo meta = vec_meta(commands);
                    for (int j=0; j<meta.length; j++) {
                        memfree(commands[j]);
                    }
                    vec_metaptr(commands)->length=0;
                } else {
                    char* temp = memloc(strlen(buffer)+1);
                    memcpy(temp, buffer, strlen(buffer)+1);
                    commands = vec_add(commands, &temp);
                }
            }
            offset=0;
            if (flag) break;
        }
        fclose(fd);

        vector_metainfo meta = vec_meta(commands);
        for (int j=0; j<meta.length; j++) {
            memfree(commands[j]);
        }
        delete_vec(commands);

    } else if (argc>2) {
        char** commands = new_vec(sizeof(char*), argc-1);
        for (int i=0; i<argc-1; i++) {
            if (argv[i+1][0] == '~') {
                execute(commands);
                vector_metainfo meta = vec_meta(commands);
                for (int j=0; j<meta.length; j++) {
                    memfree(commands[j]);
                }
                vec_metaptr(commands)->length=0;
            } else {
                char* buff = memloc(strlen(argv[i+1])+1);
                memcpy(buff, argv[i+1], strlen(argv[i+1])+1);
                commands = vec_add(commands, &buff);
            }
        }

        vector_metainfo meta = vec_meta(commands);
        for (int j=0; j<meta.length; j++) {
            memfree(commands[j]);
        }
        delete_vec(commands);
    } else {
        char** commands = new_vec(sizeof(char*), 10);
        char linebuffer[1024];
        char buffer[256];
        bool flag = false;
        int offset = 0;

        while (true) {
            fputs(">> ", stdout);
            fgets(linebuffer, 1024, stdin);
            while (offset < strlen(linebuffer)-1)
            {            
                sscanf(linebuffer + offset, "%s", buffer);
                // printf(buffer);
                offset += strlen(buffer)+1;
                if (strcmp(buffer, "QUIT")==0) {
                    flag=true;
                    break;
                }
                else if (buffer[0]=='~') {
                    execute(commands);
                    vector_metainfo meta = vec_meta(commands);
                    for (int j=0; j<meta.length; j++) {
                        memfree(commands[j]);
                    }
                    vec_metaptr(commands)->length=0;
                } else {
                    char* temp = memloc(strlen(buffer)+1);
                    memcpy(temp, buffer, strlen(buffer)+1);
                    commands = vec_add(commands, &temp);
                }
            }
            offset=0;
            if (flag) break;
        }

        vector_metainfo meta = vec_meta(commands);
        for (int j=0; j<meta.length; j++) {
            memfree(commands[j]);
        }
        delete_vec(commands);
    }


    // page_info(0);
    destroy_pages();

    return 0;
}