#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "genres.h"


#define GENRE1 "dummy1"
#define GENRE2 "dummy2"

static c2k19_genres* genres;


void setup() {
  genres = c2k19_genres_create();
}

void teardown() {
  c2k19_genres_destroy(genres);
}

void setup_genres() {
  genres = c2k19_genres_create();
  c2k19_genres_append(genres, strdup(GENRE1));
  c2k19_genres_append(genres, strdup(GENRE2));
}


Test(c2k19_genres, create, .init = setup, .fini = teardown) {  
  cr_assert_not_null(genres);
  cr_assert_eq(genres->size, 0);
}

Test(c2k19_genres, destroy, .init = setup) {
  c2k19_genres_destroy(genres);
  cr_assert(true);
}

Test(c2k19_genres, append, .init = setup, .fini = teardown) {
  c2k19_genres_append(genres, strdup(GENRE1));
  
  cr_assert_eq(genres->size, 1);
  cr_assert_str_eq(genres->tab[0], GENRE1);
  
  c2k19_genres_append(genres, strdup(GENRE2));
  
  cr_assert_eq(genres->size, 2);
  cr_assert_str_eq(genres->tab[1], GENRE2);
}

Test(c2k19_genres, contains, .init = setup, .fini = teardown) {
  c2k19_genres_append(genres, strdup(GENRE1));
  
  cr_assert    (c2k19_genres_contains(genres, GENRE1));
  cr_assert_not(c2k19_genres_contains(genres, GENRE2));
}

Test(c2k19_genres, display, .init = setup_genres, .fini = teardown) {
  cr_redirect_stdout();
    
  c2k19_genres_display(genres);
  fflush(stdout);
  
  cr_assert_stdout_eq_str("[ dummy1 dummy2 ]\n");
}

Test(c2k19_genres, deepCopy, .init = setup_genres, .fini = teardown) {
  c2k19_genres* newGenres = c2k19_genres_deepCopy(genres);
    
  cr_assert_not_null(newGenres);
  cr_assert_eq      (newGenres->size,   2);
  cr_assert_str_eq  (newGenres->tab[0], "dummy1");
  cr_assert_str_eq  (newGenres->tab[1], "dummy2");
  
  c2k19_genres_destroy(newGenres);
  
  cr_assert_not_null(genres);
  cr_assert_eq      (genres->size,   2);
  cr_assert_str_eq  (genres->tab[0], "dummy1");
  cr_assert_str_eq  (genres->tab[1], "dummy2");
}
