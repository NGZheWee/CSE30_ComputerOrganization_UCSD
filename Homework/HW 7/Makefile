# version 1.0
# if you type 'make' without arguments, this is the default
PROG = encrypter 
all: $(PROG)

# other source files and the associated object files (this can be blank) SRC is unused here
SRC	= main.c cipher.s
OBJ	= main.o cipher.o

# special libraries
LIB	=

# select the compiler and flags yopu can over-ride on command line e.g. make DEBUG= 
CC	= gcc
DEBUG	= -g
CSTD	= -std=c11
WARN	= -Wall -Wextra
CFLAGS	= -I. -D_POSIX_C_SOURCE $(DEBUG) $(WARN) $(CSTD)

# select the assembler, specify how to generate a .o from a .s and flags
AS	= gcc
ASLIST	= -Wa,-adhln
ASFLAGS = -I.

%.o : %.s
	$(AS) -c $(ASFLAGS) $(DEBUG) $< -o $@

encrypter: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $@

# remove binaries
.PHONY: clean 
clean:
	rm -f $(OBJ) $(PROG)
