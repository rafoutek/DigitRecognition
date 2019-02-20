#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "BmpLib.h"
#include "definitions.h"
#include "fonctions.h"
#include "fonctionsTraitementImage.h"

int main (void)
{
	int h=28,l=28; //images 28x28
	int nb_entrees = h*l;
	int nb_sorties = 9;
	DonneesImageRGB *img;
	MODELE_COMPLET modeleComplet = init_modeleComplet(1,nb_entrees,nb_sorties);
	RESEAU reseau = init_reseau(modeleComplet);


	int choix_menu = 0;
	while(choix_menu != 3)
	{
		choix_menu = menu();
		
		switch(choix_menu)
		{
			case 1: //apprentissage a partir des modeles
				printf("APPRENTISSAGE A PARTIR DES MODELES\n");
				
				img = lit_imageModele(0,0);
				remplit_modeleComplet_image(img, &modeleComplet);
				determine_sortieModeleAttendue(0,modeleComplet.modeles[0]);
				affiche_modele(modeleComplet.modeles[0]);
				printf("modele rempli !\n");
				getchar();
				
				//recopie entree modeleComplet dans entrees du reseau
				recopie_EntreesModele_dansEntreesReseau(modeleComplet.modeles[0], &reseau);
				affiche_entrees_reseau(reseau);
				getchar();

				//initialisation des poids aleatoires des perceptrons
				srand(time(NULL));
				init_poids_alea_Reseau(&reseau);
				//initialise les biais de chaque perceptron à 0
				init_biais_Reseau(&reseau);  
				
				short int erreurs_modeles;
				bool erreurs_reseau_negligeables;
				
				int nb_boucles = 0;
				int nb_modeles_a_apprendre ;
				
				do{
					erreurs_modeles = 0; 
					
					nb_boucles ++;
					nb_modeles_a_apprendre  = 0;
					
					//on peut selectionner les modeles a prendre en compte pour l'apprentissage ici
					for( int i = 0; i < modeleComplet.nb_modeles; i++)
					{
						printf("\n\nMODELE %d\n",i);
						nb_modeles_a_apprendre++;
						affiche_modele(modeleComplet.modeles[i]);
						propagation_avant_selon_modele(&reseau, modeleComplet.modeles[i]);
						erreurs_reseau_negligeables = erreurs_reseau_insignifiantes(reseau);
						if(erreurs_reseau_negligeables)
						{
							printf("erreurs des sorties du reseau negligeables pour le modele %d\n", i);
						}
						else 
						{
							printf("au moins une sortie du reseau a une erreur non negligeable\n");
							printf("modele %d non satisfait\n", i);				
							erreurs_modeles++;
							retropropagation(&reseau);
						}
					}
					
					printf("\nIl y a eu %d modele(s) erronés sur %d\n",erreurs_modeles, nb_modeles_a_apprendre);
					printf("boucle n°%d\n", nb_boucles);			
					
				}while(erreurs_modeles > 0);
				
				printf("TOUS LES MODELES SONT SATISFAITS AVEC CES POIDS!\n");
				
				enregistrement_biais_et_poids_reseau(reseau);
		
				break;
				
			case 2: //test apprentissage
				printf("TEST APPRENTISSAGE\n");
				recupere_biais_et_poids_enregistres(&reseau);
				int entree0 = get_entree();
				int entree1 = get_entree();
				printf("Les entrees du reseau sont %d et %d\n", entree0, entree1);
				propagation_avant_selon_entrees(&reseau, entree0, entree1);
				
				break;
			
			case 3:
				printf("Sortie du programme\n");
				break;
		}
	}
	
	
	
	//liberation pointeurs
	for(int i = 0; i < modeleComplet.nb_modeles ; i++)
		free(modeleComplet.modeles[i].entrees);
	free(modeleComplet.modeles);
	
	for(int i = 0 ; i< reseau.nb_couches ; i++)
	{
		for(int j = 0; j< reseau.couches[i].nb_perceptrons ; j++)
		{
			free(reseau.couches[i].perceptrons[j].entrees);
			free(reseau.couches[i].perceptrons[j].entrees_suivantes_liees);
		}
		free(reseau.couches[i].perceptrons);
	}
	free(reseau.couches);
	
	
	return 0;
}
