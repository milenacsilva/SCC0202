# Paths for the files when not in current directory
vpath %.c . src
vpath %.h include

CC = gcc
MAIN = main
OUTPUT_OPTION = -o $@

# Compilation related
INCLUDES = -I ./include 
CFLAGS =
COMPILE.c = $(CC) $(DDEBUG) $(CFLAGS) $(INCLUDES) $(CPPFLAGS) $(TARGET_ARCH) -c 

# Linking related
LDLIBS =
LDFLAGS =
LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

# Debugging option
DDEBUG = 
DEBUGER = valgrind
DBFLAGS = --leak-check=full --show-leak-kinds=all\
		  --track-origins=yes --verbose 
TEST_CASE = 

# For ziping stuff
FILES =
ARCHIVE_NAME = entrega.zip


######## Your code here   #############
clean: $(MAIN)

$(MAIN): main.o utils.o site.o site_list.o commands.o
#######################################

############ Pattern Rules ############# 
%: %.o
	$(LINK.o) $^ $(LDBIBS) $(OUTPUT_OPTION)

%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

########################################

############# Phony targets ############ 
.PHONY: debug
debug:
	$(DEBUGER) $(DBFLAGS) ./$(MAIN) $(TEST_CASE)
# If you want to add a test case: make debug TEST_CASE=test.txt

.PHONY: clean
clean:
	rm -f *.o
# Will remove all objects

.PHONY: zip
zip: 
	zip -r $(ARCHIVE_NAME) $(FILES)
# For ziping multiple files:  make zip FILES=Makefile\ src\ include
#######################################
