#include "cub3d.h"


static bool        isMapLine(char * line)
{
    while (*line && ft_isspace(*line)) 
        line++;

    if (ft_strncmp(line, "NO", 2) != 0 && ft_strncmp(line, "SO", 2) != 0 &&
    ft_strncmp(line, "WE", 2) != 0 && ft_strncmp(line, "EA", 2) != 0 &&
    ft_strncmp(line, "F", 1) != 0 && ft_strncmp(line, "C", 1) != 0 && 
    !ft_stremptyspaces(line))
    {
        // estou no mapa
        return true;
    }
    return false;
}

static int         getMapHeight(char *map_path)
{
    int fd = open(map_path, O_RDONLY);
    if (!fd)
        return (-1);
    
    char *line = get_next_line(fd);
    while (line && !isMapLine(line))
    {
        free(line);
        line = get_next_line(fd);
        if (!line) // end of file reached
            return (close(fd), -1); // no map
    }
    
    int h = 0;
    bool flag = false; // error checking flag
    while (line)
    {
        if (ft_stremptyspaces(line))
        {
            free(line);
            flag = true; // will continue to read from file to check for anything but whitespaces
            break ; // prevents empty lines to be added to the map
        }
        free(line);
        line = get_next_line(fd);
        h++;
    }

    // check to see if anything past the end of the map is only spaces or if theres a space inbetween the map
    if (flag)
    {
        line = get_next_line(fd);
        while (line)
        {
            if (!ft_stremptyspaces(line))
                h = -1; // error for spaced map (2 maps)
            free(line);
            line = get_next_line(fd);
        }
    }
    close (fd);
    return h;
}


static int     allocateRows(t_cub *head, char **fcontent, int height)
{
    u_int32_t lidx = 0;
    while (lidx < head->nb_lines && !isMapLine(fcontent[lidx]))
        lidx++;

    // dupping the map
    int i = 0;
    while (i < height)
    {
        // size da linha do mapa excluindo a \n no fim
        head->map[i] = ft_strdup(fcontent[lidx]);
        if (!head->map[i])
            return (SYSCALL_ERROR);

        // next line
        i++;
        lidx++;
    }
return (PARSE_SUCCESS);
}

uint8_t     allocateMap(t_cub *head, char **fcontent, char *map_path)
{
    head->mapHeight = getMapHeight(map_path); 

    // something after the map or small map
    if (head->mapHeight < 3) return MWRONG_FORMAT;

    head->mapLineLens = (int *)malloc(sizeof(int) * head->mapHeight);

    // map lines from 0 ends in NULL
    head->map = (char **)ft_calloc(head->mapHeight + 1, sizeof(char *));
    if (!head->map)
        return SYSCALL_ERROR;

    // copies each line
    if (allocateRows(head, fcontent, head->mapHeight))
        return (MWRONG_FORMAT);

    for (int i = 0; i < head->mapHeight; i++)
        head->mapLineLens[i] = ft_strlen(head->map[i]);
    
    return (PARSE_SUCCESS);
}