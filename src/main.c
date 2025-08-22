#include <stdio.h>
#include "student.h"
#include "db.h"
#include "actions.h"
#include "input.h"
#include "menu.h"

int main(void) {
    StudentDB db;
    db_init(&db);
    if (!db_load(&db)) {
        puts("Warning: failed to load data. Starting with empty DB.");
        db.size = 0; db.next_id = 1;
    }

    for (;;) {
        show_menu();
        int choice = read_int_range("Choose (1-9): ", 1, 9);
        switch (choice) {
            case 1: action_add(&db); break;
            case 2: action_list(&db); break;
            case 3: action_search_id(&db); break;
            case 4: action_search_name(&db); break;
            case 5: action_update(&db); break;
            case 6: action_delete(&db); break;
            case 7: action_sort(&db); break;
            case 8:
                if (db_save(&db)) puts("Saved.");
                else puts("Save failed.");
                break;
            case 9:
                if (!db_save(&db)) puts("Warning: failed to save on exit.");
                db_free(&db);
                puts("Goodbye!");
                return 0;
        }
    }
}
