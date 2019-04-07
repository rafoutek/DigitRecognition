#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "GfxLib.h"
#include "BmpLib.h"
#include "NNdefinitions.h"
#include "fonctions.h"
#include "fonctionsTraitementImage.h"
#include "graphic.h"

int main (int argc, char **argv)
{
	int h=28,l=28; //images 28x28
	int **r, **v, **b, **g; //matrices pour lecture des images (en couleur puis gris puis seuillée)
	int nb_entrees = h*l; // chaque pixel est une entree
	int nb_sorties = 10; // chaque sortie correspond à un chiffre


	int choix_menu = 0;
	while(choix_menu != 3)
	{
		choix_menu = menu();
		
		switch(choix_menu)
		{
			case 1: //apprentissage a partir des modeles
				lanceFenetreGraphique(argc, argv);
				break;
				
			case 2: //test apprentissage

				r = alloueMatriceInt(h,l);
				v = alloueMatriceInt(h,l);
				b = alloueMatriceInt(h,l);
				g = alloueMatriceInt(h,l);
				DonneesImageRGB *img;
				MODELE modele = init_modele(nb_entrees,nb_sorties);
				RESEAU reseau = init_reseau(modele);

				printf("TEST APPRENTISSAGE\n");
				recupere_biais_et_poids_enregistres(&reseau);
				//demande à l'utilisateur quelle image il veut test
				img = demande_et_lit_image_test();
				remplit_modele_depuis_image(img,&r,&v,&b,&g, &(modele));
				libereDonneesImageRGB(&img);
				recopie_EntreesModele_dansEntreesReseau(modele, &reseau);
				propagation_avant(&reseau);
				cree_et_affiche_classement_ressemblance(reseau);

				//liberation pointeurs
				libereMatriceInt(h,l,r);
				libereMatriceInt(h,l,v);
				libereMatriceInt(h,l,b);
				libereMatriceInt(h,l,g);
				free(modele.entrees);
				free(modele.sorties_attendues);
				
				for(int i = 0 ; i< reseau.nb_couches ; i++)
				{
					for(int j = 0; j< reseau.couches[i].nb_neurones ; j++)
					{
						free(reseau.couches[i].neurones[j].entrees);
						free(reseau.couches[i].neurones[j].entrees_suivantes_liees);
					}
					free(reseau.couches[i].neurones);
				}
				free(reseau.couches);
				break;
			
			case 3:
				printf("Sortie du programme\n");
				break;
		}
	}
	
	
	return 0;
}
