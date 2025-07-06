#include "retromfa.h"


void display_image(t_mfa *mfa, int image_index)
{
    if (image_index < 0 || image_index >= mfa->img_count)
    {
        fprintf(stderr, "Invalid image index %d\n", image_index);
        return;
    }

    mfa_image_t *img = &mfa->img_list[image_index];
    int bpp, size_line, endian;

    // Create MLX image with correct dimensions
    img->img = mlx_new_image(mfa->mlx_ptr, img->width, img->height);
    if (!img->img)
    {
        fprintf(stderr, "Failed to create MLX image\n");
        return;
    }

    char *mlx_data = mlx_get_data_addr(img->img, &bpp, &size_line, &endian);

    // Clear the image buffer just in case
    memset(mlx_data, 0, size_line * img->height);

    if (img->type == FOUND_24)
    {
        // 24-bit RGB to 32-bit BGRA conversion
        for (int y = 0; y < img->height; y++)
        {
            for (int x = 0; x < img->width; x++)
            {
                int src_i = (y * img->width + x) * 3;
                int dst_i = y * size_line + x * 4;

                unsigned char r = img->data[src_i];
                unsigned char g = img->data[src_i + 1];
                unsigned char b = img->data[src_i + 2];

                mlx_data[dst_i] = b;
                mlx_data[dst_i + 1] = g;
                mlx_data[dst_i + 2] = r;
                mlx_data[dst_i + 3] = 0; // alpha (opaque)
            }
        }
    }
    else if (img->type == FOUND_16)
    {
        // 16-bit RGB565 to 32-bit BGRA conversion
        for (int y = 0; y < img->height; y++)
        {
            for (int x = 0; x < img->width; x++)
            {
                int src_i = (y * img->width + x) * 2;
                int dst_i = y * size_line + x * 4;

                uint16_t pixel = (uint8_t)img->data[src_i] | ((uint8_t)img->data[src_i + 1] << 8);

                // Extract RGB components (5 bits red, 6 bits green, 5 bits blue)
                unsigned char r5 = (pixel >> 11) & 0x1F;
                unsigned char g6 = (pixel >> 5) & 0x3F;
                unsigned char b5 = pixel & 0x1F;

                // Scale to 8-bit per channel
                unsigned char r = (r5 * 255) / 31;
                unsigned char g = (g6 * 255) / 63;
                unsigned char b = (b5 * 255) / 31;

                mlx_data[dst_i] = b;
                mlx_data[dst_i + 1] = g;
                mlx_data[dst_i + 2] = r;
                mlx_data[dst_i + 3] = 0; // alpha (opaque)
            }
        }
    }
    else
    {
        fprintf(stderr, "Unsupported image type %d\n", img->type);
        mlx_destroy_image(mfa->mlx_ptr, img->img);
        img->img = NULL;
        return;
    }

    // Put the image to window at position (0, 0)
    mlx_put_image_to_window(mfa->mlx_ptr, mfa->win_ptr, img->img, 0, 0);
}
