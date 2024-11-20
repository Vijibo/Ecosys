#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ecosys.h"

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  assert(x < SIZE_X);
  assert(y < SIZE_Y);
  (*liste_animal) = ajouter_en_tete_animal((*liste_animal), creer_animal(x, y, energie));
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
  if ((*liste) == animal) {
    Animal* new_head = (*liste)->suivant;
    free((*liste));
    (*liste) = new_head;
  } else {
    Animal* target = (*liste);
    for (; target != NULL && target->suivant != animal; target = target->suivant);
    Animal* new_next = target->suivant->suivant;
    free(target->suivant);
    target->suivant = new_next;
  }
  return ;
}

/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
  for (Animal* it = liste; it != NULL; ) {
    Animal* next = it->suivant;
    free(it);
    it = next;
  }
  return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      if (ecosys[i][j] == '*') {
        printf("\033[32m*\033[0m");  // Proie en vert
      } else if (ecosys[i][j] == 'O') {
        printf("\033[31mO\033[0m");  // Prédateur en rouge
      } else if (ecosys[i][j] == '@') {
        printf("\033[33m@\033[0m");  // Position avec les deux en jaune
      } 
      else{
        putchar(ecosys[i][j]);  // Espace vide

      }
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);

  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}


void clear_screen1(){
  system("clear");
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
    for (Animal* it = la; it; it = it->suivant) {
      if (((float)rand() / RAND_MAX) < p_ch_dir) {
        it->dir[0] = (rand() % 3) - 1;
        it->dir[1] = (rand() % 3) - 1;
      }
      it->x += it->dir[0];
      it->y += it->dir[1];

      if (it->x < 0) {
        it->x = SIZE_X + it->x;
      }
      if (it->x >= SIZE_X) {
        it->x = it->x - SIZE_X;
      }

      if (it->y < 0) {
        it->y = SIZE_Y + it->y;
      }
      if (it->y >= SIZE_Y) {
        it->y = it->y - SIZE_Y;
      }
    }

}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
  for (Animal* it = (*liste_animal); it; it = it->suivant) {
    if (((float)rand() / RAND_MAX) < p_reproduce) {
      it->energie /= 2;
      ajouter_animal(it->x, it->y, it->energie, liste_animal);
    }
  }

}


/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    for (Animal* it = (*liste_proie); it;) {
      int* herbe = &monde[it->x][it->y];
      if ((*herbe) > 0) {
        it->energie += (*herbe);
        (*herbe) = temps_repousse_herbe;
      }

      it->energie--;
      if (it->energie == 0) {
        Animal* tmp = it;
        it = it->suivant;
        enlever_animal(liste_proie, tmp);
      }
      else {
        it = it->suivant;
      }
    }
    reproduce(liste_proie, p_reproduce_proie);
    bouger_animaux(*liste_proie);
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
  for (Animal* it = l; it; it = it->suivant) {
    if ((it->x == x) && (it->y == y)) {
      return it;
    }
  }
  return NULL;
}

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
   /*A Completer*/
    for (Animal* it = (*liste_predateur); it;) {
      Animal* proie = (*liste_proie);
      while ((proie = animal_en_XY(proie, it->x, it->y)) != NULL) {
        it->energie += proie->energie;
        Animal* tmp = proie;
        proie = proie->suivant;
        enlever_animal(liste_proie, tmp);
      }

      it->energie--;
      if (it->energie == 0) {
        Animal* tmp = it;
        it = it->suivant;
        enlever_animal(liste_predateur, tmp);
      }
      else {
        it = it->suivant;
      }
    }
    reproduce(liste_predateur, p_reproduce_predateur);
    bouger_animaux(*liste_predateur);
}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
  for (int x = 0; x < SIZE_X; x++) {
    for (int y = 0; y < SIZE_Y; y++) {
      monde[x][y]++;
    }
  }
}

