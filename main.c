#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    printf("Welcome to Geyser REPL! Type 'exit' to quit\n");
    while (true) {
        printf("gy> ");
        size_t capacity = 128;
        size_t length = 0;
        char *input = malloc(capacity);
        if (input == nullptr) return 1;
        int ch;
        while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
            if (length + 1 >= capacity) {
                capacity *= 2;
                char *temp = realloc(input, capacity);
                if (temp == nullptr) {
                    free(input);
                    return 1;
                }
                input = temp;
            }
            input[length++] = (char)ch;
        }
        input[length] = '\0';
        if (length > 0 && input[length - 1] == '\r') {
            input[--length] = '\0';
        }
        if (strcmp(input, "exit") == 0) {
            printf("Bye!\n");
            free(input);
            break;
        }
        // REPLACE THIS
        if (length > 0) {
            printf("%s\n", input);
        }
        free(input);
    }
    return 0;
}
