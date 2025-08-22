#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "actions.h"
#include "db.h"
#include "input.h"

static void to_lowercase(char *s) { for (; *s; ++s) *s = (char)tolower(*s); }

void action_add(StudentDB *db) {
    Student s = {0};
    char name[NAME_LEN];
    read_line("Enter name: ", name, sizeof(name));
    strncpy(s.name, name, NAME_LEN-1); s.name[NAME_LEN-1] = '\0';
    s.age = read_int_range("Enter age (5..120): ", 5, 120);
    s.gender = read_gender("Enter gender (M/F/O): ");
    s.nmarks = read_int_range("How many subjects (1..10)? ", 1, MAX_SUBJ);
    for (int i = 0; i < s.nmarks; ++i) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "  Mark %d (0..100): ", i+1);
        s.marks[i] = read_float_range(prompt, 0.f, 100.f);
    }
    int id = db_add(db, &s);
    if (id < 0) { puts("Failed to add (memory)."); return; }
    if (!db_save(db)) puts("Warning: failed to save data.");
    printf("Student added with ID %d.\n", id);
}

void action_list(const StudentDB *db) {
    if (db->size == 0) { puts("No students yet."); return; }
    puts("---- All Students ----");
    for (size_t i = 0; i < db->size; ++i) print_student(&db->data[i]);
    puts("----------------------");
}

void action_search_id(const StudentDB *db) {
    int id = read_int_range("Enter ID to search: ", 1, 1<<30);
    int idx = db_find_index_by_id(db, id);
    if (idx < 0) puts("Not found.");
    else print_student(&db->data[idx]);
}

void action_search_name(const StudentDB *db) {
    char key[NAME_LEN];
    read_line("Enter name fragment: ", key, sizeof(key));
    char lc[NAME_LEN]; strncpy(lc, key, sizeof(lc)); lc[NAME_LEN-1]='\0'; to_lowercase(lc);

    int found = 0;
    for (size_t i = 0; i < db->size; ++i) {
        char buf[NAME_LEN]; strncpy(buf, db->data[i].name, sizeof(buf)); buf[NAME_LEN-1]='\0'; to_lowercase(buf);
        if (strstr(buf, lc)) { print_student(&db->data[i]); found = 1; }
    }
    if (!found) puts("No matches.");
}

void action_update(StudentDB *db) {
    int id = read_int_range("Enter ID to update: ", 1, 1<<30);
    int idx = db_find_index_by_id(db, id);
    if (idx < 0) { puts("Not found."); return; }

    Student s = db->data[idx]; // start from current
    char line[128];

    printf("New name (leave blank to keep '%s'): ", s.name);
    if (fgets(line, sizeof(line), stdin)) {
        size_t n = strlen(line); while (n && (line[n-1]=='\n'||line[n-1]=='\r')) line[--n]='\0';
        if (n > 0) { strncpy(s.name, line, NAME_LEN-1); s.name[NAME_LEN-1]='\0'; }
    } else clearerr(stdin);

    printf("New age (current %d, or 0 to keep): ", s.age);
    if (fgets(line, sizeof(line), stdin)) {
        int a=0; if (sscanf(line, "%d", &a)==1 && a>=5 && a<=120) s.age = a;
    } else clearerr(stdin);

    printf("New gender (M/F/O) (current %c, blank to keep): ", s.gender);
    if (fgets(line, sizeof(line), stdin)) {
        size_t n=strlen(line); while (n && (line[n-1]=='\n'||line[n-1]=='\r')) line[--n]='\0';
        if (n==1) {
            char g=(char)toupper(line[0]);
            if (g=='M'||g=='F'||g=='O') s.gender=g;
        }
    } else clearerr(stdin);

    printf("Change marks? (y/N): ");
    if (fgets(line, sizeof(line), stdin)) {
        if (tolower(line[0])=='y') {
            s.nmarks = read_int_range("How many subjects (1..10)? ", 1, MAX_SUBJ);
            for (int i = 0; i < s.nmarks; ++i) {
                char prompt[64];
                snprintf(prompt, sizeof(prompt), "  Mark %d (0..100): ", i+1);
                s.marks[i] = read_float_range(prompt, 0.f, 100.f);
            }
        }
    } else clearerr(stdin);

    if (!db_update(db, idx, &s)) { puts("Update failed."); return; }
    if (!db_save(db)) puts("Warning: failed to save data.");
    puts("Record updated.");
}

void action_delete(StudentDB *db) {
    int id = read_int_range("Enter ID to delete: ", 1, 1<<30);
    int idx = db_find_index_by_id(db, id);
    if (idx < 0) { puts("Not found."); return; }
    if (!db_delete_by_index(db, idx)) { puts("Delete failed."); return; }
    if (!db_save(db)) puts("Warning: failed to save data.");
    puts("Deleted.");
}

void action_sort(StudentDB *db) {
    puts("Sort by:\n  1) Name ASC\n  2) Name DESC\n  3) Average ASC\n  4) Average DESC");
    int c = read_int_range("Choose (1-4): ", 1, 4);
    db_sort(db, (SortOrder)c);
    if (!db_save(db)) puts("Warning: failed to save data.");
    puts("Sorted.");
}
