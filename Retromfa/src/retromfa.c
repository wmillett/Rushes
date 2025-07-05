#include "retromfa.h"


// static bool create_output_directory(const char *dir_path) {
//     struct stat st = {0};
//     if (stat(dir_path, &st) == -1) {
//         if (mkdir(dir_path, 0700) == -1) {
//             fprintf(stderr, "Error: Failed to create directory %s\n", dir_path);
//             return false;
//         }
//     }
//     return true;
// }


// static bool read_mfa_header(FILE *file, mfa_header_t *header) {
//     if (fread(header, sizeof(mfa_header_t), 1, file) != 1) {
//         fprintf(stderr, "Error: Failed to read MFA header\n");
//         return false;
//     }
    

//     if (strncmp(header->magic, "MMF2", 4) != 0) {
//         fprintf(stderr, "Error: Invalid MFA file format (wrong magic number)\n");
//         return false;
//     }
    
//     printf("MFA Header:\n");
//     printf("  Magic: %.4s\n", header->magic);
//     printf("  Version: %u\n", header->version);
//     printf("  Objects: %u\n", header->num_objects);
//     printf("  Header Size: %u\n", header->header_size);
    
//     return true;
// }


// static bool read_object_info(FILE *file, mfa_object_t *obj) {
//     if (fread(&obj->offset, sizeof(uint32_t), 1, file) != 1) return false;
//     if (fread(&obj->size, sizeof(uint32_t), 1, file) != 1) return false;
//     if (fread(&obj->type, sizeof(uint32_t), 1, file) != 1) return false;
    

//     int i = 0;
//     while (i < 255) {
//         int c = fgetc(file);
//         if (c == EOF || c == 0) break;
//         obj->name[i++] = (char)c;
//     }
//     obj->name[i] = '\0';
    
//     return true;
// }


// static bool is_image_data(const uint8_t *data, size_t size) {
//     if (size < 8) return false;
    

//     if (size >= 2 && data[0] == 0xFF && data[1] == 0xD8) return true; // JPEG
//     if (size >= 8 && data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47) return true; // PNG
//     if (size >= 2 && data[0] == 0x42 && data[1] == 0x4D) return true; // BMP
//     if (size >= 6 && data[0] == 0x47 && data[1] == 0x49 && data[2] == 0x46) return true; // GIF
    
//     return false;
// }


// static bool extract_image_data(FILE *file, const mfa_object_t *obj, mfa_image_t *image) {
 
//     long current_pos = ftell(file);
    

//     if (fseek(file, obj->offset, SEEK_SET) != 0) {
//         fprintf(stderr, "Error: Failed to seek to object data\n");
//         return false;
//     }
    
  
//     image->data = malloc(obj->size);
//     if (!image->data) {
//         fprintf(stderr, "Error: Failed to allocate memory for image data\n");
//         return false;
//     }
    
//     if (fread(image->data, 1, obj->size, file) != obj->size) {
//         fprintf(stderr, "Error: Failed to read image data\n");
//         free(image->data);
//         return false;
//     }
    
//     image->data_size = obj->size;
    

//     if (is_image_data(image->data, obj->size)) {
       
//         image->width = 32;
//         image->height = 32;
//         image->bpp = 16;
//         printf("  Detected image: %ux%u, %u bpp\n", image->width, image->height, image->bpp);
//     } else {

//         image->width = 64;
//         image->height = 64;
//         image->bpp = 16;
//         printf("  Unknown format, using default: %ux%u, %u bpp\n", image->width, image->height, image->bpp);
//     }

//     fseek(file, current_pos, SEEK_SET);
//     return true;
// }


// static bool save_raw_image(const mfa_image_t *image, const char *filename) {
//     FILE *file = fopen(filename, "wb");
//     if (!file) {
//         fprintf(stderr, "Error: Failed to create file %s\n", filename);
//         return false;
//     }
    
//     if (fwrite(image->data, 1, image->data_size, file) != image->data_size) {
//         fprintf(stderr, "Error: Failed to write image data\n");
//         fclose(file);
//         return false;
//     }
    
//     fclose(file);
//     printf("  Saved raw image: %s (%zu bytes)\n", filename, image->data_size);
//     return true;
// }



// bool extract_images_from_mfa(const char *filename, const char *output_dir) {
//     if (!create_output_directory(output_dir)) {
//         return false;
//     }
    
//     return parse_mfa_file(filename);
// }


// bool save_image_as_png(const mfa_image_t *image, const char *filename) {

//     printf("Note: PNG saving not implemented, saving as raw data instead\n");
//     return save_raw_image(image, filename);
// }


// void free_mfa_image(mfa_image_t *image) {
//     if (image && image->data) {
//         free(image->data);
//         image->data = NULL;
//         image->data_size = 0;
//     }
// }


int find_image(t_mfa *mfa, const char *filename) {
    // This function is a placeholder for finding an image in the MFA structure.
    // It should be implemented to search through the mfa->img_list for the specified filename.
    // For now, it returns -1 to indicate that the image was not found.
    return -1;
}


// int parse_mfa_file(const char *filename) {

//     FILE *file = fopen(filename, "rb");
//     if (!file) {
//         fprintf(stderr, "Error: Failed to open file %s\n", filename);
//         return false;
//     }

//     mfa_header_t header;
//     if (!read_mfa_header(file, &header)) {
//         fclose(file);
//         return false;
//     }

//     printf("Processing %u objects...\n", header.num_objects);

//     for (uint32_t i = 0; i < header.num_objects; i++) {
//         mfa_object_t obj = {0};
//         if (!read_object_info(file, &obj)) {
//             fprintf(stderr, "Error: Failed to read object %u\n", i);
//             continue;
//         }

//         printf("Object %u: %s (offset: %u, size: %u, type: %u)\n",
//                i, obj.name, obj.offset, obj.size, obj.type);

//         if (obj.size > 0 && obj.size < 10 * 1024 * 1024) { // Less than 10MB
//             fseek(file, obj.offset, SEEK_SET);
//             uint8_t *data = malloc(obj.size);
//             if (!data || fread(data, 1, obj.size, file) != obj.size) {
//                 fprintf(stderr, "Error: Failed to read data for object %u\n", i);
//                 free(data);
//                 continue;
//             }

//                 mfa_image_t image = {0};
//                 image.data = data;
//                 image.data_size = obj.size;

//                 char output_filename[512];
//                 snprintf(output_filename, sizeof(output_filename), 
//                          "extracted_%s_%u.raw", obj.name, i);

//                 if (save_raw_image(&image, output_filename)) {
//                     printf("Successfully extracted image: %s\n", output_filename);
//                 } else {
//                     fprintf(stderr, "Failed to save image: %s\n", output_filename);
//                 }
//                 free(data);
//         }
//     }

//     fclose(file);
//     return true;
// }
