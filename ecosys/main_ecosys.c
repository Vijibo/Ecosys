#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"

#define NB_PROIES     64
#define NB_PREDATEURS 12
#define T_WAIT        93333

#define ENERGIE_BASE  10
#define N_ITER        300

float p_ch_dir              = 1/4.f;
float p_reproduce_proie     = 1/28.f;
float p_reproduce_predateur = 1/42.f;
int   temps_repousse_herbe  = -14;

int main(void)
{
  // srand(time(NULL));
  srand(0);

  Animal* liste_proie       = NULL;
  Animal* liste_predateur   = NULL;
  int monde[SIZE_X][SIZE_Y] = {0}; // "Brace Zero Initialization"

  for (int i = 0; i < NB_PROIES; i++) {
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, ENERGIE_BASE, &liste_proie);
  }
  for (int i = 0; i < NB_PREDATEURS; i++) {
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, ENERGIE_BASE, &liste_predateur);
  }

  FILE* f = fopen("Evol_Pop.csv", "w");
  fputs("JOUR, N_PROIES, N_PRED\n", f);
  for (int i = 0; i < N_ITER; i++)
  {
    rafraichir_proies(&liste_proie, monde);
    rafraichir_predateurs(&liste_predateur, &liste_proie);
    fprintf(f, "%i,%i,%i\n", i, compte_animal_it(liste_proie),  compte_animal_it(liste_predateur));

    clear_screen1();
    afficher_ecosys(liste_proie, liste_predateur);
    usleep(T_WAIT);
  }
  fclose(f);

  liberer_liste_animaux(liste_proie);
  liberer_liste_animaux(liste_predateur);

  return 0;
}

