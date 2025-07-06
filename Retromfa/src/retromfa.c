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



static bool go_through_file(t_mfa *mfa, FILE *file){
    //int16_t header[] = {0x4D, 0x4D, 0x46, 0x32}; // MMF2 header
    //int16_t header16[] = {0x06, 0x10, 0x0, 0x0}; // MMF2 header for 16-bit images
    //int16_t header24[] = {0x04, 0x10, 0x0, 0x0}; // MMF2 header for 24-bit images

    unsigned char *buffer = malloc(1, sizeof(unsigned char) * 1024);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return false;
    }

    unsigned int file_offset = 0;
    while(!feof(file)) {
        size_t bytes_read = fread(buffer, sizeof(unsigned char), 1024, file);
        if (ferror(file)) {
            perror("Error reading file");
            free(buffer);
            return false;
        }
        printf("Read %zu bytes from file\n", bytes_read);

        for (int i = 0; i < bytes_read; i++) {
            int is_image = 0;
            if(buffer[i] == HEADER16_FLAG) {
                // Process 16-bit image
                is_image = FOUND_16;
                mfa->img_count++;
                printf("Found 16-bit image header at byte %d\n", i);
                // You can add code to handle the 16-bit image here
            } else if(buffer[i] == HEADER24_FLAG) {
                // Process 24-bit image
                is_image = FOUND_24;
                mfa->img_count++;
                printf("Found 24-bit image header at byte %d\n", i);
                // You can add code to handle the 24-bit image here
            }
            else{
                continue;
            }
            if (is_image){
                fseek(file, file_offset + i - IMAGE_SIZE_OFFSET, SEEK_SET); // Move the file pointer to the next byte after the header
                fread(buffer, sizeof(unsigned char), 4, file);
                if (ferror(file)) {
                    perror("Error reading file after header");
                    free(buffer);
                    return false;
                }
                // Convert little-endian size to 16-bit integer
                int16_t width = (buffer[0] | (buffer[1] << 8));
                int16_t height = (buffer[2] | (buffer[3] << 8));
                printf("Image size: %d x %d\n", width, height);
                if (width > 0 && height > 0) {
             
                }
                else {
                    printf("Invalid image size: %d x %d at byte %d\n", width, height, i);
                }
                // Move the file pointer to the pixel data
                fseek(file, file_offset + FILE_CHUNK_OFFSET, SEEK_SET);
            }
            // Example processing: just print the byte value
            printf("%02x ", buffer[i]);
        }
    }
    free(buffer);
    fclose(file);
}


static bool read_entire_file(const char *filename, t_mfa *input)
{
    FILE *file = fopen(filename, READ_BINARY);
    if (!file)
    {
        perror("Error opening file");
        input->content = NULL;
        input->size = 0;
        return false;
    }
   
    if(!go_through_file(input, file)){
        fclose(file);
        return false;
    };



    // char buffer[BUFFER_SIZE];
    // char *file_content = NULL;
    // size_t total_size = 0;
    // ssize_t bytes_read = 0;
    // while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    // {
    //     file_content = append_to_buffer(file_content, total_size, buffer, bytes_read);
    //     if (!file_content)
    //     {
    //         close(fd);
    //         fprintf(stderr, "Error: memory allocation failed while reading file.\n");
    //         input->content = NULL;
    //         input->size = 0;
    //         return false;
    //     }
    //     total_size += bytes_read;
    // }
    // if (bytes_read < 0){
    //     perror("Error reading file");
    //     input->content = NULL;
    //     input->size = 0;
    //     close(fd);
    //     return false;
    // }
    // close(fd);
    // input->content = file_content;
    // input->size = total_size;
    // return true;
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
