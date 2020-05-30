#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "movie.h"


static c2k19_movie* movie;


void setup() {
  movie = c2k19_movie_create();
}

void teardown() {
  c2k19_movie_destroy(movie);
}

void setup_movie() {
  movie = c2k19_movie_create();
  
  c2k19_genres* genres = c2k19_genres_create();
  c2k19_genres_append(genres, strdup("dummy1"));
  c2k19_genres_append(genres, strdup("dummy2"));  
  
  movie->id       = 1;
  movie->title    = strdup("titre");
  movie->genres   = genres;
  movie->synopsis = strdup("synopsis");
  movie->year     = 1978;
  movie->note     = 0.0;
}


Test(c2k19_movie, create, .init = setup, .fini = teardown) {
  cr_assert_not_null(movie);
  
  cr_assert_eq  (movie->id, 0);
  cr_assert_null(movie->title);
  cr_assert_null(movie->genres);
  cr_assert_null(movie->synopsis);
  cr_assert_eq  (movie->year, 0);
  cr_assert_eq  (movie->note, 0.0);
}

Test(c2k19_movie, destroy, .init = setup) {
  c2k19_movie_destroy(movie);
  cr_assert(true);
}

Test(c2k19_movie, display, .init = setup_movie, .fini = teardown) {
  cr_redirect_stdout();
  
  c2k19_movie_display(movie);
  fflush(stdout);
  
  cr_assert_stdout_eq_str("id: 1\ntitle: titre\n[ dummy1 dummy2 ]\nsynopsis: synopsis\nyear: 1978\nnote: 0.000000\n\n");
}

Test(c2k19_movie, deepCopy, .init = setup_movie, .fini = teardown) {
  c2k19_movie* newMovie = c2k19_movie_deepCopy(movie);
  
  cr_assert_not_null(newMovie);
  cr_assert_eq      (newMovie->id,             1);
  cr_assert_str_eq  (newMovie->title,          "titre");
  cr_assert_not_null(newMovie->genres);
  cr_assert_eq      (newMovie->genres->size,   2);
  cr_assert_str_eq  (newMovie->genres->tab[0], "dummy1");
  cr_assert_str_eq  (newMovie->synopsis,       "synopsis");
  cr_assert_eq      (newMovie->year,           1978);
  cr_assert_float_eq(newMovie->note,           0.0, 0.1);
  
  c2k19_movie_destroy(newMovie);
  
  cr_assert_not_null(movie);
  cr_assert_eq      (movie->id,             1);
  cr_assert_str_eq  (movie->title,          "titre");
  cr_assert_not_null(movie->genres);
  cr_assert_eq      (movie->genres->size,   2);
  cr_assert_str_eq  (movie->genres->tab[0], "dummy1");
  cr_assert_str_eq  (movie->synopsis,       "synopsis");
  cr_assert_eq      (movie->year,           1978);
  cr_assert_float_eq(movie->note,           0.0, 0.1);
}
