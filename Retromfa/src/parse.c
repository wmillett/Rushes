#include "retromfa.h"

static bool check_extension(const char *filename, const char *extension) {
    if (strlen(filename) < strlen(extension)) return false; // Filename too short for extension
    char position = strlen(filename) - strlen(extension);
    const char *dot = filename + position;
    if (dot[0] != '.') return false; // Ensure there's a dot before the extension
    if (strcmp(dot, extension) < 0) return false; // Ensure the extension matches exactly
    return true;
}

 
bool parse_mfa_file(const char *filename) {

    if(check_extension(filename, ".mfa")) {
        printf("Parsing MFA file: %s\n", filename);
    } else {
        fprintf(stderr, "Error: File %s does not have a .mfa extension.\n", filename);
        return -1;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    // Example parsing logic (to be replaced with actual parsing)
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("Read line: %s", line);
    }

    fclose(file);
    return 0;
}
