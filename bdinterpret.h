#ifndef s7k_BDINT
#define s7k_BDINT

#include "bd.h"

void execute_add(char** command);
void execute_get(char** command);
void execute_change(char** command);
void execute_delete(char** command);
void execute_load(char** command);
void execute_save(char** command);
void execute_unload(char** command);
void execute_choose(char** command);
void execute_reset(char** command);

// @param argv CVector of words
void execute(char** command);

#endif