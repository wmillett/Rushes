#include "retromfa.h"

t_list	*createNode(void *mlx_img, char *data, size_t width, size_t height)
{
	t_list *node = calloc(1, sizeof (t_list));

	if (!node)
	{
		return NULL;
	}

	node->content.img_ptr = mlx_img;
	node->content.data = data;
	node->content.height = height;
	node->content.width = width;
	node->next = NULL;
	return node;
}

void	appendNode(t_data *data, t_list *node)
{
	t_list *tmp = data->images;

	if (!tmp)
	{
		data->images = node;
		return ;
	}

	while (tmp->next)
	{
		tmp = tmp->next;
	}
	tmp->next = node;
}

void	deleteList(t_data *data)
{
	t_list *tmp;


	while (data->images)
	{
		tmp = data->images;
		data->images = data->images->next;
		mlx_destroy_window(data->mlx_ptr, tmp->content.win_ptr);
		mlx_destroy_image(data->mlx_ptr, tmp->content.img_ptr);
		free(tmp);
	}
}
