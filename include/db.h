#ifndef DB_H
#define DB_H

#include "student.h"

typedef enum {
    SORT_NAME_ASC = 1,
    SORT_NAME_DESC,
    SORT_AVG_ASC,
    SORT_AVG_DESC,
} SortOrder;

void db_init(StudentDB *db);
void db_free(StudentDB *db);
void db_load(StudentDB *db);
int db_save(const StudentDB *db);

int db_add(StudentDB *db, const Student *s_template);
int db_find_index_by_id (const StudentDB *db, int id);
int db_delete_by_index(StudentDB *db, int id);
int db_update(StudentDB *db, int idx, const Student *s);

void db_sort(StudentDB *db, SortOrder order);

#endif