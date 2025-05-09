#include "cub3d.h"


// Converte estrutura com canais de cor separadas para um int
int split_to_color(t_rgb color)
{
    int result;

    result = (color.r << 16) | (color.g << 8) | color.b;
    return result;
}

void DrawTexturedVertLine(t_cub *head, t_ray *ray, int x)
{
    void *texture;
    int texWidth, texHeight;
    
    // TEXTURA ORIENTADA POR DIRECAO
    if (ray->side == 0) {
        texture = ray->DirX > 0 ? head->assets.ea : head->assets.we;
        texWidth = ray->DirX > 0 ? head->assets.ea_width : head->assets.we_width;
        texHeight = ray->DirX > 0 ? head->assets.ea_height : head->assets.we_height;
    } else {
        texture = ray->DirY > 0 ? head->assets.so : head->assets.no;
        texWidth = ray->DirY > 0 ? head->assets.so_width : head->assets.no_width;
        texHeight = ray->DirY > 0 ? head->assets.so_height : head->assets.no_height;
    }

    // WALL HIT POSITION 
    double wallX = ray->side == 0 ? 
        head->player.posY + ray->perpWallDist * ray->DirY :
        head->player.posX + ray->perpWallDist * ray->DirX;
    wallX -= floor(wallX);

    // Coordenada X da textura
    int texX = (int)(wallX * (double)texWidth);
    texX = texX < 0 ? 0 : (texX >= texWidth ? texWidth - 1 : texX);

    // Ceiling
    DrawLine(head->mlx, x, 0, x, ray->drawStart - 1, 
             split_to_color(head->assets.ceil_color));

    // GET TEXTURA IMG E DATA
    int *textureData;
    int size_line;
    textureData = (int*)mlx_get_data_addr(texture, 
                                          &head->mlx.bits_per_pixel, 
                                          &size_line, 
                                          &head->mlx.endian);

    // COORDENADA Y DA TEXTURA
    double step = 1.0 * (texHeight - 1) / ray->lineHeight;
    double texPos = (ray->drawStart - HEIGHT / 2 + ray->lineHeight / 2) * step;

    for(int y = ray->drawStart; y < ray->drawEnd; y++)
    {
        int texY = (int)texPos & (texHeight - 1);
        texPos += step;

        // COR DO PIXEL A SER PINTADO
        int color = textureData[texY * texWidth + texX];
        
        // CHECAR ANTES DE ENTREGAR - SHADOW WALL
        if(ray->side == 1) 
            color = (color >> 1) & 8355711;

        PutPixelToImg(head->mlx, x, y, color);
    }

    // Floor
    DrawLine(head->mlx, x, ray->drawEnd, x, HEIGHT, split_to_color(head->assets.floor_color));
}