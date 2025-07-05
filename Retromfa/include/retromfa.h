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


typedef struct {
    char magic[4];         
    uint32_t version;      
    uint32_t num_objects; 
    uint32_t header_size; 
} mfa_header_t;


typedef struct {
    uint32_t offset;      
    uint32_t size;        
    uint32_t type;        
    char name[256];       
} mfa_object_t;


typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t bpp;         
    uint8_t *data;        
    size_t data_size;
} mfa_image_t;


int parse_mfa_file(const char *filename);
bool extract_images_from_mfa(const char *filename, const char *output_dir);
bool save_image_as_png(const mfa_image_t *image, const char *filename);
void free_mfa_image(mfa_image_t *image);



// Mlx

typedef struct s_mfa
{
    void	*mlx_ptr;
    void	*win_ptr;
    mfa_image_t    img_list[100];
}				t_mfa;


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
