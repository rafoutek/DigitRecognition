#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "BmpLib.h"
#include "definitions.h"
#include "fonctions.h"
#include "fonctionsTraitementImage.h"

int main (void)
{
	clock_t start_t, end_t, total_t;

	int h=28,l=28; //images 28x28
	int **r, **v, **b, **g; //matrices pour lecture des images (en couleur puis gris puis seuillée)
	r = alloueMatriceInt(h,l);
	v = alloueMatriceInt(h,l);
	b = alloueMatriceInt(h,l);
	g = alloueMatriceInt(h,l);
	int nb_entrees = h*l; // chaque pixel est une entree
	int nb_sorties = 10; // chaque sortie correspond à un chiffre
	DonneesImageRGB *img;
	MODELE modele = init_modele(nb_entrees,nb_sorties);
	RESEAU reseau = init_reseau(modele);


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

				start_t = clock(); // temps depuis lancement du programme
				do{
					erreurs_modeles = 1; 
					
					nb_boucles ++;
					nb_modeles_a_apprendre  = 0;

					//on peut selectionner les modeles a prendre en compte pour l'apprentissage ici
					for( chiffre = 0; chiffre <= 9; chiffre++)
					{
						for( num = 0; num < 50; num++)
						{
							nb_modeles_a_apprendre++;
							if(AFFICHAGE)
								printf("\n\nMODELE %d.%d\n",chiffre,num);

							img = lis_imageModele(chiffre,num,chemin_image);
 							remplit_modele_depuis_image(img,&r,&v,&b,&g, &(modele));
							determine_sortieModeleAttendue(chiffre,&(modele));
							recopie_EntreesModele_dansEntreesReseau(modele, &reseau);
							propagation_avant_selon_modele(&reseau, modele.sorties_attendues);

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
							libereDonneesImageRGB(&img);
						}
					}
					if(AFFICHAGE || nb_boucles%100==0 || erreurs_modeles==0){
						printf("\nIl y a eu %d modele(s) erronés sur %d\n",erreurs_modeles, nb_modeles_a_apprendre);
						printf("boucle n°%d\n", nb_boucles);
						getchar();
					}
					
				}while(erreurs_modeles > 0);
				end_t = clock();
				total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

				printf("TOUS LES MODELES SONT SATISFAITS AVEC CES POIDS!\n");
				enregistrement_biais_et_poids_reseau(reseau);

				printf("Temps total pris par CPU pour trouver les bons parametres: %f\n", total_t  );
				free(chemin_image);
				break;
				
			case 2: //test apprentissage
				printf("TEST APPRENTISSAGE\n");
				recupere_biais_et_poids_enregistres(&reseau);
				//demande à l'utilisateur quelle image il veut test
				img = demande_et_lit_image_test();
				remplit_modele_depuis_image(img,&r,&v,&b,&g, &(modele));
				recopie_EntreesModele_dansEntreesReseau(modele, &reseau);
				propagation_avant(&reseau);
				cree_et_affiche_classement_ressemblance(reseau);
				break;
			
			case 3:
				printf("Sortie du programme\n");
				break;
		}
	}
	
	
	
	//liberation pointeurs
	free(img);
	libereMatriceInt(h,l,r);
	libereMatriceInt(h,l,v);
	libereMatriceInt(h,l,b);
	libereMatriceInt(h,l,g);
	free(modele.entrees);
	free(modele.sorties_attendues);
	
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
