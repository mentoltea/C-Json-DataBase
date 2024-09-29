#include "bd.h"

char* currname = NULL;
Table database = NULL;
Table table = NULL;

json_object* find_object_by_name(Table table, char* name) {
    if (!validate_table()) return NULL;
    vector_metainfo meta = vec_meta(table->fields);
    for (int i=0; i<meta.length; i++) {
        if (strcmp(table->fields[i].key, name)==0) {
            return &table->fields[i].value;
        }
    }
    fprintf(stderr, "No object named found %s in table!\n", name);
    return NULL;
}
int get_idx_by_name(Table table, char* name) {
    if (!validate_table()) return -1;
    vector_metainfo meta = vec_meta(table->fields);
    for (int i=0; i<meta.length; i++) {
        if (strcmp(table->fields[i].key, name)==0) {
            return i;
        }
    }
    fprintf(stderr, "No object named found %s in table!\n", name);
    return -1;
}



bool validate_table() {
    if (!database) {
        fprintf(stderr, "Choose the DB first!: load_table(\"name\")\n");
    }
    if (!table) {
        fprintf(stderr, "Choose the table first!:choose_table(\"name\")\n");
    }
    return database&&table;
}

int get_int(char* name) {
    json_object* obj = find_object_by_name(table, name);
    if (!obj) return 0;

    if (obj->type != INT) {
        fprintf(stderr, "Object %s is not INT (%d)!\n", name, obj->type);
        return 0;
    }

    return obj->data.num; 
}
float get_float(char* name) {
    json_object* obj = find_object_by_name(table, name);
    if (!obj) return 0;

    if (obj->type != FLOAT) {
        fprintf(stderr, "Object %s is not FLOAT (%d)!\n", name, obj->type);
        return 0;
    }

    return obj->data.dec; 
}
char* get_string(char* name) {
    json_object* obj = find_object_by_name(table, name);
    if (!obj) return NULL;

    if (obj->type != STR) {
        fprintf(stderr, "Object %s is not STR (%d)!\n", name, obj->type);
        return NULL;
    }

    return obj->data.str; 
}


void add_table(char* name) {
    if (!validate_table()) return;
    json_pair realpair;
    json_pair *pair = &realpair;

    pair->key = memloc(sizeof(char)*(strlen(name) + 1));
    memcpy(pair->key, name, strlen(name)+1);
    
    pair->value.type = CHILD;
    pair->value.data.child.fields = new_vec(sizeof(json_pair), 5);

    table->fields = vec_add(table->fields, pair);
}
void add_int(char* name, int value) {
    if (!validate_table()) return;
    json_pair realpair;
    json_pair *pair = &realpair;
    
    pair->key = memloc(sizeof(char)*(strlen(name) + 1));
    memcpy(pair->key, name, strlen(name)+1);
    
    pair->value.type = INT;
    pair->value.data.num = value;

    table->fields = vec_add(table->fields, pair);
}
void add_float(char* name, float value) {
    if (!validate_table()) return;
    json_pair realpair;
    json_pair *pair = &realpair;
    
    pair->key = memloc(sizeof(char)*(strlen(name) + 1));
    memcpy(pair->key, name, strlen(name)+1);
    
    pair->value.type = FLOAT;
    pair->value.data.dec = value;

    table->fields = vec_add(table->fields, pair);
}
void add_string(char* name, char *str) {
    if (!validate_table()) return;
    json_pair realpair;
    json_pair *pair = &realpair;
    
    pair->key = memloc(sizeof(char)*(strlen(name) + 1));
    memcpy(pair->key, name, strlen(name)+1);
    
    pair->value.type = STR;
    pair->value.data.str = memloc(sizeof(char)*(strlen(str) + 1));
    memcpy(pair->value.data.str, str, strlen(str) + 1);

    table->fields = vec_add(table->fields, pair);
}

void change_int(char* name, int value) {
    json_object* obj = find_object_by_name(table, name);
    if (!obj) return;

    if (obj->type != INT) {
        fprintf(stderr, "Object %s is not INT (%d)!\n", name, obj->type);
        return;
    }

    obj->data.num = value;
}
void change_float(char* name, float value) {
    json_object* obj = find_object_by_name(table, name);
    if (!obj) return;

    if (obj->type != FLOAT) {
        fprintf(stderr, "Object %s is not FLOAT (%d)!\n", name, obj->type);
        return;
    }

    obj->data.dec = value;
}
void change_string(char* name, char *str) {
    json_object* obj = find_object_by_name(table, name);
    if (!obj) return;

    if (obj->type != STR) {
        fprintf(stderr, "Object %s is not STR (%d)!\n", name, obj->type);
        return;
    }

    memfree(obj->data.str);
    obj->data.str = memloc(sizeof(char)*(strlen(str) + 1));
    memcpy(obj->data.str, str, strlen(str) + 1);
}

void delete(char* name) {
    int idx = get_idx_by_name(table, name);
    if (idx<0) return;
    dealloc_json_pair(table->fields + idx);
    vec_remove(table->fields, idx);
}

void load_table(char* filename) {
    if (database) {
        save_table(currname);
        unload_table();
    }

    FILE* fd = fopen(filename, "r");
    if (!fd) {
        fprintf(stderr, "Error open file %s!\n", filename);
        return;
    }
    database = memloc(sizeof(Table));
    *database = read_json(fd);
    fclose(fd);

    table = database;
    currname = memloc(sizeof(char)*(strlen(filename)+1));
    memcpy(currname, filename, strlen(filename)+1);
}
void save_table(char* filename) {
    if (!validate_table()) return;
    if (filename==NULL) filename = currname;
    FILE* fd = fopen(filename, "w");
    if (!fd) {
        fprintf(stderr, "Error open file %s!\n", filename);
        return;
    }
    save_json(fd, database);
    fclose(fd);
}
void unload_table() {
    if (!validate_table()) return;
    dealloc_json(database);
    memfree(database);
    database = NULL;
    table = NULL;
    memfree(currname);
    currname = NULL;
}
void choose_table(char* name) {
    json_object* obj = find_object_by_name(table, name);
    if (!obj) return;
    if (obj->type != CHILD) {
        fprintf(stderr, "Object %s is not a table!\n", name);
        return;
    }
    table = &obj->data.child;
}

void reset_table() {
    if (!validate_table) return;
    table = database;
}