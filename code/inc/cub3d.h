#ifndef CUB3D_HPP
#define CUB3D_HPP

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <sys/time.h>

// MINILIBX
#include <X11/keysym.h>
#include <X11/X.h>
# include <X11/keysym.h>
# include "mlx.h"
# include "mlx_int.h"

// Libft
#include "libft.h"

#define WIDTH 800
#define HEIGHT 600

#define MOVESPEED 1.2 // tiles per second
#define ROTSPEED 1.2 // radians per second
#define FOV 0.66

// ERROR CODES
#define PARSE_SUCCESS 0
#define PARSE_ERROR 1
#define ARG_ERROR 1
#define FTYPE_ERROR 1
#define FEMPTY_ERROR 1
#define FINVALID_ERROR 1
#define MWRONG_TEXTURE 1
#define MWRONG_FORMAT 1

#define SYSCALL_ERROR 1

// ORIENTATION
enum orientation
{
    NO = 0,
    SO = 1,
    WE = 2,
    EA = 3,
    F = 4, // Floor
    C = 5, // Ceiling
    OTHER = 100
};

typedef struct s_dda
{
    // tlvz mudar para float
    int dx;
    int dy;
    int steps;
    float xinc;
    float x;
    float y;
    float yinc;
}               t_dda;


typedef struct s_ray
{
    double cameraX;
    double DirX;
    double DirY;
    int mapX;
    int mapY;
    double deltaDistX;
    double deltaDistY;
    int hit;
    int stepX;
    int stepY;
    double sideDistX;
    double sideDistY;
    double perpWallDist;
    int side;
    int drawStart;
    int drawEnd;
    int lineHeight;
    double wallX; // Where exactly the wall was hit
    int texX; // X coordinate on the texture
} t_ray;

typedef struct rgb
{
    // RGB ranges from 0-255
    uint8_t r;
    uint8_t g;
    uint8_t b;
}               t_rgb;

typedef struct map_assets
{
    //Paths
    char *no_texture;
    char *so_texture;
    char *we_texture;
    char *ea_texture;

    void *no;
    void *so;
    void *we;
    void *ea;

    // Add texture data
    int no_width;
    int so_width;
    int we_width;
    int ea_width;

    int no_height;
    int so_height;
    int we_height;
    int ea_height;

    //Color field String
    char *floor_rgb_s;
    char *ceiling_rgb_s;

    //Struct cores separadas
    t_rgb floor_color;
    t_rgb ceil_color;
}               t_assets;

typedef struct s_mlx_data
{
	void	*mlx_ptr;
	void	*win_ptr;
	void	*img_ptr;
	void	*img_addr;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
}				t_mlx_data;

typedef struct s_movement
{
    bool w_key;
    bool s_key;
    bool a_key;
    bool d_key;
    bool l_key;
    bool r_key;
}   t_moves;

typedef struct s_player
{
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
    t_moves moves;
} t_player;


typedef struct cub
{
    // File validation
    uint32_t nb_lines; //Number of lines in the file
    char **fcontent; // Array[][] line by line

    // Player
    t_player player;

    // Map
    char **map;
    int mapHeight;
    int *mapLineLens;

    // Texturas
    t_assets assets;
    
    // MLX stuff
    t_mlx_data mlx;

    struct timeval time;
    double deltaTime;
}       t_cub;

// MAP VALIDATION
uint8_t filetype_checker(char *path);
uint8_t isFileValid(char *path);
uint8_t isFileEmpty(char *path);
uint8_t isOrientation(t_cub *head, int i, int textures_parsed);
uint8_t textureValidator(t_cub *head);
uint8_t isXpm(char *str);

// MAP INIT
uint8_t getNbLines(char *path, t_cub *head);
uint8_t allocate_file(char ** av, t_cub *head);
uint8_t collect_lines(char *path, t_cub *head);

uint8_t allocateMap(t_cub *head, char **fcontent, char *map_path);
uint8_t verifyMap(t_cub *head);

void texture_loader(t_cub *head);

// Time Utils
double DeltaTime(struct timeval *old);

// Player movement
void PlayerForward(t_player *player, char **map, t_cub *head);
void PlayerBackward(t_player *player, char **map, t_cub *head);
void PlayerLeft(t_player *player, char **map, t_cub *head);
void PlayerRight(t_player *player, char **map, t_cub *head);
void PlayerRotateRight(t_cub *head, t_player *player);
void PlayerRotateLeft(t_cub *head, t_player *player);

// Ray caster
int Raycaster(t_cub *head);

// Draw utils
void DrawLine(t_mlx_data mlx, int x1, int y1, int x2, int y2, int color);
void PutPixelToImg(t_mlx_data mlx, int x, int y, int color);
void DrawVertPixelLine(t_cub *head, int color, t_ray *ray, int x);
void DrawTexturedVertLine(t_cub *head, t_ray *ray, int x);

// hooks
int KeyReleased(int key, t_cub *head);
int KeyPressed(int key, t_cub *head);
int UpdateLoop(t_cub *head);


// Free Funcs
void freeFile(t_cub *head);
void free_textures(t_cub *head);
void freeMap(t_cub *head);
void parseFailed(t_cub *head, uint16_t error);
int ExitFun(t_cub *head);


#endif