#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "movies.h"


static c2k19_movies* movies;
static c2k19_movie* movie1;
static c2k19_movie* movie2;
static c2k19_movie* movie3;


void setup() {
  movies = c2k19_movies_create();
}

void teardown() {
  c2k19_movies_destroy(movies);
}

void setup_movies() {
  movies = c2k19_movies_create();
  
  c2k19_genres* genres1 = c2k19_genres_create();
  c2k19_genres_append(genres1, strdup("1dummy1"));
  c2k19_genres_append(genres1, strdup("1dummy2")); 
  c2k19_genres_append(genres1, strdup("1dummy3")); 
  
  c2k19_genres* genres2 = c2k19_genres_create();
  c2k19_genres_append(genres2, strdup("2dummy1"));
  c2k19_genres_append(genres2, strdup("2dummy2")); 
  c2k19_genres_append(genres2, strdup("2dummy3")); 
  
  c2k19_genres* genres3 = c2k19_genres_create();
  c2k19_genres_append(genres3, strdup("1dummy1"));
  c2k19_genres_append(genres3, strdup("2dummy2")); 
  c2k19_genres_append(genres3, strdup("1dummy3")); 
  
  movie1 = c2k19_movie_create();
  movie1->id       = 1;
  movie1->title    = strdup("titre1");
  movie1->genres   = genres1;
  movie1->synopsis = strdup("synopsis1");
  movie1->year     = 1978;
  movie1->note     = 1.1;
    
  movie2 = c2k19_movie_create();
  movie2->id       = 2;
  movie2->title    = strdup("titre2");
  movie2->genres   = genres2;
  movie2->synopsis = strdup("synopsis2");
  movie2->year     = 1979;
  movie2->note     = 2.2;  
  
  movie3 = c2k19_movie_create();
  movie3->id       = 3;
  movie3->title    = strdup("titre3");
  movie3->genres   = genres3;
  movie3->synopsis = strdup("synopsis3");
  movie3->year     = 1979;
  movie3->note     = 3.3;
}


Test(c2k19_movies, create, .init = setup, .fini = teardown) {
  cr_assert_not_null(movies);
  cr_assert_null(movies->head);
}

Test(c2k19_movies, destroy, .init = setup) {
  c2k19_movies_destroy(movies);
  cr_assert(true);
}

Test(c2k19_movies, prepend, .init = setup_movies, .fini = teardown) {    
  c2k19_movies_prepend(movies, movie1);
  
  cr_assert_not_null(movies->head);
  cr_assert_not_null(movies->head->movie);
  cr_assert_eq      (movies->head->movie->id, 1);
  cr_assert_null    (movies->head->next);
  
  c2k19_movies_prepend(movies, movie2);
  
  cr_assert_not_null(movies->head);
  cr_assert_not_null(movies->head->movie);
  cr_assert_eq      (movies->head->movie->id, 2);
  cr_assert_not_null(movies->head->next);
  cr_assert_not_null(movies->head->next->movie);
  cr_assert_eq      (movies->head->next->movie->id, 1);
  cr_assert_null    (movies->head->next->next);
 
  c2k19_movies_prepend(movies, movie3);
  
  cr_assert_not_null(movies->head);
  cr_assert_not_null(movies->head->movie);
  cr_assert_eq      (movies->head->movie->id, 3);
  cr_assert_not_null(movies->head->next);
  cr_assert_not_null(movies->head->next->movie);
  cr_assert_eq      (movies->head->next->movie->id, 2);
  cr_assert_not_null(movies->head->next->next);
  cr_assert_not_null(movies->head->next->next->movie);
  cr_assert_eq      (movies->head->next->next->movie->id, 1);
  cr_assert_null    (movies->head->next->next->next);
}

Test(c2k19_movies, display, .init = setup_movies, .fini = teardown) {
  cr_redirect_stdout();
  
  c2k19_movies_prepend(movies, movie1);
  c2k19_movies_prepend(movies, movie2);
  c2k19_movies_prepend(movies, movie3);
  
  c2k19_movies_display(movies);
  fflush(stdout);
  
  cr_assert_stdout_eq_str("id: 3\ntitle: titre3\n[ 1dummy1 2dummy2 1dummy3 ]\nsynopsis: synopsis3\nyear: 1979\nnote: 3.300000\n\nid: 2\ntitle: titre2\n[ 2dummy1 2dummy2 2dummy3 ]\nsynopsis: synopsis2\nyear: 1979\nnote: 2.200000\n\nid: 1\ntitle: titre1\n[ 1dummy1 1dummy2 1dummy3 ]\nsynopsis: synopsis1\nyear: 1978\nnote: 1.100000\n\n");
}

Test(c2k19_movies, findById, .init = setup_movies, .fini = teardown) {
  c2k19_movies_prepend(movies, movie1);
  c2k19_movies_prepend(movies, movie2);
  c2k19_movies_prepend(movies, movie3);
  
  c2k19_movie* movie = c2k19_movies_findById(movies, 2);
  
  cr_assert_not_null(movie);
  cr_assert_eq      (movie->id,             2);
  cr_assert_str_eq  (movie->title,          "titre2");
  cr_assert_not_null(movie->genres);
  cr_assert_eq      (movie->genres->size,   3);
  cr_assert_str_eq  (movie->genres->tab[0], "2dummy1");
  cr_assert_str_eq  (movie->synopsis,       "synopsis2");
  cr_assert_eq      (movie->year,           1979);
  cr_assert_float_eq(movie->note,           2.2, 0.1);
}

Test(c2k19_movies, findByTitle, .init = setup_movies, .fini = teardown) {
  c2k19_movies_prepend(movies, movie1);
  c2k19_movies_prepend(movies, movie2);
  c2k19_movies_prepend(movies, movie3);
  
  c2k19_movie* movie = c2k19_movies_findByTitle(movies, "titre1");
  
  cr_assert_not_null(movie);
  cr_assert_eq      (movie->id, 1);
}

Test(c2k19_movies, findByYear, .init = setup_movies, .fini = teardown) {
  c2k19_movies_prepend(movies, movie1);
  c2k19_movies_prepend(movies, movie2);
  c2k19_movies_prepend(movies, movie3);
  
  c2k19_movies* results = c2k19_movies_findByYear(movies, 1979);
  
  cr_assert_not_null(results);
  cr_assert_not_null(results->head);
  cr_assert_eq      (results->head->movie->id,       2);
  cr_assert_eq      (results->head->next->movie->id, 3);
  
  c2k19_movies_destroy(results);
}

Test(c2k19_movies, findByGenre, .init = setup_movies, .fini = teardown) {
  c2k19_movies_prepend(movies, movie1);
  c2k19_movies_prepend(movies, movie2);
  c2k19_movies_prepend(movies, movie3);
  
  c2k19_movies* results = c2k19_movies_findByGenre(movies, "2dummy2");
  
  cr_assert_not_null(results);
  cr_assert_not_null(results->head);
  cr_assert_eq      (results->head->movie->id,       2);
  cr_assert_eq      (results->head->next->movie->id, 3);
  
  c2k19_movies_destroy(results);
}

Test(c2k19_movies, findByGenres, .init = setup_movies, .fini = teardown) {
  c2k19_movies_prepend(movies, movie1);
  c2k19_movies_prepend(movies, movie2);
  c2k19_movies_prepend(movies, movie3);
  
  c2k19_movies* results = c2k19_movies_findByGenres(movies, (const char *[]){"1dummy1", "1dummy3"}, 2);
  
  cr_assert_not_null(results);
  cr_assert_not_null(results->head);
  cr_assert_eq      (results->head->movie->id,       1);
  cr_assert_eq      (results->head->next->movie->id, 3);
  
  c2k19_movies_destroy(results);
}
