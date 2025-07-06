#include "retromfa.h"

// static char *append_to_buffer(char *original, size_t original_size,
//                               const char *chunk, size_t chunk_size)
// {
//     char *new_buffer = list_malloc(original_size + chunk_size, sizeof(char));
//     if (!new_buffer)
//     {   fprintf(stderr,
//	"Error: Memory allocation failed while appending to buffer.\n");
//         return (NULL);
//     }
//     if (original)
//         memcpy(new_buffer, original, original_size);
//     memcpy(new_buffer + original_size, chunk, chunk_size);
//     return (new_buffer);
// }

static bool	read_through_file(t_mfa *mfa)
{
	FILE			*file;
	unsigned char	*buffer;
	unsigned int	file_offset;
	size_t			bytes_read;
	int				is_image;
	size_t			b;
	int16_t			width;
	int16_t			height;
	size_t			img_size;

	// int16_t header[] = {0x4D, 0x4D, 0x46, 0x32}; // MMF2 header
	// int16_t header16[] = {0x06, 0x10, 0x0, 0x0};
		// MMF2 header for 16-bit images
	// int16_t header24[] = {0x04, 0x10, 0x0, 0x0};
		// MMF2 header for 24-bit images
	file = fopen(mfa->filename, READ_BINARY);
	if (!file)
	{
		perror("Error opening file");
		mfa->content = NULL;
		mfa->size = 0;
		return (false);
	}
	// 1  allocating mfa->img_list and initializing mfa->imgae_count before the loop
	mfa->img_list = malloc(sizeof(mfa_image_t) * MAX_IMAGES);
	if (!mfa->img_list)
	{
		fprintf(stderr, "Error: Memory allocation failed for image list\n");
		fclose(file);
		return (false);
	}
	memset(mfa->img_list, 0, sizeof(mfa_image_t) * MAX_IMAGES);
	mfa->img_count = 0;
	buffer = malloc(sizeof(unsigned char) * 1024);
	if (buffer == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed\n");
		fclose(file);
		return (false);
	}
	file_offset = 0;
	while (!feof(file))
	{
		bytes_read = fread(buffer, sizeof(unsigned char), 1024, file);
		if (bytes_read == 0 && ferror(file))
		{
			perror("Error reading file");
			free(buffer);
			fclose(file);
			return (false);
		}
		// printf("Read %zu bytes from file\n", bytes_read);
		is_image = 0;
		for (size_t i = 0; i + HEADER_SIZE < bytes_read; i++)
		{
			is_image = 0;
			// 2  before we were reading single byte but now it is 4 byte as header is 4 byte long
			if (memcmp(buffer + i, IMAGE_HEADERS[HEADER_16BIT],
					HEADER_SIZE) == 0)
				is_image = FOUND_16;
			else if (memcmp(buffer + i, IMAGE_HEADERS[HEADER_24BIT],
					HEADER_SIZE) == 0)
				is_image = FOUND_24;
			if (is_image)
			{
				for (size_t j = 0; j < HEADER_SIZE; j++)
				{
					if (i + j >= FILE_CHUNK_SIZE || buffer[i
						+ j] != IMAGE_HEADERS[is_image][j])
					{
						is_image = false;
						break ;
					}
				}
				if (!is_image)
				{
					printf("Invalid image header at byte %zu\n", i);
					continue ; // Skip to the next byte if the header is invalid
				}
				else
				{
					fseek(file, file_offset + i - IMAGE_SIZE_OFFSET, SEEK_SET);
						// Move the file pointer to the next byte after the header
					b = fread(buffer, sizeof(unsigned char), 4, file);
					if (ferror(file) && b == 0)
					{
						perror("Error reading file after header");
						free(buffer);
						fclose(file);
						return (false);
					}
					// Convert little-endian size to 16-bit integer
					width = (buffer[0] | (buffer[1] << 8));
					height = (buffer[2] | (buffer[3] << 8));
					printf("Image size: %d x %d\n", width, height);
					if (width > 0 && height > 0)
					{
						fseek(file, file_offset + i + IMAGE_PIXEL_OFFSET,
							SEEK_SET);
						mfa->img_list[mfa->img_count].type = is_image;
						img_size = width * height;
						if (is_image == FOUND_24)
							img_size = width * height * 3;
								// (corrected) Adjust for 24-bit images
						else
							img_size = width * height * 2;
								// 1 byte per pixel for 16 bit
						mfa->img_list[mfa->img_count].data = malloc(img_size);
						if (!mfa->img_list[mfa->img_count].data)
						{
							fprintf(stderr,
								"Error: Memory allocation failed for image data\n");
							free(buffer);
							fclose(file);
							return (false);
						}
						add_garbage(&mfa->img_list[mfa->img_count].data);
						b = fread(mfa->img_list[mfa->img_count].data, 1, img_size, file);	
						if (ferror(file) && b == 0)
						{
							perror("Error reading image data");
							free(mfa->img_list[mfa->img_count].data);
							free(buffer);
							fclose(file);
							return (false);
						}
						mfa->img_list[mfa->img_count].width = width;
						mfa->img_list[mfa->img_count].height = height;
						// img_produce(mfa);
						// free(mfa->img_list[mfa->img_count].data);
						mfa->img_count++;
						if (mfa->img_count >= MAX_IMAGES - 1)
						{
							fprintf(stderr,
								"Error: Maximum number of images reached (%d)\n",
								MAX_IMAGES);
							free(buffer);
							fclose(file);
							return (false);
						}
					}
					else
					{
						printf("Invalid image size: %d x %d at byte %zd\n",
							width, height, i);
					}
				}
				// Move the file pointer to the pixel data
				//3. (testing)fseek(file, file_offset + FILE_CHUNK_SIZE, SEEK_SET);
			}
			// Example processing: just print the byte value
			// printf("%02x ", buffer[i]);
		}
		file_offset += FILE_CHUNK_SIZE;
	}
	free(buffer);
	fclose(file);
	printf("File %s read successfully, size: %u bytes\n", mfa->filename,
		file_offset);
	return (true);
}

bool	find_images(t_mfa *mfa)
{
	unsigned char	*buffer;

	(void)mfa; // Suppress unused parameter warning
	buffer = list_malloc(1, sizeof(unsigned char) * FILE_CHUNK_SIZE);
	if (buffer == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed\n");
		return (false);
	}
	if (read_through_file(mfa) == false)
	{
		return (false);
	}
	return (true);
}


// You need to pass the mlx pointer and window or create them if not already done

void create_mlx_image_from_mfa(t_mfa *mfa, void *mlx_ptr, int index)
{
    if (index >= mfa->img_count)
    {
        fprintf(stderr, "Invalid image index\n");
        return;
    }

    mfa_image_t *img = &mfa->img_list[index];

    // Create a new MLX image
    void *mlx_img = mlx_new_image(mlx_ptr, img->width, img->height);
    if (!mlx_img)
    {
        fprintf(stderr, "Failed to create MLX image\n");
        return;
    }

    // Get address to image pixel buffer
    char *mlx_data = mlx_get_data_addr(mlx_img, &(int){0}, &(int){0}, &(int){0});
    // Note: You need to get bpp, line_length and endian from mlx_get_data_addr:
    int bpp, line_length, endian;
    mlx_data = mlx_get_data_addr(mlx_img, &bpp, &line_length, &endian);

    // Now copy or convert your pixel data into mlx_data:
    // For 24-bit RGB images, you might need to convert pixel format (depends on MLX)
    // For 16-bit images, you probably need to convert 16-bit data to 32-bit ARGB

    // Example for 24-bit (assuming your data is RGB and MLX wants 32-bit ARGB):
    if (img->type == FOUND_24)
    {
        for (int y = 0; y < img->height; y++)
        {
            for (int x = 0; x < img->width; x++)
            {
                int mlx_pos = y * line_length + x * (bpp / 8);
                int img_pos = (y * img->width + x) * 3;

                // Extract RGB from your data
                unsigned char r = (unsigned char)img->data[img_pos];
                unsigned char g = (unsigned char)img->data[img_pos + 1];
                unsigned char b = (unsigned char)img->data[img_pos + 2];

                // Set pixel in MLX data (ARGB)
                mlx_data[mlx_pos] = b;          // Blue
                mlx_data[mlx_pos + 1] = g;      // Green
                mlx_data[mlx_pos + 2] = r;      // Red
                mlx_data[mlx_pos + 3] = 0;      // Alpha (0 = opaque or adjust as needed)
            }
        }
    }
    else if (img->type == FOUND_16)
    {
        // For 16-bit, you need to convert RGB565 (likely format) to ARGB8888 MLX format
        for (int y = 0; y < img->height; y++)
        {
            for (int x = 0; x < img->width; x++)
            {
                int mlx_pos = y * line_length + x * (bpp / 8);
                int img_pos = (y * img->width + x) * 2;

                // Assuming RGB565:
                uint16_t pixel = ((uint16_t)img->data[img_pos + 1] << 8) | img->data[img_pos];

                unsigned char r = ((pixel >> 11) & 0x1F) << 3;
                unsigned char g = ((pixel >> 5) & 0x3F) << 2;
                unsigned char b = (pixel & 0x1F) << 3;

                mlx_data[mlx_pos] = b;
                mlx_data[mlx_pos + 1] = g;
                mlx_data[mlx_pos + 2] = r;
                mlx_data[mlx_pos + 3] = 0;
            }
        }
    }

    // Now you can display mlx_img in a window with mlx_put_image_to_window or save it as needed
    // e.g. mlx_put_image_to_window(mlx_ptr, win_ptr, mlx_img, 0, 0);

    // Store mlx_img pointer in your structure if you want to reuse or free it later
}

