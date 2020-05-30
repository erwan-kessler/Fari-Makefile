#include "movie.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

c2k19_movie* c2k19_movie_create  (){
    c2k19_movie *c2K19Movie=(c2k19_movie*)malloc(sizeof(*c2K19Movie));
    c2K19Movie->id=0;
    c2K19Movie->genres=NULL;
    c2K19Movie->note=0.0;
    c2K19Movie->synopsis=NULL;
    c2K19Movie->title=NULL;
    c2K19Movie->year=0;
    return c2K19Movie;

}
void         c2k19_movie_destroy (c2k19_movie* movie){
    if (movie!=NULL){
        if (movie->synopsis!=NULL){
            free(movie->synopsis);
        }
        if(movie->title!=NULL){
            free(movie->title);
        }
        if (movie->genres!=NULL){
            c2k19_genres_destroy(movie->genres);
        }
        free(movie);
    }
}


/* --- display --- */

void c2k19_movie_display(c2k19_movie* movie) {
  if (!movie) return;
  
  printf("id: %d\n",       movie->id);
  printf("title: %s\n",    movie->title);
  c2k19_genres_display    (movie->genres);
  printf("synopsis: %s\n", movie->synopsis);
  printf("year: %u\n",     movie->year);
  printf("note: %f\n",     movie->note);
  printf("\n");
}


/* --- deep copy --- */

c2k19_movie* c2k19_movie_deepCopy(c2k19_movie* movie) {
  c2k19_movie* newMovie = c2k19_movie_create();
  
  newMovie->id       =                       movie->id;
  newMovie->title    =                strdup(movie->title);
  newMovie->genres   = c2k19_genres_deepCopy(movie->genres);
  newMovie->synopsis =                strdup(movie->synopsis);
  newMovie->year     =                       movie->year;
  newMovie->note     =                       movie->note;
  
  return newMovie;
}
