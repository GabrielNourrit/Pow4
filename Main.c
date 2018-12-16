#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include "Game_Base.h"
#include "Int_Graph_MLV.h"
#include "type.h"
  
int main(){
  int choix,i,j,x,y;
  char high_score[1];
  configuration config;
  FILE*l_s,*hn;
  name noms;
  int flag=0;
  char test[3];
  char test1[3];
  char* nom_temp;
  MLV_Image *pseudo,*lia,*hum;
  
  srand(time(NULL));//initialise time pour notre fonction aleatoire
  choix=main_menu();// le menu principal du jeu
  
  switch(choix){// celon ce que l'utilisateur choisit dans le menu principal
  case 1:// NOUVELLE PARTIE
    if(new_menu()==1){// sous menu pour savoir si l'user veut jouer en mode 2j ou 1j
      //--------------------------------------------
      //-----------------MODE 1J--------------------
      //--------------------------------------------
      
      /***** LA CONSOLE POUR ENTRER PSEUDO********/
      /*** pseudo1 **/
      do{
	MLV_free_window();// ferme la fenetre du sous menu precedent et la fenetre courante pour en cree une nouvelle si l'utilisateur entre un nom different de 3 caracteres
	MLV_create_window( "name", "nm", 340, 300 );
	pseudo=MLV_load_image("img/pseudo.png");
	MLV_draw_image(pseudo,80,100);// image pour faire jolie
	MLV_actualise_window();
	MLV_wait_input_box(140,210,40,30,MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK,"",&nom_temp);// notre boite de saisie qui stock dans nom_temp
	MLV_actualise_window();
	if (strlen(nom_temp)==3){// si le nom est bon on positionne un flag et on recupere le nom temporaire dans notre name
	  flag=1;
	  noms.nam1=nom_temp;
	}
      }while(flag!=1);
      flag=0;// on remet le flag a 0 pour le pseudo du J2
      MLV_free_image(pseudo);// on libere l'espace pris par la premiere image
      
      /*** pseudo2 ***/
      // semblable a la partie pseudo1 mais pour le pseudo2
      do{
	MLV_free_window();
	MLV_create_window( "name2", "nm2", 340, 300 );
	pseudo=MLV_load_image("img/pseudo2.png");
	MLV_draw_image(pseudo,80,100);
	MLV_actualise_window();
	MLV_wait_input_box(140,210,40,30,MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK,"",&nom_temp);
	MLV_actualise_window();
	if (strlen(nom_temp)==3){
	  flag=1;
	  noms.nam2=nom_temp;
	}
      }while(flag!=1);
      /***FIN DE LA CONSOLE DES PSEUDOS**/

      config.n=noms;// on recupere les noms dans notre configuration
      config.j=J1;// le premier joueur
      config.c=0;// on initialise les tours
      creer_tablette(config.t);// on creer une grille
      MLV_free_image(pseudo);// on libere l'espace prise par notre image2
      MLV_free_window();// on libere l'espace pris par notre fenetre
      jeu(config);// on lance le mode 2 joueur.
    }
    else{
      //_______________________________________________________
      // _____________________IA ZONE__________________________
      //_______________________________________________________

      /*** console pour le pseudo du J2 ***/
      do{
	MLV_free_window();
	MLV_create_window( "name2", "nm2", 340, 300 );
	pseudo=MLV_load_image("img/pseudo2.png");
	MLV_draw_image(pseudo,80,100);
        MLV_actualise_window();
	MLV_wait_input_box(140,210,40,30,MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_COLOR_BLACK,"",&nom_temp);
        MLV_actualise_window(); 
	if (strlen(nom_temp)==3){
	  flag=1;
	  noms.nam2=nom_temp;
	}
      }while(flag!=1);
      /*** end console ***/
		  
      noms.nam1="I.A"; // le nom de l'ia
      config.n=noms;// initialise les noms
      config.j=J1;// J1 commence a jouer
      config.c=0;// initialise les tours
      creer_tablette(config.t);// creer une grille
      MLV_free_image(pseudo);
      MLV_free_window();
      ia(config);// lance le mode 1 joueur
    }
    break;

    //-----------------------------------------------------
  case 2: // l'utilisateur a choisit de charger une partie
    //----------------------------------------------------
    l_s=fopen("fls/l_s","r");// on ouvre notre fichier de load_save
    config.j=fgetc(l_s)-'0';// on recupere le joueur courant
    for(i=0;i<N;i++){// on remplit la grille par les etats enregistrés
      for(j=0;j<M;j++){
	config.t[i][j]=fgetc(l_s)-'0';
      }
    }
    config.c=fgetc(l_s)-1;// on recupere le tour courant
    fclose(l_s);// on ferme notre fichier

    l_s=fopen("fls/n_s","r");// on recupere les noms des joueurs qu'on a mis dans name_save
    test[0]=fgetc(l_s);//nom 1
    test[1]=fgetc(l_s);
    test[2]=fgetc(l_s);
    test1[0]=fgetc(l_s);//nom 2
    test1[1]=fgetc(l_s);
    test1[2]=fgetc(l_s);
    fclose(l_s);// on ferme le deuxieme fichier
    config.n.nam1=test;// on met nos noms dans la config
    config.n.nam2=test1;

    // Menu de selection pour savoir si vous voulez chargez une partie contre l'ia ou contre un 2eme joueur.
    i=0;// ce sera notre variable de choix

    MLV_create_window( "trash", "trsh", 10, 10 );//fenetre poubelle pour la fermer dans la boucle
    do{
      MLV_free_window();
      MLV_create_window( "chargement", "chm", 340, 290 );
      pseudo=MLV_load_image("img/title.png");// image pour faire jole
      lia=MLV_load_image("img/IA.png");//109*30 la taille
      hum=MLV_load_image("img/hum.png");//134*31
      MLV_draw_image(pseudo,33,58);
      MLV_draw_image(lia,109,129);
      MLV_draw_image(hum,99,184);
      MLV_actualise_window();// on affiche toute nos image

      MLV_wait_mouse(&x,&y);//on recupere le clique de la souris
      if(x>=109 && x<=218 && y>=129 && y<=159)// on regarde sur quel bouton l'utilisateur clique
	i=1;
      if(x>=99 && x<=233 && y>=184 && y<=215)
	i=2;
    }while(i!=1 && i!=2);// on attend qu'il clique sur un bouton
    //on libere la place en memoire
    MLV_free_image(lia);
    MLV_free_image(pseudo);
    MLV_free_image(hum);
    MLV_free_window();
    //puis on agis celon le choix de l'user
    if (i==1)
      ia(config);
    else
      jeu(config);
    //*************************FIN DU MENU*****************
    break;
    
  case 3: // le cas ou l'utilisateur quitte simplement le programme
    exit(0);
    break;
    //-------------------------------------------------------------------------
  case 4: //------------------ la partie HIGHS SCORES--------------------------
    //-------------------------------------------------------------------------
    l_s=fopen("fls/hs","r");//on ouvre notre fichier de scores
    hn=fopen("fls/hn","r");// notre fichier de noms associes aux scores
    MLV_create_window("high score","hs",410,600);
    for(i=0;i<10;i++){
      //affichage des scores
      high_score[0]=fgetc(l_s);
      high_score[1]='\0';
      MLV_draw_text(160,i*50+50,high_score,MLV_COLOR_WHITE);
      high_score[0]=fgetc(l_s);
      high_score[1]='\0';
      MLV_draw_text(170,i*50+50,high_score,MLV_COLOR_WHITE);

      //affichage des noms
      high_score[0]=fgetc(hn);
      high_score[1]='\0';
      MLV_draw_text(200,i*50+50,high_score,MLV_COLOR_WHITE);
      high_score[0]=fgetc(hn);
      high_score[1]='\0';
      MLV_draw_text(215,i*50+50,high_score,MLV_COLOR_WHITE);
      high_score[0]=fgetc(hn);
      high_score[1]='\0';
      MLV_draw_text(230,i*50+50,high_score,MLV_COLOR_WHITE);
	
      MLV_actualise_window();
    }
    fclose(l_s);
    fclose(hn);//on ferme tout
    MLV_wait_seconds(500);// on patiente 500 secondes pour fermer ou on attend que l'utilisateur appui sur la croix.
    break;
  }
}


