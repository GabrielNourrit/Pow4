#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include "Game_Base.h"
#include "Int_Graph_MLV.h"
#include "type.h"

void jeu(configuration config){//fonction assez simple qui orchestre le mode 2 joueurs
  coup cp;
  init_configuration();//initialise la configuration graphiquement, crée la fenetre de jeu trace les boutons et trace la grille sans les trous/jetons. 

  do{
    if (config.j==J1)
      config.c++;
    afficher_configuration(&config);
    cp=lire_coup(&config);
    jouer_coup(&config,&cp);
  }while(est_finie(&config,&cp)<1);

  fin_du_game(&config);
}

void ia(configuration config){//quasiment la même que la fonction jeu mais pour le mode 1 joueur
  coup cp;

  //quand l'ia reprend une partie commencé il met son nom !
  config.n.nam1="I.A";
  init_configuration();
  do{
    if (config.j==J1)
      config.c++;
    afficher_configuration(&config);
    if (config.j==J1)// L'ia joue en premier
      cp=ia_coup(&config);
    else
      cp=lire_coup(&config);
    jouer_coup(&config,&cp);
  }while(est_finie(&config,&cp)<1);
  fin_du_game(&config);
}

coup ia_coup(configuration *config){
  int i,j,alea;//i,j compteurs et alea une variable qui va nous servir pour les positions aleatoires.
  int flag;//sert dans le cas des simulations de coups
  int l=0;//compteur de solution;
  int cpxy[500][2];//notre matrice contenant nos positions legales
  coup cp;
  
  for(i=0;i<N;i++){// parcours de la grille
    for(j=0;j<M;j++){
      cp.x=j;//test chaque coups
      cp.y=i;
      if (est_legal(config,&cp)==0){// pour les coups legaux
	cpxy[l][0]=j;// on stock les coups légaux dans une matrice
	cpxy[l][1]=i;
	l++;//on passe a la ligne suivante pour le prochain tour

	//simule un coup de l'ordinateur
	config->t[cp.y][cp.x]=1;
	flag=est_finie(config,&cp); // Jeu Finis ?
	config->t[cp.y][cp.x]=0; //On enleve la simulation
	if (flag!=0)
	  return cp;// si l'ordi peu finir il le fais en priorité

	//simulation d'un coup de l'humain
	config->t[cp.y][cp.x]=2;
	flag=est_finie(config,&cp);
	config->t[cp.y][cp.x]=0;
	if (flag!=0)
	  return cp;// si l'ia ne peu pas finir mais l'humain oui l'ordi joue defensif
      }
    }
  }
  
  alea=rand()%l;// si on a ni le cas ou l'ia finis ni celui ou l'humain peu finir au prochain tour alors l'ia joue random dans les coups legaux
  cp.x=cpxy[alea][0];
  cp.y=cpxy[alea][1];
  return cp;  
}
	  
void fin_du_game(configuration*config){
  int i=0,j,k,l,m;
  char courant;
  char tour_stg[2] = "";
  int curr_int,t;
  char save_int[21];//sauvegarde l'ancien score pour le mettre a la pos n+1
  char save_nom[31];//idem pour le pseudo
  FILE*HIGH_SCORE,*HN;

  afficher_configuration(config);// pour afficher le dernier coup
  MLV_draw_filled_rectangle(10,280,80,20,MLV_COLOR_BLACK);//pour effacer J1/J2 sur la fenetre;
  switch(config->j){//on regarde ce qu'il y a dans j
  case J1:
    MLV_draw_text(10,280,config->n.nam2,MLV_COLOR_RED); // si c'est J1 on affiche le nom de J2 avec un Win!! car il a gagné
    MLV_draw_text(40,280," WIN !!",MLV_COLOR_RED);
    break;
  case J2:
    MLV_draw_text(10,280,config->n.nam1,MLV_COLOR_ORANGE); // pareil dans l'autre sens
    MLV_draw_text(40,280," WIN !!",MLV_COLOR_ORANGE);
    break;
  case NUL:
    MLV_draw_text(10,280,"MATCH NUL!!",MLV_COLOR_WHITE); // affichage pour le match nul
    break;
  }
  MLV_actualise_window(); // on actualise bien pour voir quelque chose
  /*ECRIS LES HIGH SCORES*/
  HIGH_SCORE=fopen("fls/hs","r+"); // on ouvre hs qui est notre fichier pour les high scores
  HN=fopen("fls/hn","r+");// hn est notre fichier pour les noms associés aux high scores

  /* LIS LES HIGH SCORE POUR LES COMPARER AU NOUVEAU SCORE */
  while(i<10){// on veux les 10 premiers
    courant=fgetc(HIGH_SCORE);// le caractere courant est recuperer dans High score
    curr_int=(courant-'0')*10; // prend le nombre des dizaine 
    courant=fgetc(HIGH_SCORE); 
    curr_int=(courant-'0')+curr_int; // l'ajoute a l'unité
    /*********************************************************/
  
    if (curr_int>config->c){ // on a donc la comparaison entre le nombre de tour de la partie terminé et celui du fichier
      fseek(HIGH_SCORE,-2,SEEK_CUR);
      for(k=0;t!=EOF;k+=2){//on sauvegarde les highs score
	t=save_int[k]=fgetc(HIGH_SCORE);//on recupere le premier caractere de l'ancien high score courant
	t=save_int[k+1]=fgetc(HIGH_SCORE);//le caractere suivant
      }
      fseek(HIGH_SCORE,-k+2,SEEK_CUR);
    
      if (config->c<10){// si le tour est <10 on va ajouter 0 devant pour rester sur un format de 2 lettres
	sprintf(tour_stg, "0%d", config->c);
	fseek(HIGH_SCORE,-1,SEEK_CUR);
	fputs(tour_stg,HIGH_SCORE);
	for(l=0;l<k-4;l+=2){//on decale les highs scores pour n'ecraser que le high score numero 10 !
	  fputc(save_int[l],HIGH_SCORE);
	  fputc(save_int[l+1],HIGH_SCORE);
	}
      }
      else{
	sprintf(tour_stg, "%d", config->c);//pareil avec les nombres >=10
	fseek(HIGH_SCORE,-1,SEEK_CUR);
	fputs(tour_stg,HIGH_SCORE);
	for(l=0;l<k-4;l+=2){//le decalage
	  fputc(save_int[l],HIGH_SCORE);
	  fputc(save_int[l+1],HIGH_SCORE);
	}
      }
      for(j=0;j<i*3;j++){//on se positionne dans le fichier des noms
	fgetc(HN);
      }
      for (m=0,t=0;t!=EOF;m+=3){// on sauvegarde les noms a partir de celui sur lequel on va ecrire
	t=save_nom[m]=fgetc(HN);
	t=save_nom[m+1]=fgetc(HN);
	t=save_nom[m+2]=fgetc(HN);
      }

      fseek(HN,-m+2,SEEK_CUR);//reinit la pos

      if(config->j==J1){//si le joueur de la config est J1 alors c'est J2 qui a gagné
	fputc(config->n.nam2[0],HN);// on inscrit le nouveau nom de J2 dans notre fichier des noms
	fputc(config->n.nam2[1],HN);
	fputc(config->n.nam2[2],HN);

	for(l=0;l<m-6;l+=3){ // on fais le decalage dans les noms comme pour les highs scores
	  fputc(save_nom[l],HN);
	  fputc(save_nom[l+1],HN);
	  fputc(save_nom[l+2],HN);
	}
      }
      else{
	fputc(config->n.nam1[0],HN);// pareil avec J1 qui gagne
	fputc(config->n.nam1[1],HN);
	fputc(config->n.nam1[2],HN);
	for(l=0;l<m-6;l+=3){ // le decalage
	  fputc(save_nom[l],HN);
	  fputc(save_nom[l+1],HN);
	  fputc(save_nom[l+2],HN);
	}
      }
      MLV_draw_filled_rectangle(309,9,20,42,MLV_COLOR_BLACK);//on efface les 2 premiers boutons c'est un choix qu'on a pris de ne rendre ces boutons utilisable qu'en partie
      MLV_actualise_window();// on actualise l'affichage
      MLV_wait_mouse(&l,&m);// on attend un clique quelconque de l'utilisateur
      MLV_free_window();// on libere l'espace de la fenetre
      exit(0);// on quitte correctement le programme
    }
    i++;
  }
}


void creer_tablette(tablette tab){
  int i,j;
  for(i=0;i<N;i++){
    for(j=0;j<M;j++){
      tab[i][j]=0;// creer une tablette/Grille consiste à remplir une matrice de 0 comme pour chomp ici
    }
  }
}
 

int est_legal(configuration *config, coup *cp){
  // pour verifier que le coup est legal on va verifier que :
  // y appartient à [0;N] et x à [0;M]
  if(cp->y<0 || cp->x < 0 ||cp->x >= M || cp->y >= N){
    return -1;
  }
  //si on est sur la premiere ligne et qu'elle est vide çà marche!
  if(cp->y==5 && config->t[cp->y][cp->x] == 0){
    return 0;
  }else{

    //sinon si notre ligne est vide et que celle en dessous dans la vertical contient un jeton çà roule
    if(config->t[cp->y][cp->x] == 0){
      if(config->t[cp->y+1][cp->x]!=0){
	return 0;
      }
    }
  }
  //sinon pas bon
  return -1;
}

void jouer_coup(configuration *config, coup *cp){
  config->t[cp->y][cp->x]=(config->j+1); // enregistre le coup dans la grille
  if(config->j==J1)// change le joueur courant
    config->j=J2;
  else
    config->j=J1;
}

void match_nul(configuration *config){
  int i,j;
  for(i=0;i<N;i++){// pour verifier le match nul on parcours la grille
    for(j=0;j<M;j++){
      if(config->t[i][j]==0)//si il existe une case vide il n'y a pas match nul
	return;
    }
  }
  config->j=NUL;// sinon on stock çà dans j
  fin_du_game(config);// on appel la fonction pour clore le programme.
}
  

int est_finie(configuration *config, coup *cp){// la fonction est finie regarde tout autour du dernier coup jouer si il y a puissance 4
  match_nul(config);
  /*horizontal*/
  if (cp->x>=3)
    if (config->t[cp->y][cp->x-3]==config->t[cp->y][cp->x] && config->t[cp->y][cp->x-2]==config->t[cp->y][cp->x] && config->t[cp->y][cp->x-1]==config->t[cp->y][cp->x])
      return 1;
  if (cp->x>=2 && cp->x<=5)
    if (config->t[cp->y][cp->x-2]==config->t[cp->y][cp->x] && config->t[cp->y][cp->x-1]==config->t[cp->y][cp->x] && config->t[cp->y][cp->x+1]==config->t[cp->y][cp->x])
      return 1;
  if (cp->x>=1 && cp->x<=4)
    if(config->t[cp->y][cp->x-1]==config->t[cp->y][cp->x] && config->t[cp->y][cp->x+1]==config->t[cp->y][cp->x] && config->t[cp->y][cp->x+2]==config->t[cp->y][cp->x])
      return 1;
  if (cp->x>=0 && cp->x<=3)
    if (config->t[cp->y][cp->x+1]==config->t[cp->y][cp->x] && config->t[cp->y][cp->x+2]==config->t[cp->y][cp->x] && config->t[cp->y][cp->x+3]==config->t[cp->y][cp->x])
      return 1;
 
  /*vertical*/
  if (cp->y>=3)
    if (config->t[cp->y-3][cp->x]==config->t[cp->y][cp->x] && config->t[cp->y-2][cp->x]==config->t[cp->y][cp->x] && config->t[cp->y-1][cp->x]==config->t[cp->y][cp->x])
      return 1;

  if (cp->y>=2 && cp->y<=4)
    if (config->t[cp->y-2][cp->x]==config->t[cp->y][cp->x] && config->t[cp->y-1][cp->x]==config->t[cp->y][cp->x] && config->t[cp->y+1][cp->x]==config->t[cp->y][cp->x])
      return 1;

  if (cp->y>=1 && cp->y<=3)
    if (config->t[cp->y-1][cp->x]==config->t[cp->y][cp->x] && config->t[cp->y+1][cp->x]==config->t[cp->y][cp->x] && config->t[cp->y+2][cp->x]==config->t[cp->y][cp->x])
      return 1;

  if (cp->y<=2)
    if (config->t[cp->y+1][cp->x]==config->t[cp->y][cp->x] && config->t[cp->y+2][cp->x]==config->t[cp->y][cp->x] && config->t[cp->y+3][cp->x]==config->t[cp->y][cp->x])
      return 1;

  /*diagonale gauche haut droite bas*/
  if (cp->x>=3 && cp->y>=3)
    if (config->t[cp->y-3][cp->x-3]==config->t[cp->y][cp->x] && config->t[cp->y-2][cp->x-2]==config->t[cp->y][cp->x] && config->t[cp->y-1][cp->x-1]==config->t[cp->y][cp->x])
      return 1;
  if ((cp->x>=2 && cp->x<=5) && (cp->y>=2 && cp->y<=5 ))
    if (config->t[cp->y-2][cp->x-2]==config->t[cp->y][cp->x] && config->t[cp->y-1][cp->x-1]==config->t[cp->y][cp->x] && config->t[cp->y+1][cp->x+1]==config->t[cp->y][cp->x])
      return 1;
  if ((cp->x>=1 && cp->x<=4) && (cp->y>=1 && cp->y<=4 ))
    if (config->t[cp->y-1][cp->x-1]==config->t[cp->y][cp->x] && config->t[cp->y+1][cp->x+1]==config->t[cp->y][cp->x] && config->t[cp->y+2][cp->x+2]==config->t[cp->y][cp->x])
      return 1;
  if ((cp->x<=3) && (cp->y<=2 ))
    if (config->t[cp->y+1][cp->x+1]==config->t[cp->y][cp->x] && config->t[cp->y+2][cp->x+2]==config->t[cp->y][cp->x] && config->t[cp->y+3][cp->x+3]==config->t[cp->y][cp->x])
      return 1;
 
  /*diagonale gauche bas droite haut*/
  if ((cp->x<=3) && (cp->y>=3))
    if (config->t[cp->y-3][cp->x+3]==config->t[cp->y][cp->x] && config->t[cp->y-2][cp->x+2]==config->t[cp->y][cp->x] && config->t[cp->y-1][cp->x+1]==config->t[cp->y][cp->x])
      return 1;
  if ((cp->x>=1 && cp->x<=4) && (cp->y>=2 && cp->y<=5 ))
    if (config->t[cp->y-2][cp->x+2]==config->t[cp->y][cp->x] && config->t[cp->y-1][cp->x+1]==config->t[cp->y][cp->x] && config->t[cp->y+1][cp->x-1]==config->t[cp->y][cp->x])
      return 1;
  if ((cp->x>=2 && cp->x<=5) && (cp->y>=1 && cp->y<=4 ))
    if (config->t[cp->y-1][cp->x+1]==config->t[cp->y][cp->x] && config->t[cp->y+1][cp->x-1]==config->t[cp->y][cp->x] && config->t[cp->y+2][cp->x-2]==config->t[cp->y][cp->x])
      return 1;
  if ((cp->x>=3) && (cp->y<=2 ))
    if(config->t[cp->y+1][cp->x-1]==config->t[cp->y][cp->x] && config->t[cp->y+2][cp->x-2]==config->t[cp->y][cp->x] && config->t[cp->y+3][cp->x-3]==config->t[cp->y][cp->x])
      return 1;
  return 0;
}
