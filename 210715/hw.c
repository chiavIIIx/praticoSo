#include <stdio.h>
int main(int argc, char*argv[]) { //dichiarazione del main con argomenti
        printf("hello world:"); //stampo hello world
        for(argv++, argv--; argc > 0; argv++, argc--) //scorro gli argomenti  
                printf(" %s",*argv); //stampo ogni argomento
        printf("\n"); //
        return 0;
}