#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "definitions.h"
#include "fonctions.h"

int main (void)
{
	
	//RESEAU A 2 ENTREES ET 1 SORTIE	
	
	MODELE_COMPLET modeleComplet;
	modeleComplet.nb_modeles = 4; //on a 4 modeles pour determiner le XOR
	modeleComplet.modeles = (MODELE *)malloc(sizeof(MODELE) * modeleComplet.nb_modeles); 
	
	//allocation tableau d'entrees de chaque modele
	modeleComplet.nb_entrees = 2;
	for(int i = 0; i < modeleComplet.nb_modeles ; i++)
	{
		modeleComplet.modeles[i].nb_entrees = modeleComplet.nb_entrees;
		modeleComplet.modeles[i].entrees = (ENTREE *)malloc(sizeof(ENTREE) * modeleComplet.nb_entrees);
	}
	
	//remplissage modeles selon table de verite du XOR
	modeleComplet.modeles[0].entrees[0].x = 0;
	modeleComplet.modeles[0].entrees[1].x = 0;
	modeleComplet.modeles[0].sortie_attendue = 0;
	
	modeleComplet.modeles[1].entrees[0].x = 0;
	modeleComplet.modeles[1].entrees[1].x = 1;
	modeleComplet.modeles[1].sortie_attendue = 1;
	
	modeleComplet.modeles[2].entrees[0].x = 1;
	modeleComplet.modeles[2].entrees[1].x = 0;
	modeleComplet.modeles[2].sortie_attendue = 1;
	
	modeleComplet.modeles[3].entrees[0].x = 1;
	modeleComplet.modeles[3].entrees[1].x = 1;
	modeleComplet.modeles[3].sortie_attendue = 0;		

	
	//~ affiche_modele_complet(modeleComplet);
	
	RESEAU reseau;
	reseau.nb_couches = 2;
	reseau.couches = (COUCHE *)malloc(sizeof(COUCHE) * reseau.nb_couches);
	
	//initialisation du reseau
	reseau.couches[0].numero_couche = 0;
	reseau.couches[0].nb_perceptrons = 2;
	
	reseau.couches[1].numero_couche = 1;
	reseau.couches[1].nb_perceptrons = 1;
	
	for(int i = 0 ; i< reseau.nb_couches ; i++)
	{
		reseau.couches[i].perceptrons = (PERCEPTRON *)malloc(sizeof(PERCEPTRON) * reseau.couches[i].nb_perceptrons);
		
		for(int j = 0; j< reseau.couches[i].nb_perceptrons; j++)
		{
			//ici chaque perceptron de chaque couche a 2 entrees
			reseau.couches[i].perceptrons[j].nb_entrees = modeleComplet.nb_entrees;
			reseau.couches[i].perceptrons[j].entrees = (ENTREE *)malloc(sizeof(ENTREE) * modeleComplet.nb_entrees);
		} 
		
		//liens entre entrees perceptrons de la couche et entrees suivantes de la couche precedente
		if(i > 0) 
		{
			for (int k = 0; k < reseau.couches[i-1].nb_perceptrons; k++)
			{
				reseau.couches[i-1].perceptrons[k].nb_entrees_suivantes_liees = reseau.couches[i].nb_perceptrons;
				reseau.couches[i-1].perceptrons[k].entrees_suivantes_liees = (ENTREE **)malloc(sizeof(ENTREE *) * reseau.couches[i].nb_perceptrons);
				
				for(int l = 0; l < reseau.couches[i].nb_perceptrons; l++)
				{
					//pointeur entree suivante 0 du perceptron 0 de la couche precedente  = adresse entree 0 perceptron 0 de la couche actuelle
					//pointeur entree suivante 0 du perceptron 1 de la couche precedente  = adresse entree 1 perceptron 0 de la couche actuelle
					reseau.couches[i-1].perceptrons[k].entrees_suivantes_liees[l] = &(reseau.couches[i].perceptrons[l].entrees[k]) ;
					//printf("entree a partir du perceptron precedent = %f\n",reseau.couches[i-1].perceptrons[k].entrees_suivantes_liees[l]->x);
				}
			}
		}
	}
	
	
	
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
					for( int i = 0; i < modeleComplet.nb_modeles-1; i++)
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
