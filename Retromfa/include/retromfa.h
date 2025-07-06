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
# define ESC 65307



# define BUFFER_SIZE 4096 // Size of the buffer for reading files

# define MAX_IMAGES 1000 // Maximum number of images to store in the mfa structure
# define MAX_IMAGE_SIZE 10485760 // Maximum size of a single image in bytes (10 MB)




// Structures for handling MFA images

typedef struct {
    int16_t width;
    int16_t height;
    uint32_t bpp;         
    uint8_t *data;        
    void *img;
    char type;
    bool endian; // 0 for little-endian, 1 for big-endian
    int line_size; // Size of a single line in bytes
    char *address; // Pointer to the image data in memory
    
} mfa_image_t;




// Mlx and MFA list structures

typedef struct s_mfa
{
    void	*mlx_ptr;
    void	*win_ptr;
    int		img_count; // Count of images in img_list
    char    *filename_index; // Name of the MFA file
    char    *content; // Content of the MFA file
    size_t  size; // Size of the content
    mfa_image_t    img_list[1000]; // Array to hold images
}				t_mfa;

bool find_images(t_mfa *mfa, const char *filename);







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
