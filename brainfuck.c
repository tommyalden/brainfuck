#include <stdlib.h>
#include <stdio.h>

#define TAPE_SIZE 32768

void processInstruction(unsigned char **pointer, char **instruction) {
    switch(**instruction) {
        case '>': (*pointer)++; break; // Increment tape pointer
        case '<': (*pointer)--; break; // Decrement tape pointer
        case '+': (**pointer)++; break; // Increment tape pointer value
        case '-': (**pointer)--; break; // Decrement tape pointer value
        case '.': putc(**pointer, stdout); break; // Put tape pointer value into stdout
        case ',': **pointer = getc(stdin); break; // Set tape pointer value from stdin
        case '[': // Branch forward if tape pointer value is 0
            if (**pointer == 0 ) {
                int nesting = 1;
                while (nesting > 0) {
                    (*instruction)++;
                    if (**instruction == '\0') return; // Memory safety
                    if (**instruction == '[') nesting++;
                    if (**instruction == ']') nesting--;
                }
            }

            break;
        case ']': // Branch backward if tape pointer value is not zero
            if (**pointer != 0) {
                int nesting = 1;
                while (nesting > 0) {
                    (*instruction)--;
                    if (**instruction == '\0') return;
                    if (**instruction == ']') nesting++;
                    if (**instruction == '[') nesting--;
                }
            }

            break;
        default: break;
    }
}

int loadFile(const char *filename, char **program) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return -1;

    // Find the length of the program
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    *program = malloc(length + 1);
    if (*program == NULL) return -1;

    // Load it into memory
    fread(*program, 1, length, file);
    (*program)[length] = '\0';

    fclose(file);

    return 0;
}

int loadStdin(char **program) {
    size_t capacity = 1024;
    size_t length = 0;
    
    char *buffer = malloc(capacity);
    if (!buffer) return -1;

    char c = 0;
    while (c != EOF) {
        c = getchar();

        if (length + 1 >= capacity) {
            capacity *= 2;
            char *newBuffer = realloc(buffer, capacity);

            if(!newBuffer) {
                free(buffer);
                return -1;
            }

            buffer = newBuffer;
        }

        buffer[length++] = c;
    }

    buffer[length]= '\0';
    *program = buffer;

    return 0;
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s <file.bf>\n", argv[0]);
        return 0;
    }

    char *program = NULL;

    if (argc == 2) {
        // If -1, loadFile failed
        if(loadFile(argv[1], &program) == -1) {
            perror("Error loading file");
            return 1;
        }
    } else {
        if(loadStdin(&program) == -1) {
            perror("Error reading from stdin\n");
            return 1;
        }
    }

    if (!program) return 1;

    // Initialise tape to zeroes, alongside the other pointers
    unsigned char tape[TAPE_SIZE] = { 0 };
    unsigned char *pointer;
    char *pc;

    pointer = &tape[0];
    pc = program;

    while (*pc != '\0') {
        processInstruction(&pointer, &pc);
        pc++;
    }

    free(program);
    return 0;
}
