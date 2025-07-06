#include "retromfa.h"

static bool read_through_file(t_mfa *mfa) {
    FILE *file = fopen(mfa->filename, READ_BINARY);
    if (!file) {
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
    while (!feof(file)) {
        size_t bytes_read = fread(buffer, sizeof(unsigned char), 1024, file);
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

                    // Debugging: Print the raw bytes for width and height
                    printf("Raw size bytes at position %lu: %02x %02x %02x %02x\n",
                           ftell(file) - 4, size_buffer[0], size_buffer[1], size_buffer[2], size_buffer[3]);

                    // Convert bytes to width and height
                    int16_t width = (size_buffer[0] | (size_buffer[1] << 8));
                    int16_t height = (size_buffer[2] | (size_buffer[3] << 8));

                    printf("Detected image size: width=%d, height=%d\n", width, height);

                    if (width > 0 && height > 0) {
                        fseek(file, file_offset + i + IMAGE_PIXEL_OFFSET, SEEK_SET);
                        mfa->img_list[mfa->img_count].type = is_image;
                        size_t img_size = width * height;
                        if (is_image == FOUND_24)
                            img_size = (width + width % 2) * height; // Adjust for 24-bit images

                        mfa->img_list[mfa->img_count].data = malloc(sizeof(char) * img_size * is_image);
                        if (!mfa->img_list[mfa->img_count].data) {
                            fprintf(stderr, "Error: Memory allocation failed for image data\n");
                            free(buffer);
                            fclose(file);
                            return false;
                        }

                        add_garbage(&mfa->img_list[mfa->img_count].data);
                        size_t b = fread(mfa->img_list[mfa->img_count].data, sizeof(char), img_size * is_image, file);
                        if (ferror(file) && b == 0) {
                            perror("Error reading image data");
                            free(mfa->img_list[mfa->img_count].data);
                            free(buffer);
                            fclose(file);
                            return false;
                        }

                        mfa->img_list[mfa->img_count].width = width;
                        mfa->img_list[mfa->img_count].height = height;

                        printf("Image %d: %d x %d, size: %zu bytes\n", mfa->img_count, width, height, img_size * is_image);
                        mfa->img_count++;
                        if (mfa->img_count >= MAX_IMAGES - 1) {
                            fprintf(stderr, "Error: Maximum number of images reached (%d)\n", MAX_IMAGES);
                            free(buffer);
                            fclose(file);
                            return false;
                        }
                    } else {
                        printf("Invalid image size: %d x %d\n", width, height);
                    }
                }
                // Move the file pointer to the pixel data
                fseek(file, file_offset + FILE_CHUNK_SIZE, SEEK_SET);
            }
        }
        file_offset += FILE_CHUNK_SIZE;
    }

    free(buffer);
    fclose(file);
    printf("File %s read successfully, size: %u bytes\n", mfa->filename, file_offset);
    return true;
}

bool find_images(t_mfa *mfa) {
    if (read_through_file(mfa) == false) {
        return false;
    }
    return true;
}
