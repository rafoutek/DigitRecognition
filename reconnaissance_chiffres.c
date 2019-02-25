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
	int **r, **v, **b, **g; //matrices pour lecture des images (en couleur puis gris puis seuillée)
	r = alloueMatriceInt(h,l);
	v = alloueMatriceInt(h,l);
	b = alloueMatriceInt(h,l);
	g = alloueMatriceInt(h,l);
	int nb_entrees = h*l;
	int nb_sorties = 9;
	DonneesImageRGB *img = (DonneesImageRGB*)calloc(1, sizeof(DonneesImageRGB));
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
				
				//initialisation des poids aleatoires des perceptrons
				srand(time(NULL));
				init_poids_alea_Reseau(&reseau);
				init_biais_Reseau(&reseau);  
				
				int erreurs_modeles;
				//bool erreurs_reseau_negligeables;
				
				int nb_boucles = 0;
				int nb_modeles_a_apprendre ;

				int chiffre, num;
				char *chemin_image = (char *)malloc(sizeof(char)*50);

				do{
					erreurs_modeles = 0; 
					
					nb_boucles ++;
					nb_modeles_a_apprendre  = 0;

					//on peut selectionner les modeles a prendre en compte pour l'apprentissage ici
					for( chiffre = 0; chiffre <= 9; chiffre++)
					{
						for( num = 0; num < 10; num++)
						{
							nb_modeles_a_apprendre++;
							
							lit_imageModele(img,chiffre,num,chemin_image);
							remplit_modele_depuis_image(img,&r,&v,&b,&g, &(modeleComplet.modeles[0]));
							determine_sortieModeleAttendue(chiffre,&(modeleComplet.modeles[0]));
							recopie_EntreesModele_dansEntreesReseau(modeleComplet.modeles[0], &reseau);

							if(AFFICHAGE)
								printf("\n\nMODELE %d.%d\n",chiffre,num);
							propagation_avant_selon_modele(&reseau, modeleComplet.modeles[0].sorties_attendues);

							if( erreurs_reseau_insignifiantes(reseau) ){
								if(AFFICHAGE)
									printf("erreurs des sorties du reseau negligeables pour le modele %d.%d\n", chiffre,num);
							}
							else 
							{
								if(AFFICHAGE){
									printf("au moins une sortie du reseau a une erreur non negligeable\n");
									printf("modele %d.%d non satisfait\n",chiffre,num);	
								}			
								erreurs_modeles++;
								retropropagation(&reseau);
							}
						}
					}
					if(AFFICHAGE || nb_boucles%1000==0 || erreurs_modeles==0){
						printf("\nIl y a eu %d modele(s) erronés sur %d\n",erreurs_modeles, nb_modeles_a_apprendre);
						printf("boucle n°%d\n", nb_boucles);
					}
					
				}while(erreurs_modeles > 0);
				
				printf("TOUS LES MODELES SONT SATISFAITS AVEC CES POIDS!\n");
				
				enregistrement_biais_et_poids_reseau(reseau);
		
				break;
				
			case 2: //test apprentissage
				printf("TEST APPRENTISSAGE\n");
				recupere_biais_et_poids_enregistres(&reseau);
				//demande à l'utilisateur quelle image il veut test
				img = demande_et_lit_image_test();
				remplit_modele_depuis_image(img,&r,&v,&b,&g, &(modeleComplet.modeles[0]));
				recopie_EntreesModele_dansEntreesReseau(modeleComplet.modeles[0], &reseau);
				propagation_avant(&reseau);
				
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
