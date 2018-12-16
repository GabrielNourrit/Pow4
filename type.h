#ifndef type
#define type

#define N 6 /*lignes*/
#define M 7 /*colonnes*/

typedef int tablette[N][M];// notre grille de puissance 4

enum joueur{J1,J2,NUL};
typedef enum joueur joueur;

typedef struct{
  char*nam1;
  char*nam2;
}name;// pour les noms associers aux 2 joueurs

typedef struct{
  tablette t;
  joueur j;
  int c;// le nombre de tour
  name n;
}configuration;

typedef struct{
  int x;
  int y;
}coup;

#endif
