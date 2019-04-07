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

#include "graphic.h"
#include "graphicdefinitions.h"
#include "IHM.h"
#include "fonctionsTraitementImage.h"

#include "NNdefinitions.h"
 #include "fonctions.h"

static char *titre = "Neural Network learning graph";

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
static int LargeurFenetre = 800;
static int HauteurFenetre = 600;

//sert à lier GfxLib.c et graphic.c pour modifier LargeurFenetre et HauteurFenetre  (Redimmensionnement)
extern int varLargeurFenetre;
extern int varHauteurFenetre;


int lanceFenetreGraphique(int argc, char **argv)
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

	static DonneesImageRGB *retour, *home, *croix, *logo; 

	static zone zQuit, zRetour, zHome, zTitre;

	static zone zGraph;

	static clock_t start_t, end_t, total_t;

	static int h,l; //images 28x28
	static int **r, **v, **b, **g; //matrices pour lecture des images (en couleur puis gris puis seuillée)

	static int nb_entrees; // chaque pixel est une entree
	static int nb_sorties; // chaque sortie correspond à un chiffre
	static DonneesImageRGB *img;
	static MODELE modele;
	static RESEAU reseau;

	static int nb_boucles;
	static char *chemin_image;

	static int *erreurs ; 

	switch (evenement)
	{
		case Initialisation:
			demandeTemporisation(0);
			chargeImagesIHM(&retour,&home,&croix,&logo);
			initZones(&zQuit,&zHome,&zRetour,&zTitre, retour,home,croix, LargeurFenetre,HauteurFenetre, titre);
			initGraph(&zGraph);

			h=28,l=28;
			r = alloueMatriceInt(h,l);
			v = alloueMatriceInt(h,l);
			b = alloueMatriceInt(h,l);
			g = alloueMatriceInt(h,l);
			nb_entrees = h*l;
			nb_sorties = 10;
			modele = init_modele(nb_entrees,nb_sorties);
			reseau = init_reseau(modele);
			printf("APPRENTISSAGE A PARTIR DES MODELES\n");
			
			//initialisation des poids aleatoires des perceptrons
			srand(time(NULL));
			init_poids_alea_Reseau(&reseau);
			init_biais_Reseau(&reseau); 

			nb_boucles = 0;
			
			chemin_image = (char *)malloc(sizeof(char)*50);

			// tableau qui sera rempli case par case (à chaque boucle)
			// et qui sera affiché à chaque rafraichissent
			erreurs = (int *)malloc(sizeof(int) * MAX_X);  

			break;

		case Temporisation:

			// boucle effectuée à chaque temporisation
			apprentissage(&r,&v,&b,&g,img,&modele,&reseau,chemin_image,&(erreurs[nb_boucles]), &nb_boucles);

			if(erreurs[nb_boucles] == 0) // apprentissage fini
			{
				end_t = clock();
				total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

				printf("TOUS LES MODELES SONT SATISFAITS AVEC CES POIDS!\n");
				enregistrement_biais_et_poids_reseau(reseau);
				
				printf("Temps total pris par CPU pour trouver les bons parametres: %f\n", (double)total_t  );
				
				// fin du programme
				free(chemin_image);
				free(erreurs);
				termineBoucleEvenements();
			}

			nb_boucles++;
			rafraichisFenetre();
			break;

		case Affichage:
			switch (numPage)
			{
				case 1:
					// On part d'un fond d'ecran blanc
					effaceFenetre (255, 255, 255);

					monIHM(zQuit,zHome,zRetour,retour,home,croix,logo,numPage);
					afficheTitre(zTitre);

					// display of learning graph
					couleurCourante(255,255,255);
					rectangle(zGraph.xmin-30,zGraph.ymin-20,zGraph.xmax+20,zGraph.ymax+20);
					afficheAxesGraph(zGraph);

					// une fois le graph affiché on peut commencer l'apprentissage	
					numPage = 2;					
					start_t = clock(); // temps depuis lancement du programme			
		
					break;
				
				case 2:
					// On part d'un fond d'ecran blanc
					effaceFenetre (255, 255, 255);

					monIHM(zQuit,zHome,zRetour,retour,home,croix,logo,numPage);
					afficheTitre(zTitre);

					// display of learning graph
					couleurCourante(255,255,255);
					rectangle(zGraph.xmin-30,zGraph.ymin-20,zGraph.xmax+20,zGraph.ymax+20);
					afficheAxesGraph(zGraph);
					afficheTousLesPoints(erreurs, nb_boucles, zGraph);
					break;
			
				default:
					break;
			}

			//rafraichisFenetre();
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
