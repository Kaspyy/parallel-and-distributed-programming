#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "pomiar_czasu.h"
int zmienna_globalna = 0;

void main()
{

  int pid, wynik, i;
  inicjuj_czas();
  for (i = 0; i < 1000; i++)
  {
    zmienna_globalna = 0;

    pid = fork();

    if (pid == 0)
    {

      zmienna_globalna++;

      wynik = execv("./program", NULL);
      if (wynik == -1)
        printf("Proces potomny nie wykonal programu\n");

      exit(0);
    }
    else
    {

      wait(NULL);
    }
  }
  printf("Proces nadrzedny: zmienna globalna = %d\n", zmienna_globalna);
  drukuj_czas();
}
