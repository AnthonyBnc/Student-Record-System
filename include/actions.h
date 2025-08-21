#ifndef ACTIONS_H
#define ACTIONS_H

#include "student.h"

void action_add(StudentDB *db);
void action_list(const StudentDB *db);
void action_search_id(const StudentDB *db);
void action_search_name(const StudentDB *db);
void action_update(StudentDB *db);
void action_delete(StudentDB *db);
void action_sort(StudentDB *db);

#endif