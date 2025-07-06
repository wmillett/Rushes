#include "retromfa.h"

// Helper function to create a 32-bit RGBA color value
static uint32_t create_trgb(int t, int r, int g, int b) {
    return (t << 24 | r << 16 | g << 8 | b);
}

// Helper function to set a pixel in the image buffer
static void ft_pixel_put(t_mfa *mfa, int image_index, int x, int y, uint32_t color) {
    mfa_image_t *img = &mfa->img_list[image_index];

    // Check if coordinates are within bounds
    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return;

    // Only proceed if we have valid image data
    if (!img->address || !img->line_size || !img->bpp)
        return;

    // Calculate the memory offset for this pixel
    char *dst = img->address + (y * img->line_size + x * (img->bpp / 8));
    *(unsigned int *)dst = color;
}

// Function to convert raw image data to a displayable format
bool convert_raw_to_image(t_mfa *mfa, int image_index) {
    // Validate the image index
    if (image_index < 0 || image_index >= mfa->img_count) {
        fprintf(stderr, "Error: Invalid image index %d (valid range: 0-%d)\n",
                image_index, mfa->img_count - 1);
        return false;
    }

    mfa_image_t *current_image = &mfa->img_list[image_index];

    // Skip if already converted
    if (current_image->img != NULL) {
        return true;
    }

    // Extract dimensions for readability
    int image_width = current_image->width;
    int image_height = current_image->height;

    // Check for invalid dimensions
    if (image_width <= 0 || image_height <= 0 || !current_image->data) {
        fprintf(stderr, "Error: Invalid image dimensions or missing data for image %d\n", image_index);
        return false;
    }

    // Create a new image using MiniLibX
    current_image->img = mlx_new_image(mfa->mlx_ptr, image_width, image_height);
    if (!current_image->img) {
        fprintf(stderr, "Error: Failed to create image with mlx_new_image for image %d\n", image_index);
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
        fprintf(stderr, "Error: Failed to get image data address for image %d\n", image_index);
        return false;
    }

    // Store the values in our structure
    current_image->bpp = bits_per_pixel;
    current_image->line_size = size_line;
    current_image->endian = (bool)endian;

    // Calculate total size of image data
    size_t pixel_count = image_width * image_height;
    size_t image_data_size;

    if (current_image->type == FOUND_16) {
        image_data_size = pixel_count * 2; // 16 bits = 2 bytes per pixel
    } else { // FOUND_24
        image_data_size = (image_width + (image_width % 2)) * image_height * 3; // 24 bits = 3 bytes per pixel (with padding)
    }

    // Process the raw image data
    size_t color_index = 0;
    for (int row = 0; row < image_height; row++) {
        for (int col = 0; col < image_width; col++) {
            uint32_t pixel_color = 0x000000FF; // Default to opaque black

            // Check if we have enough data for this pixel
            if (color_index + (current_image->type == FOUND_16 ? 2 : 3) <= image_data_size) {
                if (current_image->type == FOUND_16) {
                    if (color_index + 1 < image_data_size) {
                        uint16_t color16 = (current_image->data[color_index]) |
                                          (current_image->data[color_index + 1] << 8);

                        // Extract 5-bit color components and scale to 8-bit
                        int r = ((color16 >> 10) & 0x1F) * 8;  // Scale 5-bit to 8-bit
                        int g = ((color16 >> 5) & 0x1F) * 8;
                        int b = (color16 & 0x1F) * 8;

                        pixel_color = create_trgb(0, r, g, b);
                    }
                    color_index += 2;
                } else { // FOUND_24
                    if (color_index + 2 < image_data_size) {
                        int b = current_image->data[color_index];
                        int g = current_image->data[color_index + 1];
                        int r = current_image->data[color_index + 2];
                        pixel_color = create_trgb(0, r, g, b);
                    }
                    color_index += 3;
                }
            } else {
                // Not enough data for this pixel, advance to next pixel position
                color_index += (current_image->type == FOUND_16) ? 2 : 3;
            }

            // Place the pixel color at the correct position in the image
            ft_pixel_put(mfa, image_index, col, row, pixel_color);
        }

        // Handle padding for 24-bit images
        if (current_image->type == FOUND_24) {
            int padding_bytes = (image_width % 2) * 3; // 3 bytes per pixel
            color_index += padding_bytes;
        }
    }

    return true;
}



// Function to display all loaded images in a grid
void display_images(t_mfa *mfa) {

    int row_pos = 0;
    int col_pos = 0;
    int max_row_height = 0;

    for (int img_index = 0; img_index < mfa->img_count; img_index++) {
        mfa_image_t *current_image = &mfa->img_list[img_index];

        // Check if we need to move to a new row
        if (row_pos + current_image->width > SCREEN_WIDTH) {
            row_pos = 0;
            col_pos += max_row_height + 5; // Add spacing between rows
            max_row_height = 0;            // Reset max row height for new row
        }

        // Update max row height
        if (current_image->height > max_row_height) {
            max_row_height = current_image->height;
        }

        // Display the image if it's within bounds
        if (col_pos + current_image->height > 0 && col_pos < SCREEN_HEIGHT) {
            mlx_put_image_to_window(mfa->mlx_ptr, mfa->win_ptr,
                                  current_image->img, row_pos, col_pos);
        }

        // Move to next column position
        row_pos += current_image->width + 5; // Add spacing between images
    }
}
