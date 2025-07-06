#include "retromfa.h"
bool read_through_file(t_mfa *mfa) {
    FILE *file = fopen(mfa->filename, READ_BINARY);
    if (!file) {
        perror("Error opening file");
        mfa->content = NULL;
        mfa->size = 0;
        return false;
    }

    unsigned char *buffer = malloc(sizeof(unsigned char) * FILE_CHUNK_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return false;
    }

    unsigned int file_offset = 0;
    while (!feof(file)) {
        size_t bytes_read = fread(buffer, sizeof(unsigned char), FILE_CHUNK_SIZE, file);
        if (bytes_read == 0 && ferror(file)) {
            perror("Error reading file");
            free(buffer);
            fclose(file);
            return false;
        }

        for (size_t i = 0; i < bytes_read; i++) {
            int is_image = 0;
            if (buffer[i] == HEADER16_FLAG) {
                is_image = FOUND_16;
            } else if (buffer[i] == HEADER24_FLAG) {
                is_image = FOUND_24;
            }

            if (is_image) {
                bool valid_header = true;
                for (size_t j = 0; j < HEADER_SIZE; j++) {
                    if (i + j >= FILE_CHUNK_SIZE || buffer[i + j] != IMAGE_HEADERS[is_image][j]) {
                        valid_header = false;
                        break;
                    }
                }

                if (valid_header) {
                    fseek(file, file_offset + i - IMAGE_SIZE_OFFSET, SEEK_SET);
                    unsigned char size_buffer[4];
                    size_t bytes = fread(size_buffer, sizeof(unsigned char), 4, file);
                    if (bytes != 4) {
                        perror("Error reading file after header");
                        free(buffer);
                        fclose(file);
                        return false;
                    }

                    int16_t width = (size_buffer[0] | (size_buffer[1] << 8));
                    int16_t height = (size_buffer[2] | (size_buffer[3] << 8));

                    if (width > 0 && height > 0) {
                        fseek(file, file_offset + i + IMAGE_PIXEL_OFFSET, SEEK_SET);

                        // Set up the image structure
                        mfa_image_t *new_image = &mfa->img_list[mfa->img_count];
                        new_image->type = is_image;
                        new_image->width = width;
                        new_image->height = height;

                        size_t img_size = width * height;
                        if (is_image == FOUND_24)
                            img_size = (width + width % 2) * height; // Adjust for 24-bit images

                        // Allocate memory for the raw image data
                        new_image->data = tracked_malloc(img_size * is_image);
                        if (!new_image->data) {
                            fprintf(stderr, "Error: Memory allocation failed for image data\n");
                            free(buffer);
                            fclose(file);
                            return false;
                        }

                        // Read the image data
                        size_t bytes_read = fread(new_image->data,
                                                sizeof(char),
                                                img_size * is_image,
                                                file);
                        if (ferror(file) || bytes_read != img_size * is_image) {
                            perror("Error reading image data");
                            free(buffer);
                            fclose(file);
                            return false;
                        }

                        printf("Found image %d: %dx%d, type=%d\n",
                              mfa->img_count, width, height, is_image);

                        // Convert this image immediately after loading its data
                        if (!convert_raw_to_image(mfa, mfa->img_count)) {
                            fprintf(stderr, "Error converting image %d\n", mfa->img_count);
                            free(buffer);
                            fclose(file);
                            return false;
                        }

                        // Only increment after successful processing
                        mfa->img_count++;
                        if (mfa->img_count >= MAX_IMAGES) {
                            fprintf(stderr, "Error: Maximum number of images reached (%d)\n", MAX_IMAGES);
                            free(buffer);
                            fclose(file);
                            return false;
                        }

                        // Skip past this image data in the file
                        fseek(file, file_offset + i + FILE_CHUNK_SIZE, SEEK_SET);
                    } else {
                        printf("Invalid image size: %d x %d\n", width, height);
                    }
                }
            }
        }
        file_offset += FILE_CHUNK_SIZE;
    }

    free(buffer);
    fclose(file);
    printf("File %s read successfully, size: %u bytes\n", mfa->filename, file_offset);
    return true;
}

