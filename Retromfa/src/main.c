#include "retromfa.h"


static bool check_extension(const char *filename, const char *extension) {
    if (strlen(filename) < strlen(extension)) return false;
    char position = strlen(filename) - strlen(extension);
    const char *dot = filename + position;
    if (dot[0] != '.') return false;
    if (strcmp(dot, extension) < 0) return false;
    return true;
}



int main(int argc, char *argv[]) {
    printf("RetroMFA - MFA File Image Extractor\n");
    printf("====================================\n\n");
    

    if (argc < 2) {
        printf("Usage: ./retromfa <mfa_file1> <mfa_file2> ...\n");
            return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        if (!check_extension(argv[i], EXTENSION_MFA)) {
            fprintf(stderr, "Error: File %s does not have .mfa extension\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    // Setup mlx
    t_mfa mfa;
    mfa.mlx_ptr = mlx_init();
    if (!mfa.mlx_ptr) {
        fprintf(stderr, "Error: Failed to initialize MLX\n");
        return EXIT_FAILURE;
    }
    mfa.win_ptr = mlx_new_window(mfa.mlx_ptr, 800, 600, "RetroMFA Viewer");
    if (!mfa.win_ptr) {
        fprintf(stderr, "Error: Failed to create MLX window\n");
        //mlx_destroy_display(mfa.mlx_ptr);
        return EXIT_FAILURE;
    }
    


    // Process each MFA file
    printf("Processing %d MFA file(s)...\n", argc - 1);
    for (int i = 1; i < argc; i++) {
        printf("\n=== Processing file: %s ===\n", argv[i]);
        if (!parse_mfa_file(argv[i])) {
            fprintf(stderr, "Failed to process %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    printf("\nAll files processed successfully!\n");
    return EXIT_SUCCESS;
}
