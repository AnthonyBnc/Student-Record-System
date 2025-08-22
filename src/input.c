#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "input.h"

static void trim_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n && (s[n-1]=='\n' || s[n-1]=='\r')) s[--n] = '\0';
}

void read_line(const char *prompt, char *buf, size_t len) {
    for (;;) {
        printf("%s", prompt);
        if (!fgets(buf, (int)len, stdin)) { clearerr(stdin); continue; }
        trim_newline(buf);
        if (buf[0] == '\0') { puts("Please enter a non-empty value."); continue; }
        return;
    }
}

int read_int_range(const char *prompt, int lo, int hi) {
    for (;;) {
        printf("%s", prompt);
        char line[128]; int x;
        if (!fgets(line, sizeof(line), stdin)) { clearerr(stdin); continue; }
        if (sscanf(line, "%d", &x) == 1 && x >= lo && x <= hi) return x;
        printf("Please enter an integer in [%d..%d].\n", lo, hi);
    }
}

float read_float_range(const char *prompt, float lo, float hi) {
    for (;;) {
        printf("%s", prompt);
        char line[128]; float x;
        if (!fgets(line, sizeof(line), stdin)) { clearerr(stdin); continue; }
        if (sscanf(line, "%f", &x) == 1 && x >= lo && x <= hi) return x;
        printf("Please enter a number in [%.2f..%.2f].\n", lo, hi);
    }
}

char read_gender(const char *prompt) {
    for (;;) {
        char line[32];
        read_line(prompt, line, sizeof(line));
        if (strlen(line) == 1) {
            char g = (char)toupper(line[0]);
            if (g=='M'||g=='F'||g=='O') return g;
        }
        puts("Enter gender as M, F, or O.");
    }
}
