#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <math.h>
#include <complex.h>
#include "GfxLib.h"		// Seul cet include est necessaire pour faire du graphique
#include "ESLib.h"	
#include "OutilsLib.h"	
#include "BmpLib.h"

#include "graphicdefinitions.h"
#include "IHM.h"
#include "fonctionsTraitementImage.h"

void chargeImagesIHM(DonneesImageRGB **retour,DonneesImageRGB **home,DonneesImageRGB **croix,DonneesImageRGB **logo)
{
	*retour = lisBMPRGB("./images/retour.bmp");
	*home = lisBMPRGB("./images/home.bmp");
	*croix = lisBMPRGB("./images/croix.bmp");
	*logo = lisBMPRGB("./images/logo.bmp");

	if( *retour == NULL || *home == NULL || *croix == NULL || *logo == NULL)
	{
		perror("problem reading IHM images");
		libereImagesIHM(retour,home,croix,logo);
		termineBoucleEvenements();
	}
}

void libereImagesIHM(DonneesImageRGB **retour,DonneesImageRGB **home,DonneesImageRGB **croix,DonneesImageRGB **logo)
{
	libereDonneesImageRGB(retour); 
	libereDonneesImageRGB(home); 
	libereDonneesImageRGB(croix); 
	libereDonneesImageRGB(logo);
}

void monIHM(zone zQuit, zone zHome, zone zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, DonneesImageRGB *logo, int numpage)
{
	int ecart_bord = 10;

	effaceFenetre(0,102,204);

	afficheImage(zQuit,croix);
	
	if(numpage == 1)
		ecrisImage(ecart_bord, hauteurFenetre() - ecart_bord - logo->hauteurImage, logo->largeurImage, logo->hauteurImage, logo->donneesRGB); //affiche logo
	
	if(numpage > 2) // 2 est la page d'acceuil
	{
		afficheImage(zHome,home);
		afficheImage(zRetour,retour);
	}
}

void afficheTitre(zone zTitre)
{
/* 	couleurCourante(0,100,255);
	rectangle(zTitre.xmin, zTitre.ymin, zTitre.xmax, zTitre.ymax);
	 */
	epaisseurDeTrait(3);
	couleurCourante(250,250,0); // Choix de la couleur 
	afficheChaine(zTitre.texte, 30, zTitre.xmin + (zTitre.longueur/7)  , zTitre.ymin+10);
}



void afficheImage(zone zITest, DonneesImageRGB *image)
{
	ecrisImage(zITest.xmin, zITest.ymin, zITest.xmax-zITest.xmin, zITest.ymax-zITest.ymin, image->donneesRGB);
}



//Fonctions de la page principal de l'interface

void initZones(zone *zQuit, zone *zHome, zone *zRetour, zone *zTitre, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, char *titre)
{
			// initialisation de la zone zQuit			
			zQuit->xmax = largeurFenetre() - 4 ; 
			zQuit->xmin = zQuit->xmax - croix->largeurImage;
			zQuit->ymax = hauteurFenetre() - 2 ;
			zQuit->ymin = zQuit->ymax - croix->hauteurImage ; 

			//initialisation du bouton accueil
			zHome->xmax = zQuit->xmin-4;  //on part du bouton quitter pour placer le bouton home juste à côté
			zHome->xmin = zHome->xmax - home->largeurImage ;
			zHome->ymax = hauteurFenetre()-2;
			zHome->ymin = zHome->ymax - home->hauteurImage ;

			//initialisation du bouton retour
			zRetour->xmax = zHome->xmin-4;//on part du bouton home pour placer le bouton retour juste à côté
			zRetour->xmin = zRetour->xmax - retour->largeurImage ;
			zRetour->ymax = hauteurFenetre()-2 ;
			zRetour->ymin = zRetour->ymax - retour->hauteurImage;

			//initialisation de la zone Titre		
			zTitre->longueur = 650; //choix de la longueur de la zone
			zTitre->hauteur = 60; //pareil mais pour la hauteur
			zTitre->espaceHaut = 10;
			zTitre->espaceBas = 100;
			zTitre->xmin = (largeurFenetre() - zTitre->longueur)/2; //cette formule sert à créer une zone centrée horizontalement
			zTitre->xmax = zTitre->xmin + zTitre->longueur;
			zTitre->ymax = hauteurFenetre() - zTitre->espaceHaut;
			zTitre->ymin = zTitre->ymax - zTitre->hauteur ;
			strcpy(zTitre->texte, titre);

}

void redimensionneZones(zone *zQuit, zone *zHome, zone *zRetour, zone *zTitre, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix)
{
			// initialisation de la zone zQuit			
			zQuit->xmax = largeurFenetre() - 4 ; 
			zQuit->xmin = zQuit->xmax - croix->largeurImage;
			zQuit->ymax = hauteurFenetre() - 2 ;
			zQuit->ymin = zQuit->ymax - croix->hauteurImage ; 

			//initialisation du bouton accueil
			zHome->xmax = zQuit->xmin-4;  //on part du bouton quitter pour placer le bouton home juste à côté
			zHome->xmin = zHome->xmax - home->largeurImage ;
			zHome->ymax = hauteurFenetre() - 2;
			zHome->ymin = zHome->ymax - home->hauteurImage ;

			//initialisation du bouton retour
			zRetour->xmax = zHome->xmin-4;//on part du bouton home pour placer le bouton retour juste à côté
			zRetour->xmin = zRetour->xmax - retour->largeurImage ;
			zRetour->ymax = hauteurFenetre() - 2 ;
			zRetour->ymin = zRetour->ymax - retour->hauteurImage;

			//initialisation de la zone Titre		
			zTitre->longueur = 650; //choix de la longueur de la zone
			zTitre->hauteur = 60; //pareil mais pour la hauteur
			zTitre->espaceHaut = 10;
			zTitre->espaceBas = 100;
			zTitre->xmin = (largeurFenetre() - zTitre->longueur)/2; //cette formule sert à créer une zone centrée horizontalement
			zTitre->xmax = zTitre->xmin + zTitre->longueur;
			zTitre->ymax = hauteurFenetre() - zTitre->espaceHaut;
			zTitre->ymin = zTitre->ymax - zTitre->hauteur ;

}


void initGraph(zone *zGraph)
{
	//initialisation de la zone Titre	
	zGraph->espaceGauche = ECART_AXES+10;
	zGraph->espaceDroite = ECART_AXES;
	zGraph->longueur = largeurFenetre() - zGraph->espaceGauche - zGraph->espaceDroite; //choix de la longueur de la zone

	zGraph->espaceHaut = 100;
	zGraph->espaceBas = ECART_AXES;	
	zGraph->hauteur = hauteurFenetre() - zGraph->espaceHaut - zGraph->espaceBas ; //pareil mais pour la hauteur

	zGraph->xmin = zGraph->espaceGauche; 
	zGraph->xmax = zGraph->xmin + zGraph->longueur;
	zGraph->ymin = zGraph->espaceBas ;
	zGraph->ymax = zGraph->ymin + zGraph->hauteur;
	
}

void redimmensionneGraph(zone *zGraph)
{
	zGraph->longueur = largeurFenetre() - zGraph->espaceGauche - zGraph->espaceDroite; //choix de la longueur de la zone
	zGraph->hauteur = hauteurFenetre() - zGraph->espaceHaut - zGraph->espaceBas ; //pareil mais pour la hauteur
	zGraph->xmax = zGraph->xmin + zGraph->longueur;
	zGraph->ymax = zGraph->ymin + zGraph->hauteur;
}

void afficheAxesGraph(zone zGraph )
{
	// affchage des axes
	couleurCourante(0,0,0);
	ligne(zGraph.xmin, zGraph.ymin, zGraph.xmax, zGraph.ymin); // axe abscisse
	ligne(zGraph.xmin, zGraph.ymin, zGraph.xmin, zGraph.ymax); // axe ordonnee

  	int intervalle_x = (zGraph.longueur * INTERVALLE_X) / MAX_X; // intervalles de l'ihm reels
	int intervalle_y = (zGraph.hauteur * INTERVALLE_Y) / MAX_Y;  

	int intervalle_x_display = 0; //intervalles mis à l'échelle
	int intervalle_y_display = 0;

	char num [1000]; // 9999 max !
	epaisseurDeTrait(2);
	for(int x = 0; x <= zGraph.longueur; x += intervalle_x)
	{
		sprintf(num, "%d",intervalle_x_display);
		afficheChaine(num, 15 , zGraph.xmin + x, zGraph.ymin - 16);
		intervalle_x_display += INTERVALLE_X;
	}
	for(int y = 0; y <= zGraph.hauteur; y += intervalle_y)
	{
		sprintf(num, "%d",intervalle_y_display);
		afficheChaine(num, 15 , zGraph.xmin - 25, zGraph.ymin + y);
		intervalle_y_display += INTERVALLE_Y;
	}
}

void affichePointFonction(zone zGraph)
{
	float y=0;
	float intervalle = zGraph.longueur / MAX_X; // intervalle pour chaque point
	epaisseurDeTrait(2);
	couleurCourante(0,0,0);
	for(float x = 0; x<MAX_X; x+=intervalle)
	{
		y = x;
		if(y < zGraph.ymax)
			point(round(x) + zGraph.xmin, y + zGraph.ymin);
	}
}

void affichePointDansGraph(int nb_boucle, int erreurs_modeles, zone zGraph )
{
	//printf("nb boucle = %d, erreurs = %d\n", nb_boucle,erreurs_modeles);
	float x =  (zGraph.longueur * nb_boucle) / MAX_X; // placer une iteration sur le graph
	float y =  (zGraph.hauteur * erreurs_modeles) / MAX_Y; // placer un point sur l'ordonnee
	//printf("hauteur graph = %d, y point = %d\n", zGraph.hauteur ,(int)round(y));

	if(erreurs_modeles < zGraph.ymax)
		point((int)round(x) + zGraph.xmin, (int)round(y) + zGraph.ymin);
}

void afficheTousLesPoints(int *erreurs, int nb_boucles, zone zGraph)
{
	epaisseurDeTrait(1);
	couleurCourante(0,0,0);

	for(int num_boucle = 0; num_boucle < nb_boucles; num_boucle++)
	{
		affichePointDansGraph(num_boucle, erreurs[num_boucle], zGraph);
	}
}