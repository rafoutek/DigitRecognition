#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <math.h>		// Pour pouvoir utiliser les librairies mathematiques
#include <complex.h>
#include <unistd.h>
#include <time.h>

#include "GfxLib.h"		// Seul cet include est necessaire pour faire du graphique
#include "ESLib.h"	
#include "OutilsLib.h"	
#include "BmpLib.h"


#include "definitions.h"
#include "IHM.h"
#include "fonctionsTraitementImage.h"

static char *titre = "Neural Network learning graph";

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
static int LargeurFenetre = 800;
static int HauteurFenetre = 600;

//sert à lier GfxLib.c et main.c pour modifier LargeurFenetre et HauteurFenetre  (Redimmensionnement)
extern int varLargeurFenetre;
extern int varHauteurFenetre;

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
   des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);


int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);

	prepareFenetreGraphique(titre, LargeurFenetre, HauteurFenetre);

	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
	   qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();

	return 0;
}


/* La fonction de gestion des evenements, appelee automatiquement par le systeme
   des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement)
{
	static int numPage = 1;

	static DonneesImageRGB *retour, *home, *croix; 

	static zone zQuit, zRetour, zHome, zTitre;

	static zone zGraph;

	switch (evenement)
	{
		case Initialisation:
			demandeTemporisation(20);
			retour = lisBMPRGB("retour.bmp");
			home = lisBMPRGB("home.bmp");
			croix = lisBMPRGB("croix.bmp");
			if(retour == NULL || home == NULL || croix == NULL)
			{
				perror("problem reading images");
				libereDonneesImageRGB(&retour); libereDonneesImageRGB(&home); libereDonneesImageRGB(&croix);
				exit(EXIT_FAILURE);
			}
			initZones(&zQuit,&zHome,&zRetour,&zTitre, retour,home,croix, LargeurFenetre,HauteurFenetre, titre);
			initGraph(&zGraph);
			break;

		case Temporisation:

			// A chaque mise a jour il faut redessiner la fenetre :
			rafraichisFenetre();
			break;

		case Affichage:
			// On part d'un fond d'ecran blanc
			effaceFenetre (255, 255, 255);

			monIHM(zQuit,zHome,zRetour,numPage, LargeurFenetre);
			afficheTitre(zTitre);

			// display of learning graph
			couleurCourante(255,255,255);
			rectangle(zGraph.xmin-30,zGraph.ymin-20,zGraph.xmax+20,zGraph.ymax+20);
			afficheAxesGraph(zGraph);
			affichePointFonction(zGraph);
			break;

		case Clavier:

			switch (caractereClavier())
			{
				case 'q':
				case 'Q':
					exit(0);
					break;
			}
			break;

		case ClavierSpecial:
			//printf("ASCII %d\n", toucheClavier());
			break;

		case BoutonSouris:
			if (etatBoutonSouris() == GaucheAppuye) 
			{
				if (zQuit.xmin<abscisseSouris() && abscisseSouris()<zQuit.xmax && zQuit.ymin<ordonneeSouris() && ordonneeSouris()<zQuit.ymax)
				{
					exit(0);
				}


				//bouton HOME
				if ((zHome.xmin<abscisseSouris() && abscisseSouris()<zHome.xmax && zHome.ymin<ordonneeSouris() && ordonneeSouris()<zHome.ymax) && (numPage>1))
				{	
					numPage=1;  //reviens à la premiere page (accueil)
					rafraichisFenetre();	
				}

				//bouton RETOUR 				
				if ((zRetour.xmin<abscisseSouris() && abscisseSouris()<zRetour.xmax && zRetour.ymin<ordonneeSouris() && ordonneeSouris()<zRetour.ymax) && (numPage>1))
				{	
					if(numPage > 1) numPage=1;
					//if(numPage == 2 || numPage == 3 || numPage == 4) numPage=1;

					//else if(numPage == 21 || numPage == 211 || numPage == 212 || numPage == 213) numPage=3; 

					rafraichisFenetre();	
				}

			}
			break;

		case Souris: // Si la souris est deplacee
			break;

		case Inactivite: // Quand aucun message n'est disponible
			break;

		case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
			// Donc le systeme nous en informe
			LargeurFenetre = varLargeurFenetre;
			HauteurFenetre = varHauteurFenetre;
			redimensionneZones(&zQuit,&zHome,&zRetour,&zTitre,retour,home,croix, LargeurFenetre,HauteurFenetre);
			redimmensionneGraph(&zGraph);
			rafraichisFenetre();
			
			break;
	}
}
