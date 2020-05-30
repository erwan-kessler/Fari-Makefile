#pragma once

#include "genres.h"

typedef struct _c2k19_movie {
  int           id;
  char*         title;
  c2k19_genres* genres;
  char*         synopsis;
  int           year;
  float         note;
} c2k19_movie;

c2k19_movie* c2k19_movie_create  ();
void         c2k19_movie_destroy (c2k19_movie* movie);
void         c2k19_movie_display (c2k19_movie* movie);
c2k19_movie* c2k19_movie_deepCopy(c2k19_movie* movie);
