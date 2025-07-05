#include "retromfa.h"






int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <MFA files>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Process each MFA file provided as an argument
    for (int i = 1; i < argc; i++) {
        printf("Processing file: %s\n", argv[i]);
        parse_mfa_file(argv[i]);
        // Here you would add the logic to handle each MFA file
        // For now, we just print the filename
    }

    return EXIT_SUCCESS;
}
