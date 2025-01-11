# clib
        _____   _____              ____________ ______  ______   
   _____\    \_|\    \            /            \\     \|\     \  
  /     /|     |\\    \          |\___/\  \\___/||     |\|     | 
 /     / /____/| \\    \          \|____\  \___|/|     |/____ /  
|     | |____|/   \|    | ______        |  |     |     |\     \  
|     |  _____     |    |/      \  __  /   / __  |     | |     | 
|\     \|\    \    /            | /  \/   /_/  | |     | |     | 
| \_____\|    |   /_____/\_____/||____________/|/_____/|/_____/| 
| |     /____/|  |      | |    |||           | /|    |||     | | 
 \|_____|    ||  |______|/|____|/|___________|/ |____|/|_____|/  
        |____|/                               

I want to create a C library similar to glib.

The starting point for maintaining a C project is to understand how it's built,
and for a project with a lot of files, it's a big job, and it's almost a futile
task for a novice to C without the scale, so one of the quickest ways to make
progress is to write a similar project yourself, and then it's easier to look 
back at a famous open source project.

This project uses two build tools, ninja and CMake, the former for building 
when the code is bug-free, and the latter for debugging bugs.

ninja build: compile: 'ninja', test: 'tests.exe', clear: 'ninja -t clean'

CMake build: After VSCode writes the CMakeLists.txt, press 'Ctrl + s' to 
automatically generate the build directory, and then use VS to open the .sln 
file in the build directory to build, please refer to the search engine for 
more VS use.

## Note

The code may not be compiled, and it will be modified and optimized every day 
to achieve an ideal state, and many designs are unreasonable at present, so the 
progress can not be too fast, and so on one day I feel that the design is more 
reasonable, and then it is not too late to quickly advance the progress of the
project, so that the probability of rework and modification in the end will be
smaller, and the worst case is to write a lot of space but find a new and
better design, and then the workload of modification will be very large.

## Miscellaneous

Those who have the strength can directly maintain and modify the glib to meet 
their own development needs, but for a C novice like me, developing one from 
scratch is the fastest way to progress. And then this is a project that has no
attempt to make money at all, because the previous parent project is almost 
yellow, and it has encountered so many difficulties that I have not made any
progress for several months, and I don't know how long this state will last, so
the progress of this project will probably be very slow, it depends on the mood.

## symbols

[*] ~ Modify function, [*] + add function, [*] - delete function, [*] ^ Write 
document, [/d./d./d] * New version released

where /d is the number [0-9], the first /d is the version, the second /d is the 
number of functions, and the third /d is the number of iterations of the 
original number of functions. The principle is that when the number of
functions exceeds 10, the version number should be increased by 1, except for
the first /d can be greater than 9, and the subsequent /d cannot be greater 
than 9.

## Principles

Efficiency > stipulates, that is, in the way of solving problems, when the 
benefits of efficiency are greater than the benefits of the rules, then use the 
efficiency method to solve the problem, and vice versa.

## Status

-------------------------------------------------------------------------------
    NAME   |  CODING  |  TESTING  |  Python Like  |  COMPLETED
 DynaArray                               ✔              ✔   
 LinkedList                              ✔              ✔
 HashMap                                          
-------------------------------------------------------------------------------

Explanation: What is Python Like, in Python a List instance can be inserted 
into different types, both int and class, then in the c language, we usually 
see that the data structure and algorithm are the so-called List instance can 
only be inserted into the same type, which is obviously not good enough, used
in other data structures such as Pascal language, but also can only insert the
same type, So how C implements this feature is the requirement to do.

C language implementation Python Like, using pointers**

## Use

For more information on how to use it, check out the source code in tests.c, 
which has a lot of hands-on practice.