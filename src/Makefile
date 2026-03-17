.IGNORE: clean test

CPU ?= 86 # 64

FLAG_MACHINE = /MACHINE:X$(CPU)

LIB_TARGET = libclibx$(strip $(CPU)).lib
TESTS_TARGET = tests.exe

TARGETS = $(LIB_TARGET) $(TESTS_TARGET)

all: $(TARGETS)

CC_CMD = cl
LIB_CMD = lib
LINK_CMD = link

LIB_SRC = error.c dynaarray.c linkedlist.c hashmap.c
LIB_OBJ = $(LIB_SRC:.c=.obj)

TESTS_SRC = tests.c
TESTS_OBJ = $(TESTS_SRC:.c=.obj)

$(LIB_TARGET): $(LIB_OBJ)
	$(LIB_CMD) $(FLAG_MACHINE) /OUT:$@ $^

$(TESTS_TARGET): $(TESTS_OBJ) $(LIB_TARGET)
	$(LINK_CMD) $(FLAG_MACHINE) /OUT:$@ $^ 

%.obj: %.c
	$(CC_CMD) /c /utf-8 $<

clean:
	powershell 'Remove-Item "*.obj", "*.exp", "*.lib","*.pdb","*.exe"'

test:
	./$(TESTS_TARGET)
