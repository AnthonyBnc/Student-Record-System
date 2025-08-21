#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

void  read_line(const char *prompt, char *buf, size_t len);
int   read_int_range(const char *prompt, int lo, int hi);
float read_float_range(const char *prompt, float lo, float hi);
char  read_gender(const char *prompt);

#endif // INPUT_H
