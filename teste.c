#include <stdio.h>
#include <unistd.h>

int
main (int argc, char *argv[])
{
    float nucleos = sysconf(_SC_NPROCESSORS_ONLN);

    printf ("Número de núcleos: %.0f\n", nucleos);

    return 0;
}
