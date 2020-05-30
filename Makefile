# Folders
#BINDIR  = bin
SRCDIR  = src
OBJDIR  = obj
TESTDIR1 = test/testForFari/set1
TESTDIR2 = test/testForFari/set2
TESTDIR3 = test/testForFari/set3
TESTDIR4 = test/testForFari/set4
TESTDIR5 = test/testForFari/set5
TESTDIR6 = test/testForFari/set6
TESTDIR7 = test/testForFari/set7
TESTDIR8 = test/testForFari/set8
TESTDIR9 = test/testForFari/set9
HASHLIB= highwayhash
CJSONLIB= cJson
# Files
MAIN.C = main.c
MAIN.O = main.o

# Generals
CC     = gcc
TARGET = fari

# Flags
CFLAGS   = -std=gnu11 -Wall -pedantic -O3
CFLAGS  += -g
CFLAGS  += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address

# Targets
.PHONY: default
.DEFAULT_GOAL : default

default: clean target

clean:
	rm -rf $(OBJDIR)
	rm -f fari

$(OBJDIR)/checker.o: $(SRCDIR)/Checker.c $(SRCDIR)/Checker.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/commands.o: $(SRCDIR)/Commands.c $(SRCDIR)/Commands.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/launcher.o: $(SRCDIR)/launcher.c $(SRCDIR)/launcher.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/parser.o: $(SRCDIR)/Parser.c $(SRCDIR)/Parser.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/set.o: $(SRCDIR)/Set.c $(SRCDIR)/Set.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/highwayhash.o: $(HASHLIB)/highway.c $(HASHLIB)/highway.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/cJSON.o: $(CJSONLIB)/cJSON.c $(CJSONLIB)/cJSON.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/json_logs.o: $(SRCDIR)/json_logs.c $(SRCDIR)/json_logs.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/$(MAIN.O): $(SRCDIR)/$(MAIN.C)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJDIR)/checker.o $(OBJDIR)/commands.o $(OBJDIR)/launcher.o $(OBJDIR)/parser.o $(OBJDIR)/set.o $(OBJDIR)/highwayhash.o $(OBJDIR)/cJSON.o $(OBJDIR)/json_logs.o $(OBJDIR)/$(MAIN.O)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

target: $(TARGET)

test1: target
	cp $(TARGET) $(TESTDIR1);cd $(TESTDIR1);./$(TARGET);./MyAwesomeExecutable;cat logs.json

test2: target
	cp $(TARGET) $(TESTDIR2);cd $(TESTDIR2);./$(TARGET);./MyAwesomeExecutable;cat logs.json

test3: target
	cp $(TARGET) $(TESTDIR3);cd $(TESTDIR3);./$(TARGET);./MyAwesomeExecutable movies.csv>/dev/null;cat logs.json

test3.1: target
	cp $(TARGET) $(TESTDIR3);cd $(TESTDIR3);./$(TARGET) farifile2;./TestGenre;cat logs.json

test3.2: target
	cp $(TARGET) $(TESTDIR3);cd $(TESTDIR3);./$(TARGET) farifile3;./TestMovie;cat logs.json

test3.3: target
	cp $(TARGET) $(TESTDIR3);cd $(TESTDIR3);./$(TARGET) farifile4;./TestMovies;cat logs.json


#This test was removed due to a precision in the subject
#test3.4: target
#	cp $(TARGET) $(TESTDIR2);$(TESTDIR2)/$(TARGET) $(TESTDIR2)/farifile;./MyAwesomeExecutable;cat logs.json

test3.5: target
	cp $(TARGET) $(TESTDIR3);cd $(TESTDIR3);./$(TARGET) farifile5;./TestMovies;cat logs.json

test4: target
	cp $(TARGET) $(TESTDIR4);cd $(TESTDIR4);./$(TARGET) ;

test5: target
	cp $(TARGET) $(TESTDIR5);cd $(TESTDIR5);./$(TARGET) -k;

test6: target
	cp $(TARGET) $(TESTDIR6);cd $(TESTDIR6);./$(TARGET) -k || (echo "Test successfull"; exit 0 );

test7: target
	cp $(TARGET) $(TESTDIR7);cd $(TESTDIR7);./$(TARGET);

test8: target
	cp $(TARGET) $(TESTDIR8);cd $(TESTDIR8);./$(TARGET);./MyAwesomeExecutable;cat logs.json

test9: target
	cp $(TARGET) $(TESTDIR9);cd $(TESTDIR9);./$(TARGET);./MyAwesomeExecutable;cat logs.json

run: target
	$(TARGET) farifile
