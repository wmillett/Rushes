#include "retromfa.h"


static bool check_extension(const char *filename, const char *extension) {
    if (strlen(filename) < strlen(extension)) return false;
    char position = strlen(filename) - strlen(extension);
    const char *dot = filename + position;
    if (dot[0] != '.') return false;
    if (strcmp(dot, extension) < 0) return false;
    return true;
}


static bool check_files(const char *filename) {

    if (!check_extension(filename, EXTENSION_MFA)) {
        fprintf(stderr, "Error: File %s does not have .mfa extension\n", filename);
        return false;
    }
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error: Failed to open file %s\n", filename);
        return false;
    }
    char buffer[5] = {0};
	if(!read(fd, buffer, 4))
    {
        fprintf(stderr, "Error: Failed to read file %s\n", filename);
        close(fd);
        return false;
    }
    // Check if the file starts with "MMF2"
	if (strcmp(buffer, "MMF2") != 0)
	{
        fprintf(stderr, "Error: File %s is not a valid MFA file\n", filename);
		close(fd);
		return false;
	}
    close(fd);
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
        if (!check_files(argv[i])) {
            //fprintf(stderr, "Error: File %s does not have .mfa extension\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    // Setup mlx

    t_mfa *mfa = list_malloc(1, sizeof(t_mfa));
    if (!mfa) {
        fprintf(stderr, "Error: Failed to allocate memory for t_mfa\n");
        return EXIT_FAILURE;
    }
    
    mfa->mlx_ptr = mlx_init();
    if (!mfa->mlx_ptr) {
        fprintf(stderr, "Error: Failed to initialize MLX\n");
        free(mfa);
        return EXIT_FAILURE;
    }
    mfa->win_ptr = mlx_new_window(mfa->mlx_ptr, 800, 600, "RetroMFA Viewer");
    if (!mfa->win_ptr) {
        fprintf(stderr, "Error: Failed to create MLX window\n");
        exit_mlx(mfa);
        free(mfa);
        return EXIT_FAILURE;
    }
    

    mlx_key_hook(mfa->win_ptr, key_hook, mfa);
    mlx_loop(mfa->mlx_ptr);

    for (int i = 0; i < mfa->img_count; i++) {
        mlx_destroy_image(mfa->mlx_ptr, mfa->img_list[i].img);
        // if (img->data) {
        //     free(img->data);
        //     img->data = NULL;
        // }
    }

    exit_mlx(mfa);
    all_free(); // Free all allocated memory

    // // Process each MFA file
    // printf("Processing %d MFA file(s)...\n", argc - 1);
    // for (int i = 1; i < argc; i++) {
    //     printf("\n=== Processing file: %s ===\n", argv[i]);
    //     if (!parse_mfa_file(argv[i])) {
    //         fprintf(stderr, "Failed to process %s\n", argv[i]);
    //         return EXIT_FAILURE;
    //     }
    // }

    printf("\nAll files processed successfully!\n");
    return EXIT_SUCCESS;
}
