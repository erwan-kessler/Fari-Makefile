#define SNOW_ENABLED

#include <sys/fcntl.h>
#include "../snow/snow.h"
#include "../src/Parser.h"
#define DEBUG 0
int devNull;

describe(Parser_Test) {
	before_each() {

	    if (!DEBUG){

			devNull = open("/dev/null", O_WRONLY);
			if (devNull == -1) {
				fprintf(stderr, "Error in open('/dev/null',0)\n");
				exit(EXIT_FAILURE);
			}
			close(STDERR_FILENO);
			int dup2Result = dup2(devNull, STDERR_FILENO);
			if (dup2Result == -1) {
				fprintf(stderr, "Error in dup2(devNull, STDERR_FILENO)\n");
				exit(EXIT_FAILURE);
			}
	    }

    }
	after_each() {
		close(devNull);
	}
	subdesc(Parser_comments_tests){
		it("Parsing of simple comment") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data = parser("testFileForParser/comments_simple");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 1, "The headers contains elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
		it("Parsing of multiple comments") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data = parser("testFileForParser/comments_multiple");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 1, "The headers contains elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
	}
	subdesc(Parser_blank_file_tests) {
		it("Parsing of blank file") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
                data_t *data = parser("testFileForParser/blank_nothing");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 250, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}

		it("Parsing of blank file with tabs") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data = parser("testFileForParser/blank_tabs");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 1, "The headers contains elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
		it("Parsing of blank file with spaces") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data = parser("testFileForParser/blank_spaces");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 1, "The headers contains elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}


		it("Parsing of blank file with window returns") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data = parser("testFileForParser/blank_return_windows");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 1, "The headers contains elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
		it("Parsing of blank file with linux returns") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data = parser("testFileForParser/blank_return_linux");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 1, "The headers contains elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
	}
	subdesc(Executable_name_tests) {
		it("Parsing of executable name with more than one name") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
                data_t *data = parser("testFileForParser/executable_name_long");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 249, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
		it("Parsing of executable name redefined") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t* data=parser("testFileForParser/executable_name_multiple");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 254, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}

	}
	subdesc(Header_files_test) {
		it("Parsing of one header file") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data=parser("testFileForParser/header_simple");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 2, "The headers contains no or too much elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
		it("Parsing of multiple header file") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data=parser("testFileForParser/header_multiple");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 5, "The headers contains no or too much elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
		it("Parsing of redefined header file") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data=parser("testFileForParser/header_redefinition");
				asserteq(data->executable_name, "test", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 1, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 4, "The headers contains no or too much elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
                destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
	}


	subdesc(Linked_headers) {
		it("Parsing of one linked header file") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data=parser("testFileForParser/linked_headers");
				asserteq(data->executable_name, "f", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 4, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 2, "The headers contains no or too much elements");
				asserteq_int(data->flags->number_of_elements, 1, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 1, "The libraries contains elements");
				asserteq_int(data->linked_headers->number_of_elements, 3, "The libraries contains elements");
				destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}

	}


	subdesc(Complete_files_test) {
		it("Parsing of a complex file") {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				data_t *data=parser("testFileForParser/complex_file");
				asserteq(data->executable_name, "f", "The name of the executable is incorrect");
				asserteq_int(data->source_files->number_of_elements, 4, "The sources contains elements");
				asserteq_int(data->header_files->number_of_elements, 2, "The headers contains no or too much elements");
				asserteq_int(data->flags->number_of_elements, 2, "The flags contains elements");
				asserteq_int(data->libraries->number_of_elements, 2, "The libraries contains elements");
				destroy_data(data);
				exit(0);
			}
			wait(&status);
			if (WIFEXITED(status)) {
				asserteq_int(WEXITSTATUS(status), 0, "The return code was incorrect");
			} else {
				fail("The process did not exit correctly");
			}
		}
	}
	subdesc(Java_Parsing_test) {
        it("Parsing of a java simple java compile") {
            pid_t pid = fork();
            int status;
            if (pid == 0) {
                data_t *data = parser("testFileForParser/java_simple");

                asserteq_int(data->source_files->number_of_elements, 6, "The amount of .class generated is incorrect");
                asserteq_int(data->flags->number_of_elements, 1, "There shouldnt be any flags");
                asserteq_int(data->header_files->number_of_elements, 1, "There shouldnt be any headers");
                asserteq_int(data->libraries->number_of_elements, 1, "There shouldnt be any library");
                destroy_data(data);
                exit(0);
            }
            wait(&status);
            if (WIFEXITED(status)) {
                asserteq_int(WEXITSTATUS(status), 0, "The return code is incorrect");

            } else {
                fail("The process didnt exit correctly");
            }
        }

        it("Parsing a java file with a lib line") {
            pid_t pid = fork();
            int status;
            if (pid == 0) {
                data_t *data = parser("testFileForParser/java_lib");
                asserteq_int(data->libraries->number_of_elements, 3, "The amount of libraries is incorrect");
                asserteq_int(data->source_files->number_of_elements, 4, "The amount of java files is incorrect");
                asserteq_int(data->flags->number_of_elements, 1, "There shouldnt be any flags");
                asserteq_int(data->header_files->number_of_elements, 1, "There shouldnt be any headers");
                destroy_data(data);
                exit(0);
            }
            wait(&status);
            if (WIFEXITED(status)) {
                asserteq_int(WEXITSTATUS(status), 0, "The return code is incorrect");
            } else {
                fail("The process didnt exit correctly");
            }
        }

        it("Parsing a java file with a flag line") {
            pid_t pid = fork();
            int status;
            if (pid == 0) {
                data_t *data = parser("testFileForParser/java_flag");
                asserteq_int(data->libraries->number_of_elements, 1, "There shouldnt be any library");
                asserteq_int(data->source_files->number_of_elements, 4, "The amount of java files is incorrect");
                asserteq_int(data->flags->number_of_elements, 3, "The ammount of flags is incorrect");
                asserteq_int(data->header_files->number_of_elements, 1, "There shouldnt be any headers");
                destroy_data(data);
                exit(0);
            }
            wait(&status);
            if (WIFEXITED(status)) {
                asserteq_int(WEXITSTATUS(status), 0, "The return code is incorrect");
            } else {
                fail("The process didnt exit correctly");
            }

        }

        it("Parsing a java file with headers") {
            pid_t pid = fork();
            int status;
            if (pid == 0) {
                data_t *data = parser("testFileForParser/java_header");
                destroy_data(data);
                exit(0);
            }
            wait(&status);
            if (WIFEXITED(status)) {
                asserteq_int(WEXITSTATUS(status), 203, "The return code was incorrect");
            } else {
                fail("The process did not exit correctly");
            }
        }

        it("Parsing a complete java file") {
            pid_t pid = fork();
            int status;
            if (pid == 0) {
                data_t *data = parser("testFileForParser/java_complete_command_test");
                asserteq_int(data->source_files->number_of_elements, 4, "The amount of java files is incorrect");
                asserteq_int(data->libraries->number_of_elements, 3, "TThe amount of libraries  is incorrect");
                asserteq_int(data->flags->number_of_elements, 3, "The amount of flags is incorrect");
                asserteq_int(data->header_files->number_of_elements, 1, "There shouldnt be any headers");
                destroy_data(data);
                exit(0);
            }
            wait(&status);
            if (WIFEXITED(status)) {
                asserteq_int(WEXITSTATUS(status), 0, "The return code is incorrect");
            } else {
                fail("The process didnt exit correctly");
            }


        }
    }
}

snow_main();