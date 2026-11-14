/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: KarmaFaber <github.com/KarmaFaber>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 13:09:34 by KarmaFaber        #+#    #+#             */
/*   Updated: 2026/11/14 01:04:41 by KarmaFaber       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Una función simulada de split/duplicación con malloc para ver dobles punteros
char **allocate_words(void)
{
    char **tab;

    // Pedimos memoria para 3 punteros de strings (doble puntero) usando malloc
    tab = malloc(sizeof(char *) * 4);
    if (!tab)
        return (NULL);

    tab[0] = malloc(10); // TINY allocation
    tab[1] = malloc(20); // TINY allocation
    tab[2] = malloc(5000); // SMALL o LARGE según tus umbrales
    tab[3] = NULL;

    if (tab[0]) strcpy(tab[0], "42");
    if (tab[1]) strcpy(tab[1], "Madrid");
    if (tab[2]) strcpy(tab[2], "OuterCore");

    return (tab);
}

int main(void)
{
    char **words;
    int i;

    printf("--- Test de Dobles Punteros y Malloc ---\n");
    words = allocate_words();
    if (!words)
    {
        printf("Error de asignación\n");
        return (1);
    }

    i = 0;
    while (words[i])
    {
        printf("Palabra[%d]: %s (dir: %p)\n", i, words[i], (void *)words[i]);
        i++;
    }

    // Liberamos la memoria (cuando implementemos free)
    i = 0;
    while (words[i])
    {
        free(words[i]);
        i++;
    }
    free(words);

    printf("--- Test finalizado con éxito ---\n");
    return (0);
}