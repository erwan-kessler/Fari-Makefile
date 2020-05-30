#pragma once

typedef struct _c2k19_genres {
  char** tab;
  int    size;
} c2k19_genres;

c2k19_genres* c2k19_genres_create  ();
void          c2k19_genres_destroy (c2k19_genres* genres);
void          c2k19_genres_append  (c2k19_genres* genres, char* genre);
int           c2k19_genres_contains(c2k19_genres* genres, const char* genre);
void          c2k19_genres_display (c2k19_genres* genres);
c2k19_genres* c2k19_genres_deepCopy(c2k19_genres* genres);
