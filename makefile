.PHONY: all test clean

# Edit this line to your libfec directory.
# i.e. for "E:\libfec" change the following line to "e/libfec"
# DO NOT add preceding slash "/"
LIB_DIR_WIN := e/libfec


# DO NOT Edit beyond this line.
CC := gcc

SRC := libfec.a
LIB_DIR := $(addprefix /cygdrive/, $(LIB_DIR_WIN))
LIB_SRC := $(foreach libsrc, $(SRC), $(LIB_DIR)/$(libsrc))

INC_DIR := ./includes

all:
	$(CC) -o decode.exe decode.c $(LIB_SRC) -I$(INC_DIR)

test:
	@echo $(LIB_SRC)

clean:
	rm -f *.exe