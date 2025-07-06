#include "retromfa.h"


// static char *append_to_buffer(char *original, size_t original_size,
//                               const char *chunk, size_t chunk_size)
// {
//     char *new_buffer = list_malloc(original_size + chunk_size, sizeof(char));
//     if (!new_buffer)
//     {   fprintf(stderr, "Error: Memory allocation failed while appending to buffer.\n");
//         return NULL;
//     }
//     if (original)
//         memcpy(new_buffer, original, original_size);
//     memcpy(new_buffer + original_size, chunk, chunk_size);
//     return new_buffer;
// }



static bool read_through_file(t_mfa *mfa){
    //int16_t header[] = {0x4D, 0x4D, 0x46, 0x32}; // MMF2 header
    //int16_t header16[] = {0x06, 0x10, 0x0, 0x0}; // MMF2 header for 16-bit images
    //int16_t header24[] = {0x04, 0x10, 0x0, 0x0}; // MMF2 header for 24-bit images


    FILE *file = fopen(mfa->filename, READ_BINARY);
    if (!file)
    {
        perror("Error opening file");
        mfa->content = NULL;
        mfa->size = 0;
        return false;
    }


    unsigned char *buffer = malloc(sizeof(unsigned char) * 1024);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return false;
    }

    unsigned int file_offset = 0;
    while(!feof(file)) {
        size_t bytes_read = fread(buffer, sizeof(unsigned char), 1024, file);
        if (bytes_read == 0 && ferror(file)) {
            perror("Error reading file");
            free(buffer);
            fclose(file);
            return false;
        }
        //printf("Read %zu bytes from file\n", bytes_read);

        int is_image = 0;
        for (size_t i = 0; i < bytes_read; i++) {
            is_image = 0;
            if(buffer[i] == HEADER16_FLAG) {
                is_image = FOUND_16;
                
            } else if(buffer[i] == HEADER24_FLAG) {
                is_image = FOUND_24;
            }
            
            if (is_image){
                for (size_t j = 0; j < HEADER_SIZE; j++)
                {
                    if (i + j >= FILE_CHUNK_SIZE || buffer[i + j] != IMAGE_HEADERS[is_image][j]) {
                        is_image = false;
                        break;
                    }
                }
                if (!is_image) {
                    printf("Invalid image header at byte %zu\n", i);
                    continue; // Skip to the next byte if the header is invalid
                }
                else{

                    fseek(file, file_offset + i - IMAGE_SIZE_OFFSET, SEEK_SET); // Move the file pointer to the next byte after the header
                    size_t b = fread(buffer, sizeof(unsigned char), 4, file);
                    if (ferror(file) && b == 0) {
                        perror("Error reading file after header");
                        free(buffer);
                        fclose(file);
                        return false;
                    }
                    // Convert little-endian size to 16-bit integer
                    int16_t width = (buffer[0] | (buffer[1] << 8));
                    int16_t height = (buffer[2] | (buffer[3] << 8));
                    printf("Image size: %d x %d\n", width, height);
                    if (width > 0 && height > 0) {
                        fseek(file, file_offset + i + IMAGE_PIXEL_OFFSET, SEEK_SET);

						mfa->img_list[mfa->img_count].type = is_image;
                        size_t img_size = width * height;
                        if (is_image == FOUND_24)
                            img_size = (width + width % 2) * height; // Adjust for 24-bit images


						mfa->img_list[mfa->img_count]->data = list_malloc(sizeof(char) * img_size * mfa->img_list[mfa->img_count].type);
						if (!mfa->img_list[mfa->img_count].data) {
                            fprintf(stderr, "Error: Memory allocation failed for image data\n");
                            free(buffer);
                            fclose(file);
							return false;
						}
                        add_garbage(&mfa->img_list[mfa->img_count].data );
						size_t b = fread(mfa->img_list[mfa->img_count].data, sizeof(char), img_size * mfa->img_list[mfa->img_count].type, file);
                        if (ferror(file) && b == 0) {
                            perror("Error reading image data");
                            free(mfa->img_list[mfa->img_count].data);
                            free(buffer);
                            fclose(file);
                            return false;
                        }
                        mfa->img_list[mfa->img_count].width = width;
                        mfa->img_list[mfa->img_count].height = height;
                        






						img_produce(mfa);

						//free(mfa->img_list[mfa->img_count].data);
						mfa->img_count++;
                        if (mfa->img_count >= MAX_IMAGES - 1) {
                            fprintf(stderr, "Error: Maximum number of images reached (%d)\n", MAX_IMAGES);
                            free(buffer);
                            fclose(file);
                            return false;
                        }

                    }
                    else {
                        //printf("Invalid image size: %d x %d at byte %d\n", width, height, i);
                    }
                }
                // Move the file pointer to the pixel data
                fseek(file, file_offset + FILE_CHUNK_SIZE, SEEK_SET);
            }
            // Example processing: just print the byte value
            //printf("%02x ", buffer[i]);
        }
        file_offset += FILE_CHUNK_SIZE;
    }
    free(buffer);
    fclose(file);
    printf("File %s read successfully, size: %u bytes\n", mfa->filename, file_offset);
    return true;
}


bool find_images(t_mfa *mfa) {

    (void)mfa; // Suppress unused parameter warning


    unsigned char *buffer = list_malloc(1, sizeof(unsigned char) * FILE_CHUNK_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return false;
    }

    if(read_through_file(mfa) == false   )
    {
        return false;
    }
    
    return true;
}
