#include "genres.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

c2k19_genres *c2k19_genres_create() {
    c2k19_genres *c2K19Genres = (c2k19_genres *) malloc(sizeof(c2k19_genres));
    c2K19Genres->size = 0;
    c2K19Genres->tab = NULL;
    return c2K19Genres;
}

void c2k19_genres_destroy(c2k19_genres *genres) {
    for (int i = 0; i < genres->size; i++) {
        free(genres->tab[i]);
    }
    free(genres->tab);
    free(genres);
}

void c2k19_genres_append(c2k19_genres *genres, char *genre) {
    genres->size++;
    if (genres->tab == NULL) {
        genres->tab = (char **) calloc(genres->size, sizeof(char *));
        if (genres->tab == NULL) {
            fprintf(stderr, "Calloc failed");
            exit(-1);
        }
    } else {

        char **temp = (char**)realloc(genres->tab, sizeof(char *) * genres->size);
        if (temp == NULL) {
            fprintf(stderr, "Calloc failed");
            exit(-1);
        }
        genres->tab=temp;
    }
    genres->tab[genres->size - 1] = genre;
}

int c2k19_genres_contains(c2k19_genres *genres, const char *genre) {
    for (int i = 0; i < genres->size; i++) {
        if (strcmp(genres->tab[i], genre) == 0) {
            return 1;
        }
    }
    return 0;
}


/* --- display --- */

void c2k19_genres_display(c2k19_genres *genres) {
    if (!genres) return;

    printf("[ ");
    for (int i = 0; i < genres->size; ++i)
        printf("%s ", genres->tab[i]);
    printf("]\n");
}


/* --- deep copy --- */

c2k19_genres *c2k19_genres_deepCopy(c2k19_genres *genres) {
    c2k19_genres *newGenres = c2k19_genres_create();

    for (int i = 0; i < genres->size; ++i)
        c2k19_genres_append(newGenres, strdup(genres->tab[i]));

    return newGenres;
}
