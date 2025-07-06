#include "retromfa.h"

void exit_mlx(t_mfa *mfa) {
    if(mfa->win_ptr){

        mlx_destroy_window(mfa->mlx_ptr, mfa->win_ptr);
        free(mfa->win_ptr);
    }
    if(mfa->mlx_ptr){
        mlx_destroy_display(mfa->mlx_ptr);
        free(mfa->mlx_ptr);
    }
    all_free(); // Free all allocated memory
}

int key_hook(int keycode, t_mfa *mfa) {
    if (keycode == ESC_KEY)
        mlx_loop_end(mfa->mlx_ptr);
    return 0;
}
