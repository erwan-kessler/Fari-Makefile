#pragma once

#include "movie.h"

typedef struct _c2k19_moviesItem {
  c2k19_movie*              movie;
  struct _c2k19_moviesItem* next;
} c2k19_moviesItem;

typedef struct _c2k19_movies {
  c2k19_moviesItem* head;
} c2k19_movies;

c2k19_movies* c2k19_movies_create      ();
void          c2k19_movies_destroy     (c2k19_movies* movies);
void          c2k19_movies_prepend     (c2k19_movies* movies, c2k19_movie* movie);
void          c2k19_movies_display     (c2k19_movies* movies);
c2k19_movie*  c2k19_movies_findById    (c2k19_movies* movies, int id);
c2k19_movie*  c2k19_movies_findByTitle (c2k19_movies* movies, const char* title);
c2k19_movies* c2k19_movies_findByYear  (c2k19_movies* movies, unsigned int year);
c2k19_movies* c2k19_movies_findByGenre (c2k19_movies* movies, const char* genre);
c2k19_movies* c2k19_movies_findByGenres(c2k19_movies* movies, const char* genres[], int genres_size);

