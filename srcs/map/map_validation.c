/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:08:31 by davi              #+#    #+#             */
/*   Updated: 2025/01/09 16:42:56 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

// Ends in .cub?
uint8_t filetype_checker(char *path)
{
    uint32_t i;

    i = ft_strlen(path);
    if(i <= 4)
        return (FTYPE_ERROR);
    
    // *.cub
    i = i - 4;
    if (ft_strncmp(path + i, ".cub", 4) != 0)
        return (FTYPE_ERROR);

    return (PARSE_SUCCESS);
}
// Verifica se arquivo existe pelo fd retornado
uint8_t isFileValid(char *path)
{
    int32_t fd;

    fd = open(path, O_RDONLY);
    if (fd == -1)
        return (FINVALID_ERROR);
    close(fd);
    return (PARSE_SUCCESS);
}
// Verifica se o arquivo esta vazio
uint8_t isFileEmpty(char *path)
{
    int32_t fd;
    uint8_t has_content;

    fd = open(path, O_RDONLY);
    if (fd == -1)
        return (FEMPTY_ERROR);
    
    // Trying to read 1 byte from the file
    if (read(fd, &has_content, 1) == 0)
        return (close(fd), FEMPTY_ERROR);

    close(fd);
    return (PARSE_SUCCESS);
}

// Validacao do path das texturas
uint8_t isTextureValid(t_cub *head) //todo printfs
{
    if (isFileValid(head->assets.no_texture) || isXpm(head->assets.no_texture))
    {
        printf("[NAO VALIDADO]: NO %s\n", strerror(errno));
        return (MWRONG_TEXTURE);
    }
    if (isFileValid(head->assets.so_texture) || isXpm(head->assets.so_texture))
    {
        printf("[NAO VALIDADO]: SO %s\n", strerror(errno));
        return (MWRONG_TEXTURE);
    }
    if (isFileValid(head->assets.we_texture) || isXpm(head->assets.we_texture))
    {
        printf("[NAO VALIDADO]: WE %s\n", strerror(errno));
        return (MWRONG_TEXTURE);
    }
    if (isFileValid(head->assets.ea_texture) || isXpm(head->assets.ea_texture))
    {
        printf("[NAO VALIDADO]: EA %s\n", strerror(errno));
        //printf("EA PATH: %s\n", head->assets.ea_texture); // Debug print
        return (MWRONG_TEXTURE);
    }
    printf("[TEXTURAS VALIDADAS]\n");
    return (PARSE_SUCCESS);
}

// Validar RGB em formato de string
uint8_t isColorRgbstring(char *str)
{
    uint32_t i;
    uint32_t channel; // 3 espectros/canais r g b
    uint32_t nb_per_channel;

    channel = 0;
    nb_per_channel = 0;
    i = 0;
    if (str == NULL)
        return (PARSE_ERROR);
    while (channel < 3)
    {
        // spaces
        while (ft_isspace(str[i]))
            i++;

        // number?
        while (str[i] != ',' && str[i] != '\0')
        {
            if (!ft_isdigit(str[i]))
                return (MWRONG_TEXTURE);
            nb_per_channel++;
            i++;
        }
        
        // TODO DAVI WTF IS THIS COMMENT BETTER PLSSSSS
        if ((nb_per_channel == 0 && channel != 0) || nb_per_channel > 3) //O Primeiro canal pode ser vazio se possuir virgula
            return (MWRONG_TEXTURE);
        
        // next channel?
        //TODO consegues limpar isto?
        if (str[i] == ',')
        {
            nb_per_channel = 0;
            channel++;
            i++;
        }
        else if (str[i] == '\0' && channel == 2)
            channel++;
        else
            return (MWRONG_TEXTURE);
    }

    //? debug
    printf("[IS COLOR VALID]: YES\n");
    return (0);
}

// Parser func de RGB
uint8_t isColorValid(t_cub *head)
{
    if(isColorRgbstring(head->assets.floor_rgb_s)
    || isColorRgbstring(head->assets.ceiling_rgb_s))
        return (MWRONG_TEXTURE);

    t_rgb *fcolor = &(head->assets.floor_color);
    t_rgb *ccolor = &(head->assets.ceil_color);

    fcolor->r = ft_atoi(head->assets.floor_rgb_s); // Vai parar na primeira virgula
    fcolor->g = ft_atoi(ft_strchr(head->assets.floor_rgb_s, ',') + 1);
    fcolor->b = ft_atoi(ft_strrchr(head->assets.floor_rgb_s, ',') + 1);

    ccolor->r = ft_atoi(head->assets.ceiling_rgb_s); // Vai parar na primeira virgula
    ccolor->g = ft_atoi(ft_strchr(head->assets.ceiling_rgb_s, ',') + 1); // Retorna primeira virgula e adiciono mais 1
    ccolor->b = ft_atoi(ft_strrchr(head->assets.ceiling_rgb_s, ',') + 1); // Retorno a utlima virgula e adiciono mais 1

    //? debug
    {
        printf("[ISCOLORVALID] INT FLOOR COLORS %u %u %u\n", fcolor->r, head->assets.floor_color.g, head->assets.floor_color.b);
        printf("[ISCOLORVALID] INT Ceiling COLORS %u %u %u\n", ccolor->r, head->assets.ceil_color.g, head->assets.ceil_color.b);
        printf("[ISCOLORVALID] Sucesso em ambos!\n");
    }    
    return (0);
}
// Da parse e valida Texturas e cores
uint8_t textureValidator(t_cub *head)
{
    uint8_t i;
    u_int32_t textures_parsed;

    textures_parsed = 0;
    i = 0;
    while (i < head->nb_lines && textures_parsed != 6)
    {
        errno = 0;
        // if(isOrientation(&head->fcontent[i], head, textures_parsed) != OTHER)
        if(isOrientation(head, i, textures_parsed) != OTHER)
        {
            if (errno != 0)
                return (SYSCALL_ERROR);
            textures_parsed++;
        }
        i++;
    }

    // All need to be present
    if (textures_parsed != 6)
        return (MWRONG_TEXTURE);

    //? debug
    {
        printf("[NO PATH]: %s#\n", head->assets.no_texture);
        printf("[SO PATH]: %s#\n", head->assets.so_texture);
        printf("[WE PATH]: %s#\n", head->assets.we_texture);
        printf("[EA PATH]: %s#\n", head->assets.ea_texture);
        printf("[FLOOR COLOR STRING]: %s\n", head->assets.floor_rgb_s);
        printf("[Ceiling COLOR STRING]: %s\n", head->assets.ceiling_rgb_s);
    }
    
    if (isTextureValid(head)) return (MWRONG_TEXTURE);
    if (isColorValid(head)) return (MWRONG_TEXTURE);

    printf("PASSOU TEXTURE VALIDATOR\n");
    return (0);
}


