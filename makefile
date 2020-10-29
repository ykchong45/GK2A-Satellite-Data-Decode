CC := gcc

MODULES := vtb rs common
LIB_DIR := $(addprefix lib/, $(MODULES))
LIB_SRC := $(foreach libdir, $(LIB_DIR), $(wildcard $(libdir)/*.c))
INCLUDE := $(foreach libdir, $(LIB_DIR), -I$(libdir))

all:
	$(CC) -o main.exe frame_sync.c $(LIB_SRC) $(INCLUDE)