.IGNORE: clean

LIB_TARGET = clib.dll
TESTS_TARGET = tests.exe

CC = cl
LINK = link

LIB_SRC = error.c dynaarray.c linkedlist.c hashmap.c
LIB_OBJ = $(LIB_SRC:.c=.obj)

TESTS_SRC = tests.c
TESTS_OBJ = tests.obj

all: $(LIB_TARGET) $(TESTS_TARGET)

$(LIB_TARGET): $(LIB_OBJ)
	$(LINK) /DLL $^ /OUT:$@

$(TESTS_TARGET): clib.lib $(TESTS_OBJ)
	$(LINK) $^ /OUT:$@

%.obj: %.c
	$(CC) /c /utf-8 $<

clean:
	rm -rf $(LIB_OBJ) $(TESTS_OBJ) $(LIB_TARGET) $(TESTS_TARGET) *.lib *.exp 

test:
	./tests.exe