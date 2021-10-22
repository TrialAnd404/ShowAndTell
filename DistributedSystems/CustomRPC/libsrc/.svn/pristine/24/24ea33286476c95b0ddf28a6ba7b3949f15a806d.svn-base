CC=gcc
DEPCC=$(CC) -MM
CFLAGS=-g

# Simply compile all .c files found in this directory
FILES=$(shell find . -name '*.c')
OBJS=$(FILES:.c=.o)
DEPS=.depend
LIB=libhamster.a
DOC=../doc


all: $(DOC) $(LIB)

# Build a library: "$@" is the make-rule's
# "Target" (here: "$(LIB)") and thus
# eventually (see above) "libhamster.a". 
$(LIB): $(OBJS)
	$(AR) -rv $@ $(OBJS)

-include $(DEPS)

$(OBJS):

$(DEPS):
	$(DEPCC) $(FILES) >$(DEPS)

# Build documentation using doxygen
$(DOC): $(FILES) hamsterdef.h
	doxygen doxyfile

clean:
	rm -f $(OBJS) $(DEPS)

distclean: clean
	rm -f $(LIB) *~
	rm -rf $(DOC)
