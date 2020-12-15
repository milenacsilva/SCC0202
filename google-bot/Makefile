# Paths for the files when not in current directory
vpath %.c . src
vpath %.h include

CC = gcc
MAIN = main
OUTPUT_OPTION = -o $@

INCLUDES = -I ./include 
CFLAGS = -std=c99 -O1
COMPILE.c = $(CC) $(DDEBUG) $(CFLAGS) $(INCLUDES) $(CPPFLAGS) $(TARGET_ARCH) -c 

LDLIBS =
LDFLAGS =
LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

DDEBUG = 
DEBUGER = valgrind
DBFLAGS = --leak-check=full --show-leak-kinds=all\
		  --track-origins=yes --verbose 
TEST_CASE = 

clean: $(MAIN)
$(MAIN): main.o utils.o parser.o site.o trie.o list.o avl.o mini_google_bot.o

run:
	./$(MAIN)

%: %.o
	$(LINK.o) $^ $(LDBIBS) $(OUTPUT_OPTION)

%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

.PHONY: debug
debug:
	$(DEBUGER) $(DBFLAGS) ./$(MAIN) $(TEST_CASE)

.PHONY: clean
clean:
	rm -f *.o

