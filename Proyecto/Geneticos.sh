gcc Lista.c -o Lista.o -c
gcc PL.c -o PL.o -c
gcc Main.c -o Main PL.o Lista.o
./Main