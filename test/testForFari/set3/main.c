#include "genres.h"
#include "movie.h"
#include "movies.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s file.csv\n", argv[0]);
    exit(-1);
  }

  c2k19_movies* movies = c2k19_parser_parseCsvFile(argv[1]);
  c2k19_movies_display(movies);
  c2k19_movies_destroy(movies);
}
