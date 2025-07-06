#ifndef RETROMFA_H
# define RETROMFA_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>


# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include "mlx.h"

# define EXTENSION_MFA ".mfa"
# define ESC_KEY 65307
# define BUFFER_SIZE 4096 // Size of the buffer for reading files


# define MAX_IMAGES 1000 // Maximum number of images to store in the mfa structure
# define MAX_IMAGE_SIZE 10485760 // Maximum size of a single image in bytes (10 MB)

# define READ_BINARY "rb" // Read mode for binary files



// MMF2 header flags for image types

# define HEADER16_FLAG 0x06 // MMF2 header flag for 16-bit images
# define HEADER24_FLAG 0x04 // MMF2 header flag for 24-bit images
# define HEADER_SIZE 4 // Size of the MMF2 header in bytes

# define FOUND_16 1 // Flag to indicate a 16-bit image was found
# define FOUND_24 2 // Flag to indicate a 24-bit image was found


# define IMAGE_SIZE_OFFSET -4
# define IMAGE_PIXEL_OFFSET 16 // Offset where image data starts in the file

# define FILE_CHUNK_SIZE 1024 // Size of each chunk to read from the file

// Structures for handling MFA images

typedef struct mfa_image_s{
    int16_t width;
    int16_t height;
    uint32_t bpp;         
    unsigned char *data;
    void *img;
    int type;
    bool endian; // 0 for little-endian, 1 for big-endian
    int line_size; // Size of a single line in bytes
    char *address; // Pointer to the image data in memory
    
} mfa_image_t;


// // Header pattern for 16-bit images
// static const int16_t HEADER_16BIT[] = {0x06, 0x10, 0x00, 0x00};

// // Header pattern for 24-bit images
// static const int16_t HEADER_24BIT[] = {0x04, 0x10, 0x00, 0x00};



typedef enum {
    HEADER_NONE = 0,
    HEADER_16BIT,
    HEADER_24BIT,
    HEADER_TYPE_COUNT  // for bounds checking if needed
} HeaderType;

static const int16_t IMAGE_HEADERS[HEADER_TYPE_COUNT][HEADER_SIZE] = {
    [HEADER_NONE]  = {0x00, 0x00, 0x00, 0x00},  // Placeholder for NONE
    [HEADER_16BIT] = {0x06, 0x10, 0x00, 0x00},  // 16-bit image header
    [HEADER_24BIT] = {0x04, 0x10, 0x00, 0x00}   // 24-bit image header
};


// Mlx and MFA list structures

typedef struct s_mfa
{
    void	*mlx_ptr;
    void	*win_ptr;
    int		img_count; // Count of images in img_list
    char    *filename; // Name of the MFA file
    char    *content; // Content of the MFA file
    size_t  size; // Size of the content
    mfa_image_t    *img_list; // 
}				t_mfa;




bool find_images(t_mfa *mfa);
void create_mlx_image_from_mfa(t_mfa *mfa, void *mlx_ptr, int index);
void display_image(t_mfa *mfa, int image_index);







void	exit_mlx(t_mfa *mfa);
int		key_hook(int keycode, t_mfa *mfa);

// Garbage Collector

typedef struct s_memlist
{
	void				*address;
	struct s_memlist	*mem_next;
}						t_memlist;


t_memlist				*mem_data(void);
void					*list_malloc(size_t nmemb, size_t size);
void					free_one(void *address);
void					all_free(void);
void					add_garbage(void *to_add);


#endif
