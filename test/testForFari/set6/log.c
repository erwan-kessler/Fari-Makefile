#include "log.h"
#include "log_aux.h"
int main (){
   double x, ret; 
   x = 2.7;

   /* finding log(2.7) */
   ret = log(x); 
   printf("log(%lf) = %lf\n", x, ret);

   return(0); 
} 
