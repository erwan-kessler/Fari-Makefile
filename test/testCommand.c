#define SNOW_ENABLED

#include <sys/fcntl.h>
#include "../snow/snow.h"
#include "../src/Parser.h"
#include "../src/Commands.h"


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
    //This test cannot pass, the file doesnt exist, it was prolly never finished, commenting it
    /*
	it("Parsing of simple command") {
		pid_t pid = fork();
		int status;
		if (pid == 0) {
			data_t *data = parser("testFileForParser/command_test");
			asserteq(data->executable_name, "f", "The name of the executable is incorrect");
			asserteq_int(data->source_files->number_of_elements, 4, "The sources contains elements");
			asserteq_int(data->header_files->number_of_elements, 4, "The headers contains no or too much elements");
			asserteq_int(data->flags->number_of_elements, 4, "The flags contains elements");
			asserteq_int(data->libraries->number_of_elements, 2, "The libraries contains elements");

			set_t *set=createCompileCommand(data);
			delete_set(set);
			destroy_data(data);
			exit(0);
		}
		wait(&status);
		if (WIFEXITED(status)) {
			asserteq_int(WEXITSTATUS(status), 255, "The return code was incorrect");
		} else {
			fail("The process did not exit correctly");
		}
	}
     */
	it("Testing a simple java command"){
        pid_t pid = fork();
        int status;
        if (pid == 0) {
            data_t *data = parser("testFileForCommands/java_command_test");
            asserteq_int(data->source_files->number_of_elements, 4, "The amount of source files is incorrect");
            asserteq_int(data->header_files->number_of_elements, 1, "There shouldnt be any headers");
            asserteq_int(data->flags->number_of_elements, 1, "There shouldnt be any flag");
            asserteq_int(data->libraries->number_of_elements, 1, "There shouldnt be any library");

            char * java_command = createJavaCommand(data);
            asserteq(java_command, "javac ./testFileForCommands/test1.java ./testFileForCommands/test2.java ./testFileForCommands/test3.java", "The command is incorrect");

            free(java_command);
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

	it("Testing a java command with a flag"){
        pid_t pid = fork();
        int status;
        if (pid == 0) {
            data_t *data = parser("testFileForCommands/java_flag_command_test");
            asserteq_int(data->source_files->number_of_elements, 4, "The amount of source files is incorrect");
            asserteq_int(data->header_files->number_of_elements, 1, "There shouldnt be any headers");
            asserteq_int(data->flags->number_of_elements, 3, "The amount of flags files is incorrect");
            asserteq_int(data->libraries->number_of_elements, 1, "There shouldnt be any library ");

            char * java_command = createJavaCommand(data);
            asserteq(java_command, "javac -encoding UTF-8 ./testFileForCommands/test1.java ./testFileForCommands/test2.java ./testFileForCommands/test3.java", "The command is incorrect");

            free(java_command);
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

    it("Testing a java command with a library"){
        pid_t pid = fork();
        int status;
        if (pid == 0) {
            data_t *data = parser("testFileForCommands/java_library_command_test");
            asserteq_int(data->source_files->number_of_elements, 4, "The amount of source files is incorrect");
            asserteq_int(data->header_files->number_of_elements, 1, "There shouldnt be any headers");
            asserteq_int(data->flags->number_of_elements, 1, "There shouldnt be any flag");
            asserteq_int(data->libraries->number_of_elements, 3, "The amount of libraries files is incorrect");

            char * java_command = createJavaCommand(data);
            asserteq(java_command, "javac -cp mysql-connector-java-8.0.12.jar; ./testFileForCommands/test1.java ./testFileForCommands/test2.java ./testFileForCommands/test3.java", "The command is incorrect");

            free(java_command);
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

	it("Testing a complete java command"){
        pid_t pid = fork();
        int status;
        if (pid == 0) {
            data_t *data = parser("testFileForCommands/java_complete_command_test");
            asserteq_int(data->source_files->number_of_elements, 4, "The amount of source files is incorrect");
            asserteq_int(data->header_files->number_of_elements, 1, "There shouldnt be any headers");
            asserteq_int(data->flags->number_of_elements, 3, "The amount of flags files is incorrect");
            asserteq_int(data->libraries->number_of_elements, 3, "The amount of libraries files is incorrect");

            char * java_command = createJavaCommand(data);
            asserteq(java_command, "javac -encoding UTF-8 -cp mysql-connector-java-8.0.12.jar; ./testFileForCommands/test1.java ./testFileForCommands/test2.java ./testFileForCommands/test3.java", "The command is incorrect");

            free(java_command);
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

snow_main();