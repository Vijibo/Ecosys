#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "ecosys.h"

float p_ch_dir=0.01;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;

void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie)
{
  FILE* f = fopen(nom_fichier, "w");
  fputs("<proies>\n", f);
  for (Animal* it = liste_proie; it; it = it->suivant) {
    fprintf(f, "x=%i y=%i dir=[%i %i] e=%f\n", it->x, it->y, it->dir[0], it->dir[1], it->energie);
  }
  fputs("</proies>\n", f);

  fputs("<predateurs>\n", f);
  for (Animal* it = liste_predateur; it; it = it->suivant) {
    fprintf(f, "x=%i y=%i dir=[%i %i] e=%f\n", it->x, it->y, it->dir[0], it->dir[1], it->energie);
  }
  fputs("</predateurs>\n", f);
  fclose(f);
}

void lire_ecosys_aux(FILE* f, const char* balise_debut, const char* balise_fin, Animal** liste) {
  char buf[256] = {0};
  fgets(buf, 256, f);
  if (strcmp(buf, balise_debut) != 0) {
    return;
  }
  else {
    fgets(buf, 256, f);
    if (strcmp(buf, balise_fin) != 0) {
      (*liste) = creer_animal(0, 0, 0);
      sscanf(buf, "x=%i y=%i dir=[%i %i] e=%f",
        &(*liste)->x,
        &(*liste)->y,
        &(*liste)->dir[0],
        &(*liste)->dir[1],
        &(*liste)->energie
      );
    }

    Animal* it = *liste;
    while (1) {
      fgets(buf, 256, f);
      if (strcmp(buf, balise_fin) == 0) break;
      it->suivant = creer_animal(0, 0, 0);
      it = it->suivant;
      sscanf(buf, "x=%i y=%i dir=[%i %i] e=%f",
        &it->x,
        &it->y,
        &it->dir[0],
        &it->dir[1],
        &it->energie
      );
    }
  }
}

void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie) {
  assert(*liste_predateur == NULL);
  assert(*liste_proie == NULL);

  FILE* f = fopen(nom_fichier, "r");

  lire_ecosys_aux(f, "<proies>\n", "</proies>\n", liste_proie);
  lire_ecosys_aux(f, "<predateurs>\n", "</predateurs>\n", liste_predateur);

  fclose(f);
}

void egalite_liste_animal(Animal* liste_a, Animal* liste_b, int n) {
  Animal* it_a = liste_a;
  Animal* it_b = liste_b;
  for (int i = 0; i < n; i++) {
    assert((it_a->energie == it_b->energie) &&
          (it_a->dir[0] == it_b->dir[0]) &&
          (it_a->dir[1] == it_b->dir[1]) &&
          (it_a->x == it_b->x) &&
          (it_a->y == it_b->y));
    it_a = it_a->suivant;
    it_b = it_b->suivant;
  }
}

int main(void) {
  int i;
  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;
  int energie=10;

  srand(time(NULL));

  for (int i = 0; i < 20; i++) {
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie, &liste_proie);
  }

  for (int i = 0; i < 20; i++) {
    ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, energie, &liste_predateur);
  }

  ecrire_ecosys("save.ecosys", liste_predateur, liste_proie);

  Animal *liste_proie_bis = NULL;
  Animal *liste_predateur_bis = NULL;

  lire_ecosys("save.ecosys", &liste_predateur_bis, &liste_proie_bis);

  egalite_liste_animal(liste_proie, liste_proie_bis, 20);
  egalite_liste_animal(liste_predateur, liste_predateur_bis, 20);

  afficher_ecosys(liste_proie, liste_predateur);
  Animal* dernier = liste_proie
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant
      ->suivant;
  enlever_animal(&liste_proie, dernier);
  enlever_animal(&liste_proie, liste_proie);
  enlever_animal(&liste_proie, liste_proie->suivant);
  afficher_ecosys(liste_proie, liste_predateur);

  liberer_liste_animaux(liste_predateur);
  liberer_liste_animaux(liste_proie);
  return 0;
}
