#include "bdinterpret.h"

void execute_add(char** argv) {
    vector_metainfo meta = vec_meta(argv);
    if (meta.length < 2) {
        fprintf(stderr, "Too few arguments!\n");
        fprintf(stderr, "ADD <TYPE> <NAME> [<VALUE>]\n");
        return;
    } else if (strcmp(argv[1], "TABLE") == 0) {
        if (meta.length<3) {
            fprintf(stderr, "Too few arguments!\n");
            fprintf(stderr, "ADD TABLE <NAME>\n");
            return;
        }
        add_table(argv[2]);
    } else if (strcmp(argv[1], "INT") == 0) {
        if (meta.length<4) {
            fprintf(stderr, "Too few arguments!\n");
            fprintf(stderr, "ADD INT <NAME> <VALUE>\n");
            return;
        }
        int value = atoi(argv[3]);
        add_int(argv[2], value);
    } else if (strcmp(argv[1], "FLOAT") == 0) {
        if (meta.length<4) {
            fprintf(stderr, "Too few arguments!\n");
            fprintf(stderr, "ADD FLOAT <NAME> <VALUE>\n");
            return;
        }
        float value = atof(argv[3]);
        add_float(argv[2], value);
    } else if (strcmp(argv[1], "STRING") == 0) {
        if (meta.length<4) {
            fprintf(stderr, "Too few arguments!\n");
            fprintf(stderr, "ADD STRING <NAME> <VALUE>\n");
            return;
        }
        add_string(argv[2], argv[3]);
    } else {
        fprintf(stderr, "Cannot figure type %s\n", argv[1]);
    }
}

void execute_get(char** argv) {
    vector_metainfo meta = vec_meta(argv);
    
    if (meta.length < 3) {
        fprintf(stderr, "Too few arguments!\n");
        fprintf(stderr, "GET <TYPE> <NAME>\n");
        return;
    } else if (0 == strcmp(argv[1], "INT")) {
        int value = get_int(argv[2]);
        fprintf(stdout, "%d\n", value);
    } else if (0 == strcmp(argv[1], "FLOAT")) {
        float value = get_float(argv[2]);
        fprintf(stdout, "%f\n", value);
    } else if (0 == strcmp(argv[1], "STRING")) {
        char* value = get_string(argv[2]);
        fprintf(stdout, "%s\n", value);
    } else {
        fprintf(stderr, "Cannot figure type %s\n", argv[1]);
    }
}

void execute_change(char** argv) {
    vector_metainfo meta = vec_meta(argv);
    if (meta.length < 4) {
        fprintf(stderr, "Too few arguments!\n");
        fprintf(stderr, "CHANGE <TYPE> <NAME> <NEW VALUE>\n");
        return;
    } else if (0 == strcmp(argv[1], "INT")) {
        int value = atoi(argv[3]);
        change_int(argv[2], value);
    } else if (0 == strcmp(argv[1], "FLOAT")) {
        float value = atof(argv[3]);
        change_float(argv[2], value);
    } else if (0 == strcmp(argv[1], "STRING")) {
        change_string(argv[2], argv[3]);
    } else {
        fprintf(stderr, "Cannot figure type %s\n", argv[1]);
    }
}

void execute_delete(char** argv) {
    vector_metainfo meta = vec_meta(argv);
    if (meta.length < 2) {
        fprintf(stderr, "Too few arguments!\n");
        fprintf(stderr, "DELETE <NAME>\n");
        return;
    }
    delete(argv[1]);
}

void execute_load(char** argv) {
    vector_metainfo meta = vec_meta(argv);
    if (meta.length < 2) {
        fprintf(stderr, "Too few arguments!\n");
        fprintf(stderr, "LOAD <NAME>\n");
        return;
    }
    load_table(argv[1]);
}

void execute_save(char** argv) {
    vector_metainfo meta = vec_meta(argv);
    if (meta.length < 2) {
        save_table(0);
    } else {
        save_table(argv[1]);
    }
}

void execute_unload(char** argv) {
    unload_table();
}

void execute_choose(char** argv) {
    vector_metainfo meta = vec_meta(argv);
    if (meta.length < 2) {
        fprintf(stderr, "Too few arguments!\n");
        fprintf(stderr, "CHOOSE <NAME>\n");
        return;
    }
    choose_table(argv[1]);
}

void execute_reset(char** argv) {
    reset_table();
}

void execute(char** argv) {
    vector_metainfo meta = vec_meta(argv);
    // printf(argv[0]);
    if (meta.length==0) {
        fprintf(stderr, "No command given!\n");
        return;
    } else if (strcmp(argv[0], "ADD") == 0) {
        execute_add(argv);
    } else if (strcmp(argv[0], "GET") == 0) {
        execute_get(argv);
    } else if (strcmp(argv[0], "CHANGE") == 0) {
        execute_change(argv);
    } else if (strcmp(argv[0], "DELETE") == 0) {
        execute_delete(argv);
    } else if (strcmp(argv[0], "LOAD") == 0) {
        execute_load(argv);
    } else if (strcmp(argv[0], "SAVE") == 0) {
        execute_save(argv);
    } else if (strcmp(argv[0], "UNLOAD") == 0) {
        execute_unload(argv);
    } else if (strcmp(argv[0], "CHOOSE") == 0) {
        execute_choose(argv);
    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[0]);
        return;
    }
}