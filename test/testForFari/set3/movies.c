#include "movies.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* --- create / destroy --- */

c2k19_movies *c2k19_movies_create() {
    c2k19_movies *c2K19Movies = (c2k19_movies *) malloc(sizeof(*c2K19Movies));
    if (c2K19Movies == NULL) {
        fprintf(stderr, "malloc error");
        exit(-1);
    }
    c2K19Movies->head = NULL;
    return c2K19Movies;
}

void c2k19_movies_destroy(c2k19_movies *movies) {
    c2k19_moviesItem *current = movies->head;
    while (current != NULL) {
        c2k19_moviesItem *next = current->next;
        c2k19_movie_destroy(current->movie);
        free(current);
        current = next;
    }
    free(movies);

}

void c2k19_movies_prepend(c2k19_movies *movies, c2k19_movie *movie) {
    c2k19_moviesItem *current = movies->head;

    c2k19_moviesItem *c2K19MoviesItem = (c2k19_moviesItem *) malloc(sizeof(*c2K19MoviesItem));
    if (c2K19MoviesItem == NULL) {
        fprintf(stderr, "malloc error");
        exit(-1);
    }
    c2K19MoviesItem->movie = movie;
    c2K19MoviesItem->next = current;
    movies->head = c2K19MoviesItem;

}


c2k19_movie *c2k19_movies_findById(c2k19_movies *movies, int id) {
    c2k19_moviesItem *current = movies->head;
    while (current->next != NULL) {
        if (current->movie->id == id) {
            return current->movie;
        }
        current = current->next;
    }
    if (current->next == NULL && movies->head->movie->id == id) {
        return movies->head->movie;
    }
    return NULL;
}

c2k19_movie *c2k19_movies_findByTitle(c2k19_movies *movies, const char *title) {
    c2k19_moviesItem *current = movies->head;
    while (current != NULL) {
        if (strcmp(current->movie->title, title) == 0) {
            return current->movie;
        }
        current = current->next;
    }
    return NULL;
}

c2k19_movies *c2k19_movies_findByYear(c2k19_movies *movies, unsigned int year) {
    c2k19_movies *list = c2k19_movies_create();
    c2k19_moviesItem *current = movies->head;
    while (current != NULL) {
        if (current->movie->year == (int) year) {
            c2k19_movies_prepend(list, c2k19_movie_deepCopy(current->movie));
        }
        current = current->next;
    }
    return list;
}

c2k19_movies *c2k19_movies_findByGenre(c2k19_movies *movies, const char *genre) {
    c2k19_movies *list = c2k19_movies_create();
    c2k19_moviesItem *current = movies->head;
    while (current != NULL) {
        if (c2k19_genres_contains(current->movie->genres, genre)) {
            c2k19_movies_prepend(list, c2k19_movie_deepCopy(current->movie));
        }
        current = current->next;
    }
    return list;
}

c2k19_movies *c2k19_movies_findByGenres(c2k19_movies *movies, const char *genres[], int genres_size) {
    c2k19_movies *list = c2k19_movies_create();
    c2k19_moviesItem *current = movies->head;
    while (current != NULL) {
        int bool = 0;
        for (int i = 0; i < genres_size; i++) {
            if (bool == 0 && c2k19_genres_contains(current->movie->genres, genres[i])) {
                c2k19_movies_prepend(list, c2k19_movie_deepCopy(current->movie));
                bool = 1;
            }
        }
        current = current->next;
    }
    return list;
}




/* --- prepend --- */

// A COMPLETER //


/* --- display --- */

void c2k19_movies_display(c2k19_movies *movies) {
    if (!movies) return;

    c2k19_moviesItem *item = movies->head;
    while (item) {
        c2k19_movie_display(item->movie);
        item = item->next;
    }
}


/* --- finders --- */

// A COMPLETER //
