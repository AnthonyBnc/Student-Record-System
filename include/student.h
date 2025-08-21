#ifndef STUDENT_H
#define STUDENT_H

#include <stddef.h>

#define NAME_LEN 64
#define MAX_SUBJ 10

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    char gender;
    int nmarks;
    float marks[MAX_SUBJ];
    float average;
} Student;

typedef struct {
    Student *data;
    size_t size;
    size_t cap;
    int next_id;
} StudentDB;

void compute_average(Student *s);
void print_student(const Student *s);

#endif