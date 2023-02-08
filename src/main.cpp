#include <cstdio>

int main() {
    FILE *file = fopen("assets/test.txt", "rb");
    if (!file) {
        printf("cannot open file\n");
        return 1;
    }
    while (!feof(file)) {
        char c = fgetc(file);
        if (c != EOF) {
            putchar(c);
        }
    }
    fclose (file);
    return 0;
}

