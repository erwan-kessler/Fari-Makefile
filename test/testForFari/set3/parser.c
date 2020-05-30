#include "parser.h"
#include "movie.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static c2k19_movie*  c2k19_parser_parseCsvLine  (char* line);
static c2k19_genres* c2k19_parser_parseCsvGenres(char* field);


c2k19_movies* c2k19_parser_parseCsvFile(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Failed to open file.\n");
    exit(1);
  }
  
  c2k19_movies* movies = c2k19_movies_create();

  while (!feof(file)) {
    char line[1024];
    int numchar = fscanf(file, " %[^\n]s ", line);
    if (numchar != 1) continue;

    c2k19_movie* movie = c2k19_parser_parseCsvLine(line);
    c2k19_movies_prepend(movies, movie);
  }

  fclose(file);
  return movies;
}


static c2k19_movie* c2k19_parser_parseCsvLine(char* line) {
  c2k19_movie* movie = c2k19_movie_create();
  
  int   field_index = 0;
  char* field_value;
  
  while ((field_value = strsep(&line, ";"))) {
    field_index += 1;
    switch(field_index) {
      case 1:
        movie->id = atoi(field_value);
        break;
      case 2:
        movie->title = strdup(field_value);
        break;
      case 3:
        movie->genres = c2k19_parser_parseCsvGenres(field_value);
        break;
      case 4:
        movie->synopsis = strdup(field_value);
        break;
      case 6:
        sscanf(field_value, "%*d/%*d/%d", &movie->year);
        break;
      case 14:
        movie->note = atof(field_value);
        break;
      default:
        break;
    }
  }
  
  return movie;
}


static c2k19_genres* c2k19_parser_parseCsvGenres(char* field) {
  c2k19_genres* genres = c2k19_genres_create();
  
  char* genre;
  while ((genre = strsep(&field, ","))) {
    c2k19_genres_append(genres, strdup(genre));
  }
  
  return genres;
}
