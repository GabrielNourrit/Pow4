#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>
#include "Int_Graph_MLV.h"
#include "Game_Base.h"
#include "type.h"

void init_configuration(){// affiche notre grille sans trou ouvre notre fenetre de jeu et dessine nos boutons
  MLV_create_window( "Pow4", "", 340,300 );
  MLV_draw_filled_rectangle( 2, 2, 280, 240, MLV_COLOR_BLUE);
  
  /*DESSIN DES BOUTONS*/
  /*SAVE*/
  MLV_draw_rectangle( 310, 10, 15, 15, MLV_COLOR_WHITE);
  MLV_draw_text(312,10,"S",MLV_COLOR_WHITE);
  /*RECOMMENCER*/
  MLV_draw_rectangle( 310, 35, 15, 15, MLV_COLOR_WHITE);
  MLV_draw_text(312,35,"R",MLV_COLOR_WHITE);
  /*QUITTER*/
  MLV_draw_rectangle( 310, 60, 15, 15, MLV_COLOR_WHITE);
  MLV_draw_text(312,60,"Q",MLV_COLOR_WHITE);
  /*****/
}

void afficher_configuration(configuration *config){// affiche les trou si 0 les jaune si 1 les rouges si 2
  int i,j;
  for(i=0;i<N;i++){
    for(j=0;j<M;j++){// pour toute la configuration
      switch(config->t[i][j]){
      case 0:
	MLV_draw_filled_circle( j*40+20, i*40+20, 15, MLV_COLOR_BLACK );
	break;
      case 1:
	MLV_draw_filled_circle( j*40+20, i*40+20, 15, MLV_COLOR_ORANGE );
	break;
      case 2:
	MLV_draw_filled_circle( j*40+20, i*40+20, 15, MLV_COLOR_RED );
	break;
      }
    }
    MLV_draw_filled_rectangle(10,280,80,20,MLV_COLOR_BLACK);// on efface en bas a gauche et on ecris le nom du joueur a qui c'est le tour de jouer
    if (config->j==J1)
      MLV_draw_text(10,280,config->n.nam1,MLV_COLOR_ORANGE);
    if (config->j==J2)
      MLV_draw_text(10,280,config->n.nam2,MLV_COLOR_RED);
  }
  MLV_actualise_window();
}

void button(int*x,int*y,configuration*config){
  int i,j,a,b;
  FILE*l_s;
  MLV_Image *pseudo,*lia,*hum;

  /*GESTION DES BOUTONS*/
  /*SAVE*/
  if((*x>=310 && *x<=325) && (*y>=10 && *y<=25)){// si t'appuies sur save
    l_s=fopen("fls/l_s","w+");// on ouvre notre load_save fichier
    fputc(config->j+'0',l_s);// on ecrit d'abord le joueur courant
    for(i=0;i<N;i++){// puis on ecris toute les cases de la grille
      for(j=0;j<M;j++){
	fputc(config->t[i][j]+'0',l_s);
      }
    }
    fputc (config->c,l_s);// et enfin le nombre de tour
    fclose(l_s);
      
    l_s=fopen("fls/n_s","w+");// dans le fichier nom on met le nom des deux joueur
    fputc (config->n.nam1[0],l_s);
    fputc (config->n.nam1[1],l_s);
    fputc (config->n.nam1[2],l_s);
    fputc (config->n.nam2[0],l_s);
    fputc (config->n.nam2[1],l_s);
    fputc (config->n.nam2[2],l_s);
    fclose(l_s);
  }
  /*RECOMMENCER*/
  if((*x>=310 && *x<=325) && (*y>=35 && *y<=50)){// donc si on clique sur le bouton pour recommencer
    MLV_free_window();// ca ferme la fenetre
    config->j=J1;// on reinitialise notre configuration : joueur,tablette,tour
    creer_tablette(config->t);
    config->c=0;

    // Menu de selection pour savoir si vous voulez chargez une partie contre l'ia ou contre un 2eme joueur.
    i=0;// ce sera notre variable de choix

    MLV_create_window( "trash", "trsh", 340, 290 );
    do{
      MLV_free_window();
      MLV_create_window( "chargement", "chm", 340, 290 );
      pseudo=MLV_load_image("img/title.png");
      lia=MLV_load_image("img/IA.png");//109*30 la taille
      hum=MLV_load_image("img/hum.png");//134*31
      MLV_draw_image(pseudo,33,58);
      MLV_draw_image(lia,109,129);
      MLV_draw_image(hum,99,184);
      MLV_actualise_window();

      MLV_wait_mouse(&a,&b);//on recupere le clique de la souris
      if(a>=109 && a<=218 && b>=129 && b<=159)
	i=1;
      if(a>=99 && a<=233 && b>=184 && b<=215)
	i=2;
    }while(i!=1 && i!=2);
    //on libere la place en memoire
    MLV_free_image(lia);
    MLV_free_image(pseudo);
    MLV_free_image(hum);
    MLV_free_window();
    //puis on agis celon le choix de l'user
    if (i==1)
      ia(*config);
    else
      jeu(*config);
    //*************************FIN DU MENU
  }
  
  /*QUITTER*/
  if((*x>=310 && *x<=325) && (*y>=60 && *y<=75)){// rien de plus simple
    MLV_free_window();
    exit(0);
  }
  /******/
}

coup lire_coup(configuration *config){
  int x,y;
  coup cp;
  do{// lire coup attend un clique et s'il est pas legal elle recommence
    MLV_wait_mouse(&x,&y);
    button(&x,&y,config);
    cp.x=(x)/(40);
    cp.y=(y)/(40);
  }
  while(est_legal(config,&cp)<0);

  return cp;// quand le coup est (enfin) legal elle le renvoie
}

int main_menu(){// notre menu graphique principal 
  int x,y,a;
  MLV_Image *New,*credits,*exit,*hs,*load,*titre;// toutes nos images
  MLV_create_window( "Menu", "m", 410,600 );//notre fenetre
  New=MLV_load_image("img/NewG.png");
  credits=MLV_load_image("img/credits.png");
  exit=MLV_load_image("img/Exit.png");
  hs=MLV_load_image("img/HS.png");
  load=MLV_load_image("img/Load.png");
  titre=MLV_load_image("img/titre.png");// les initialisation
  
  MLV_draw_image(titre, 67, 84); //270*71 les tailles des images servent pour delimiter les zones de clique
  MLV_draw_image(New, 75, 230);  //255*52
  MLV_draw_image(load, 83, 293); //242*47
  MLV_draw_image(hs, 63, 350); //279*51
  MLV_draw_image(exit, 128, 411); //133*46
  MLV_draw_image(credits, 113, 540); //157*32
 
  MLV_actualise_window();
  do{
    MLV_wait_mouse(&x,&y);// on attend que l'utilisateur clique sur un bouton

    if (x>=75 && x<=330 && y>=230 && y<=282)
      a=1;
    if (x>=83 && x<=325 && y>=293 && y<=340)
      a=2;
    if (x>=63 && x<=342 && y>=350 && y<=401)
      a=4;
    if (x>=128 && x<=261 && y>=411 && y<=457)
      a=3;
  }while(a!=1 && a!=2 && a!=3 && a!=4);
  //on libere tout l'espace plus necessaire
  MLV_free_image(titre);
  MLV_free_image(New);
  MLV_free_image(load);
  MLV_free_image(hs);
  MLV_free_image(exit);
  MLV_free_image(credits);
  MLV_free_window();
  // on renvoie le choix de l'utilisateur
  return a;
}

int new_menu(){// sous menu graphique du menu principal tres tres proche du meni principal dans le principe
  int a,x,y;
  MLV_Image *j1,*credits,*j2,*titre;
  MLV_create_window( "Menu", "m", 410,600 );
  credits=MLV_load_image("img/credits.png");
  titre=MLV_load_image("img/titre.png");
  j1=MLV_load_image("img/1j.png");
  j2=MLV_load_image("img/2j.png");
  MLV_draw_image(titre, 67, 84);
  MLV_draw_image(j1, 116, 328);//138*40
  MLV_draw_image(j2, 107, 261);//170*47
  MLV_draw_image(credits, 113, 540);
  MLV_actualise_window();
  do{
    MLV_wait_mouse(&x,&y);

    if (x>=107 && x<=277 && y>=261 && y<=308)
      a=1;
    if (x>=116 && x<=254 && y>=328 && y<=368)
      a=2;
  }while(a!=1 && a!=2);
  MLV_free_image(titre);
  MLV_free_image(j1);
  MLV_free_image(j2);
  MLV_free_image(credits);
  return a;
}
