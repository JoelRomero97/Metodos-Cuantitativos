gcc Lista.c -o Lista.o -c
gcc PL.c -o PL.o -c -lm
gcc Main.c -o Main PL.o Lista.o -lm
rm Lista.o
rm PL.o
./Main