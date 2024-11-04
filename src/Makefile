all: arraylist_test.exe
obj = ../obj
bin = ../bin

arraylist_test.obj: arraylist_test.c
	cl /c $^ 

types.obj: types.c
	cl /c $^

arraylist.obj: arraylist.c
	cl /c $^ /D WINDOWS 

arraylist_test.exe: arraylist_test.obj types.obj arraylist.obj
	link /out:$(bin)/$@ $^

test:
	../bin/arraylist_test.exe

clean:
	rm *.obj
	rm ../bin/*.exe