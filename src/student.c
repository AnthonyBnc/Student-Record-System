#include <stdio.h>
#include <string.h>
#include "student.h"

void compute_average(Student *s) {
    if (!s || s -> nmarks <= 0) { if (s) s->average = 0.f; return;}
    float sum = 0.f;
    for (int i = 0; i < s->nmarks; i++) sum += s->marks[i];
    s -> average = sum / (float)s->nmarks;
} 

void print_student(const Student *s) {
    printf("ID:%d | %-20s | Age:%d | Gender:%c | Subjects:%d | Avg:%.2f\n",
        s->id, s->name, s->age, s->gender, s->nmarks, s->average);
}