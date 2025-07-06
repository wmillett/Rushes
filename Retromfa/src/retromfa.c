#include "retromfa.h"


static char *append_to_buffer(char *original, size_t original_size,
                              const char *chunk, size_t chunk_size)
{
    char *new_buffer = list_malloc(original_size + chunk_size, sizeof(char));
    if (!new_buffer)
    {   fprintf(stderr, "Error: Memory allocation failed while appending to buffer.\n");
        return NULL;
    }
    if (original)
        memcpy(new_buffer, original, original_size);
    memcpy(new_buffer + original_size, chunk, chunk_size);
    return new_buffer;
}


static bool read_entire_file(const char *filename, t_mfa *input)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        input->content = NULL;
        input->size = 0;
        return false;
    }
    char buffer[BUFFER_SIZE];
    char *file_content = NULL;
    size_t total_size = 0;
    ssize_t bytes_read = 0;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        file_content = append_to_buffer(file_content, total_size, buffer, bytes_read);
        if (!file_content)
        {
            close(fd);
            fprintf(stderr, "Error: memory allocation failed while reading file.\n");
            input->content = NULL;
            input->size = 0;
            return false;
        }
        total_size += bytes_read;
    }
    if (bytes_read < 0){
        perror("Error reading file");
        input->content = NULL;
        input->size = 0;
        close(fd);
        return false;
    }
    close(fd);
    input->content = file_content;
    input->size = total_size;
    return true;
}




bool find_images(t_mfa *mfa, const char *filename) {

    (void)mfa; // Suppress unused parameter warning
    (void)filename; // Suppress unused parameter warning

    unsigned char *buffer = list_malloc(1, sizeof(unsigned char) * 1024);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return false;
    }

    if(read_entire_file(filename, mfa) == false   )
    {
        return false;
    }
    printf("File %s read successfully, size: %zu bytes\n", filename, mfa->size);

    // This function is a placeholder for finding an image in the MFA structure.
    // It should be implemented to search through the mfa->img_list for the specified filename.
    // For now, it returns -1 to indicate that the image was not found.
    return true;
}
