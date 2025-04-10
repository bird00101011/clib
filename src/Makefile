.IGNORE: clean

LIB_TARGET = clib.lib
TESTS_TARGET = tests.exe

TARGETS = $(LIB_TARGET) $(TESTS_TARGET)

CC_CMD = cl
LIB_CMD = lib
LINK_CMD = link

LIB_SRC = error.c dynaarray.c linkedlist.c hashmap.c
LIB_OBJ = $(LIB_SRC:.c=.obj)

TESTS_SRC = tests.c
TESTS_OBJ = $(TESTS_SRC:.c=.obj)

all: $(TARGETS)

$(LIB_TARGET): $(LIB_OBJ)
	$(LIB_CMD) /OUT:$@ $^

$(TESTS_TARGET): $(TESTS_OBJ) $(LIB_TARGET)
	$(LINK_CMD) /OUT:$@ $^

%.obj: %.c
	$(CC_CMD) /c /utf-8 $<

clean:
	rm -f *.obj *.lib *.exp *.exe

test:
	./$(TESTS_TARGET)
