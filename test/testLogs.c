#define SNOW_ENABLED

#include "../src/json_logs.h"
#include "../snow/snow.h"
#include "../src/Parser.h"

describe(Log_test){
    it("Create sources log"){
        char *filename = "testForLogs/set1/farifile";
        data_t * data = parser(filename);
        int * errCode = malloc(sizeof(int));
        *errCode=0;
        char* compare="{\n"
					  "\t\"sources\":\t[\"./testForLogs/set1/a.c\", \"./testForLogs/set1/b.c\", \"./testForLogs/set1/c.c\"],\n"
					  "\t\"headers\":\t[],\n"
					  "\t\"libraries\":\t[],\n"
					  "\t\"flags\":\t[],\n"
					  "\t\"executable_name\":\t\"MyAwesomeExecutable\",\n"
					  "\t\"fari_error_msg\":\t\"\",\n"
					  "\t\"compilation\":\t{\n"
					  "\t\t\"commands\":\t[\"gcc -c ./testForLogs/set1/a.c -o ./testForLogs/set1/a.o\", \"gcc -c ./testForLogs/set1/b.c -o ./testForLogs/set1/b.o\", \"gcc -c ./testForLogs/set1/c.c -o ./testForLogs/set1/c.o\"],\n"
					  "\t\t\"error_msg\":\t\"\"\n"
					  "\t},\n"
					  "\t\"linking\":\t{\n"
					  "\t\t\"command\":\t\"gcc -o MyAwesomeExecutable ./testForLogs/set1/a.o ./testForLogs/set1/b.o ./testForLogs/set1/c.o\",\n"
					  "\t\t\"error_msg\":\t\"\"\n"
					  "\t},\n"
					  "\t\"fari_msg\":\t\"Compilation terminée.\"\n"
					  "}";
        asserteq_str(create_log(data,0,errCode),compare);
    }
    it("Create sources and headers log"){
        char *filename = "testForLogs/set2/farifile";
        data_t * data = parser(filename);
        int * errCode = malloc(sizeof(int));
        *errCode=0;
        char* compare="{\n"
					  "\t\"sources\":\t[\"./testForLogs/set2/a.c\", \"./testForLogs/set2/b.c\", \"./testForLogs/set2/c.c\"],\n"
					  "\t\"headers\":\t[\"./testForLogs/set2/b.h\", \"./testForLogs/set2/a.h\", \"./testForLogs/set2/c.h\", \"./testForLogs/set2/h.h\"],\n"
					  "\t\"libraries\":\t[],\n"
					  "\t\"flags\":\t[],\n"
					  "\t\"executable_name\":\t\"MyAwesomeExecutable\",\n"
					  "\t\"fari_error_msg\":\t\"\",\n"
					  "\t\"compilation\":\t{\n"
					  "\t\t\"commands\":\t[\"gcc -c ./testForLogs/set2/a.c -o ./testForLogs/set2/a.o\", \"gcc -c ./testForLogs/set2/b.c -o ./testForLogs/set2/b.o\", \"gcc -c ./testForLogs/set2/c.c -o ./testForLogs/set2/c.o\"],\n"
					  "\t\t\"error_msg\":\t\"\"\n"
					  "\t},\n"
					  "\t\"linking\":\t{\n"
					  "\t\t\"command\":\t\"gcc -o MyAwesomeExecutable ./testForLogs/set2/a.o ./testForLogs/set2/b.o ./testForLogs/set2/c.o\",\n"
					  "\t\t\"error_msg\":\t\"\"\n"
					  "\t},\n"
					  "\t\"fari_msg\":\t\"Compilation terminée.\"\n"
					  "}";
        asserteq_str(create_log(data,0,errCode),compare);
    }
    it("Create sources, headers and flags log"){
        char *filename = "testForLogs/set3/farifile";
        data_t * data = parser(filename);
        int * errCode = malloc(sizeof(int));
        *errCode=0;
        char* compare="{\n"
					  "\t\"sources\":\t[\"./testForLogs/set3/a.c\", \"./testForLogs/set3/b.c\", \"./testForLogs/set3/c.c\"],\n"
					  "\t\"headers\":\t[\"./testForLogs/set3/b.h\", \"./testForLogs/set3/a.h\", \"./testForLogs/set3/c.h\", \"./testForLogs/set3/h.h\"],\n"
					  "\t\"libraries\":\t[],\n"
					  "\t\"flags\":\t[\"-std=gnu11\", \"-Wall\", \"-pedantic\", \"-O3\", \"-g\", \"-fsanitize=address\", \"-fno-omit-frame-pointer\"],\n"
					  "\t\"executable_name\":\t\"MyAwesomeExecutable\",\n"
					  "\t\"fari_error_msg\":\t\"\",\n"
					  "\t\"compilation\":\t{\n"
					  "\t\t\"commands\":\t[\"gcc -std=gnu11 -Wall -pedantic -O3 -g -fsanitize=address -fno-omit-frame-pointer -c ./testForLogs/set3/a.c -o ./testForLogs/set3/a.o\", \"gcc -std=gnu11 -Wall -pedantic -O3 -g -fsanitize=address -fno-omit-frame-pointer -c ./testForLogs/set3/b.c -o ./testForLogs/set3/b.o\", \"gcc -std=gnu11 -Wall -pedantic -O3 -g -fsanitize=address -fno-omit-frame-pointer -c ./testForLogs/set3/c.c -o ./testForLogs/set3/c.o\"],\n"
					  "\t\t\"error_msg\":\t\"\"\n"
					  "\t},\n"
					  "\t\"linking\":\t{\n"
					  "\t\t\"command\":\t\"gcc -std=gnu11 -Wall -pedantic -O3 -g -fsanitize=address -fno-omit-frame-pointer -o MyAwesomeExecutable ./testForLogs/set3/a.o ./testForLogs/set3/b.o ./testForLogs/set3/c.o\",\n"
					  "\t\t\"error_msg\":\t\"\"\n"
					  "\t},\n"
					  "\t\"fari_msg\":\t\"Compilation terminée.\"\n"
					  "}";
        asserteq_str(create_log(data,0,errCode),compare);
    }
    it("Create Source log"){
        char *filename = "testForLogs/set4/farifile";
        data_t * data = parser(filename);
        int * errCode = malloc(sizeof(int));
        *errCode=0;
        char* compare="{\n"
					  "\t\"sources\":\t[\"./testForLogs/set4/a.c\", \"./testForLogs/set4/b.c\", \"./testForLogs/set4/c.c\"],\n"
					  "\t\"headers\":\t[\"./testForLogs/set4/b.h\", \"./testForLogs/set4/a.h\", \"./testForLogs/set4/c.h\", \"./testForLogs/set4/h.h\"],\n"
					  "\t\"libraries\":\t[\"-lcriterion\"],\n"
					  "\t\"flags\":\t[\"-std=gnu11\", \"-Wall\", \"-pedantic\", \"-O3\", \"-g\", \"-fsanitize=address\", \"-fno-omit-frame-pointer\"],\n"
					  "\t\"executable_name\":\t\"MyAwesomeExecutable\",\n"
					  "\t\"fari_error_msg\":\t\"\",\n"
					  "\t\"compilation\":\t{\n"
					  "\t\t\"commands\":\t[\"gcc -std=gnu11 -Wall -pedantic -O3 -g -fsanitize=address -fno-omit-frame-pointer -c ./testForLogs/set4/a.c -o ./testForLogs/set4/a.o\", \"gcc -std=gnu11 -Wall -pedantic -O3 -g -fsanitize=address -fno-omit-frame-pointer -c ./testForLogs/set4/b.c -o ./testForLogs/set4/b.o\", \"gcc -std=gnu11 -Wall -pedantic -O3 -g -fsanitize=address -fno-omit-frame-pointer -c ./testForLogs/set4/c.c -o ./testForLogs/set4/c.o\"],\n"
					  "\t\t\"error_msg\":\t\"\"\n"
					  "\t},\n"
					  "\t\"linking\":\t{\n"
					  "\t\t\"command\":\t\"gcc -std=gnu11 -Wall -pedantic -O3 -g -fsanitize=address -fno-omit-frame-pointer -o MyAwesomeExecutable ./testForLogs/set4/a.o ./testForLogs/set4/b.o ./testForLogs/set4/c.o -lcriterion\",\n"
					  "\t\t\"error_msg\":\t\"\"\n"
					  "\t},\n"
					  "\t\"fari_msg\":\t\"Compilation terminée.\"\n"
					  "}";
        asserteq_str(create_log(data,0,errCode),compare);
    }
}
snow_main();