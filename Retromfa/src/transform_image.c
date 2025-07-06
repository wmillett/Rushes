#include "retromfa.h"

// Helper function to create a 32-bit RGBA color value
static uint32_t compose_rgba_color(int alpha, int red, int green, int blue) {
    return ((alpha & 0xFF) << 24) | ((red & 0xFF) << 16) | ((green & 0xFF) << 8) | (blue & 0xFF);
}

// Helper function to set a pixel in the image buffer
static void place_pixel_in_image(mfa_image_t *img, int x, int y, uint32_t color) {
    // Check if coordinates are within bounds
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;
    // Calculate the memory offset for this pixel
    int pixel_offset = y * img->line_size + x * (img->bpp / 8);
    // Write the color value to the image buffer
    *(uint32_t*)(img->address + pixel_offset) = color;
}

// Function to convert raw image data to a displayable format
bool convert_raw_to_image(t_mfa *mfa) {
    // Get the current image from the list
    mfa_image_t *current_image = &mfa->img_list[mfa->img_count];
    // Extract dimensions for readability
    int image_width = current_image->width;
    int image_height = current_image->height;

    // Create a new image using MiniLibX
    current_image->img = mlx_new_image(mfa->mlx_ptr, image_width, image_height);
    if (!current_image->img) {
        fprintf(stderr, "Error: Failed to create image with mlx_new_image\n");
        return false;
    }
    // Temporary variables to match mlx_get_data_addr expectations
    int bits_per_pixel;
    int size_line;
    int endian;

    // Get the image data address
    current_image->address = mlx_get_data_addr(current_image->img,
                                               &bits_per_pixel,
                                               &size_line,
                                               &endian);

    if (!current_image->address) {
        fprintf(stderr, "Error: Failed to get image data address\n");
        return false;
    }

    // Store the values in our structure
    current_image->bpp = (uint32_t)bits_per_pixel;  // Cast to uint32_t if needed
    current_image->line_size = size_line;
    current_image->endian = (bool)endian;  // Cast to bool

    // Calculate total size of image data
    size_t pixel_count = image_width * image_height;
    size_t image_data_size = pixel_count * current_image->type; // Each pixel is 2 or 3 bytes

    // Adjust for 24-bit images with padding
    if (current_image->type == FOUND_24) {
        image_data_size = (image_width + image_width % 2) * image_height * current_image->type;
    }

    // Validate image data
    if (!current_image->data || image_data_size > MAX_IMAGE_SIZE) {
        fprintf(stderr, "Error: Invalid image data or size exceeds maximum\n");
        return false;
    }

    // Process the raw image data
    size_t color_index = 0;
    for (int row = 0; row < image_height; row++) {
        for (int col = 0; col < image_width; col++) {
            uint32_t pixel_color = 0;
            // Process based on color depth
            if (current_image->type == FOUND_16) {
                // Process 16-bit color
                if (color_index + 1 >= image_data_size) {
                    fprintf(stderr, "Error: Not enough data for 16-bit pixel at (%d, %d)\n", col, row);
                    return false;
                }
                // Read 16-bit color value (little-endian)
                uint16_t color16 = (current_image->data[color_index]) |
                                  (current_image->data[color_index + 1] << 8);
                // Extract 5-bit color components
                int red = (color16 >> 10) & 0x1F;    // 5 bits
                int green = (color16 >> 5) & 0x1F;   // 5 bits
                int blue = color16 & 0x1F;           // 5 bits
                // Scale 5-bit colors to 8-bit (0-255) range
                red = (red * 255) / 31;
                green = (green * 255) / 31;
                blue = (blue * 255) / 31;
                // Create a 32-bit color with alpha set to 0 (fully opaque)
                pixel_color = compose_rgba_color(0, red, green, blue);
                color_index += 2; // Move to next 16-bit color
            }
            else if (current_image->type == FOUND_24) {
                // Process 24-bit color (BGR format)
                if (color_index + 2 >= image_data_size) {
                    fprintf(stderr, "Error: Not enough data for 24-bit pixel at (%d, %d)\n", col, row);
                    return false;
                }
                // Extract RGB components (assuming BGR format)
                int blue = current_image->data[color_index];
                int green = current_image->data[color_index + 1];
                int red = current_image->data[color_index + 2];
                // Create a 32-bit color with alpha set to 0 (fully opaque)
                pixel_color = compose_rgba_color(0, red, green, blue);
                color_index += 3; // Move to next 24-bit color
            }
            // Place the pixel color at the correct position in the image
            place_pixel_in_image(current_image, col, row, pixel_color);
        }
        // Handle padding for 24-bit images
        if (current_image->type == FOUND_24) {
            // Calculate padding bytes needed for 4-byte alignment
            int padding_bytes = (image_width % 2) * 3; // 3 bytes per pixel
            color_index += padding_bytes;
        }
    }

    return true;
}
