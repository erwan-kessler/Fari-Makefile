# Folders
BINDIR  = bin
SRCDIR  = src
OBJDIR  = obj
DATADIR = data

# Files
MAIN.C = main.c
MAIN.O = main.o

# Generals
CC     = clang
TARGET = c2k19

# Flags
CFLAGS   = -std=gnu11 -Wall -pedantic -O3
CFLAGS  += -g
CFLAGS  += -fsanitize=address -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address

# Targets
.PHONY: clean tests run_tests target

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

$(OBJDIR)/genres.o: $(SRCDIR)/genres.c $(SRCDIR)/genres.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/test_genres: $(SRCDIR)/test_genres.c $(OBJDIR)/genres.o
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) $^ -o $@ $(LDFLAGS) -lcriterion

$(OBJDIR)/movie.o: $(SRCDIR)/movie.c $(SRCDIR)/movie.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/test_movie: $(SRCDIR)/test_movie.c $(OBJDIR)/movie.o $(OBJDIR)/genres.o
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) $^ -o $@ $(LDFLAGS) -lcriterion

$(OBJDIR)/movies.o: $(SRCDIR)/movies.c $(SRCDIR)/movies.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/test_movies: $(SRCDIR)/test_movies.c $(OBJDIR)/movies.o $(OBJDIR)/movie.o $(OBJDIR)/genres.o
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) $^ -o $@ $(LDFLAGS) -lcriterion

tests: $(BINDIR)/test_genres $(BINDIR)/test_movie $(BINDIR)/test_movies

run_tests: tests
	$(BINDIR)/test_genres
	$(BINDIR)/test_movie
	$(BINDIR)/test_movies

$(OBJDIR)/parser.o: $(SRCDIR)/parser.c $(SRCDIR)/parser.h
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/$(MAIN.O): $(SRCDIR)/$(MAIN.C)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/$(TARGET): $(OBJDIR)/genres.o $(OBJDIR)/movie.o $(OBJDIR)/movies.o $(OBJDIR)/parser.o $(OBJDIR)/$(MAIN.O)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

target: $(BINDIR)/$(TARGET)
	
run: target
	$(BINDIR)/$(TARGET) $(DATADIR)/movies.csv

rundevil: target
	$(BINDIR)/$(TARGET) $(DATADIR)/devil.csv
