#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "db.h"

#define DATA_FILE "students.dat"
#define INIT_CAP 16

static int db_reserve(StudentDB *db, size_t need) {
    if (need <= db -> cap) return 1;
    size_t new_cap = db -> cap ? db -> cap : INIT_CAP;
    while (new_cap < need) new_cap *= 2;
    Student *p = (Student *)realloc(db->data, new_cap * sizeof(Student));
    if (!p) return 0;
    db -> data = p;
    db -> cap = new_cap;
    return 1;
}

void db_init(StudentDB *db) {
    db -> data = (Student *)malloc(INIT_CAP * sizeof(Student));
    db -> size = 0;
    db -> cap = db -> data ? INIT_CAP : 0;
    db -> next_id = 1;
}

void db_free(StudentDB *db) {
    free(db -> data);
    db -> data = NULL;
    db -> size = 0;
    db -> cap = 0;
    db -> next_id = 1;
}

int db_save(const StudentDB *db) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (!fp) { perror("fopen"); return 0;}
    if (fwrite(&db->next_id, sizeof(db->next_id), 1, fp) != 1) { fclose(fp); return 0;}
    if (fwrite(&db -> size, sizeof(db -> size), 1, fp) != 1) { fclose(fp); return 0;}
    if (db -> size > 0) {
        if (fwrite(db -> data, sizeof(Student), db -> size, fp) != db->size) {
            fclose(fp); return 0;
        }
    }
    fclose(fp);
    return 1;
}

int db_load(StudentDB *db) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return 1; // first run
    int next_id = 1; size_t sz = 0;
    if (fread(&next_id, sizeof(next_id), 1, fp) != 1) { fclose(fp); return 0; }
    if (fread(&sz, sizeof(sz), 1, fp) != 1) { fclose(fp); return 0; }
    if (!db_reserve(db, sz)) { fclose(fp); return 0; }
    if (sz > 0) {
        if (fread(db->data, sizeof(Student), sz, fp) != sz) { fclose(fp); return 0; }
    }
    db->size = sz; db->next_id = next_id;
    fclose(fp);
    return 1;
}

int db_add(StudentDB *db, const Student *s_template) {
    if (!db_reserve(db, db->size + 1)) return -1;
    Student s = *s_template;
    s.id = db->next_id++;
    compute_average(&s);
    db->data[db->size++] = s;
    if (db_save(db)) return db->size - 1;
    return -1;
}

int db_find_index_by_id(const StudentDB *db, int id) {
    for (size_t i = 0; i < db->size; ++i) if (db->data[i].id == id) return (int)i;
    return -1;
}

int db_delete_by_index(StudentDB *db, int idx) {
    if (idx < 0 || (size_t)idx >= db->size) return 0;
    db->data[idx] = db->data[db->size - 1];
    db->size--;
    return 1;
}

int db_update(StudentDB *db, int idx, const Student *s) {
    if (idx < 0 || (size_t)idx >= db->size) return 0;
    Student copy = *s;
    compute_average(&copy);
    copy.id = db->data[idx].id; // keep same ID
    db->data[idx] = copy;
    return 1;
}

/* Sorting */
static int cmp_name_asc(const void *a, const void *b) {
    const Student *x = (const Student *)a;
    const Student *y = (const Student *)b;
    return strcasecmp(x->name, y->name);
}
static int cmp_name_desc(const void *a, const void *b) { return -cmp_name_asc(a,b); }

static int cmp_avg_asc(const void *a, const void *b) {
    const Student *x = (const Student *)a;
    const Student *y = (const Student *)b;
    if (x->average < y->average) return -1;
    if (x->average > y->average) return  1;
    return 0;
}
static int cmp_avg_desc(const void *a, const void *b) { return -cmp_avg_asc(a,b); }

void db_sort(StudentDB *db, SortOrder order) {
    if (db->size <= 1) return;
    switch (order) {
        case SORT_NAME_ASC:  qsort(db->data, db->size, sizeof(Student), cmp_name_asc);  break;
        case SORT_NAME_DESC: qsort(db->data, db->size, sizeof(Student), cmp_name_desc); break;
        case SORT_AVG_ASC:   qsort(db->data, db->size, sizeof(Student), cmp_avg_asc);   break;
        case SORT_AVG_DESC:  qsort(db->data, db->size, sizeof(Student), cmp_avg_desc);  break;
        default: break;
    }
}