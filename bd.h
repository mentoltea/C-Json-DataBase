#ifndef s7k_BD
#define s7k_BD

#include "json.h"
#include <stdbool.h>

typedef json_child* Table;

extern Table database;
extern Table table;
extern char* currname;

bool validate_table();

json_object* find_object_by_name(Table table, char* name);
int get_idx_by_name(Table table, char* name);

void add_table(char* name);
void add_int(char* name, int value);
void add_float(char* name, float value);
void add_string(char* name, char *str);

int get_int(char* name);
float get_float(char* name);
char* get_string(char* name);

void change_int(char* name, int value);
void change_float(char* name, float value);
void change_string(char* name, char *str);

void delete(char* name);

void load_table(char* filename);
void save_table(char* filename);
void unload_table();
void choose_table(char* name);
void reset_table();


#endif