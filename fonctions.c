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

#define ACTIVATION_BIAIS 1
#define LEARNING_SPEED 1

//fonction de creation du modele complet
MODELE_COMPLET init_modeleComplet(int nb_modeles, int nb_entrees, int nb_sorties)
{	
	MODELE_COMPLET modeleComplet;
	modeleComplet.nb_modeles = nb_modeles;
	modeleComplet.modeles = (MODELE *)malloc(sizeof(MODELE) * modeleComplet.nb_modeles);  

	//allocation tableau d'entrees de chaque modele
	modeleComplet.nb_entrees = nb_entrees;
	for(int i = 0; i < modeleComplet.nb_modeles ; i++)
	{
		modeleComplet.modeles[i].nb_entrees = modeleComplet.nb_entrees;
		modeleComplet.modeles[i].entrees = (ENTREE *)malloc(sizeof(ENTREE) * modeleComplet.nb_entrees);
	}
	//allocation tableau des sorties attendues de chaque modele
	modeleComplet.nb_sorties = nb_sorties;
	for(int i = 0; i < modeleComplet.nb_modeles ; i++)
	{
		modeleComplet.modeles[i].nb_sorties = modeleComplet.nb_sorties;
		modeleComplet.modeles[i].sorties_attendues = (double *)malloc(sizeof(double) * modeleComplet.nb_sorties);
	}	
	return modeleComplet;
}

void remplit_modeleComplet_XOR(MODELE_COMPLET *modeleComplet)
{
	//remplissage modeles selon table de verite du XOR
	modeleComplet->modeles[0].entrees[0].x = 0;
	modeleComplet->modeles[0].entrees[1].x = 0;
	modeleComplet->modeles[0].sorties_attendues[0] = 0;
	
	modeleComplet->modeles[1].entrees[0].x = 0;
	modeleComplet->modeles[1].entrees[1].x = 1;
	modeleComplet->modeles[1].sorties_attendues[0] = 1;
	
	modeleComplet->modeles[2].entrees[0].x = 1;
	modeleComplet->modeles[2].entrees[1].x = 0;
	modeleComplet->modeles[2].sorties_attendues[0] = 1;
	
	modeleComplet->modeles[3].entrees[0].x = 1;
	modeleComplet->modeles[3].entrees[1].x = 1;
	modeleComplet->modeles[3].sorties_attendues[0] = 0;		

	//~ affiche_modele_complet(modeleComplet);
}

DonneesImageRGB* lit_imageModele(int chiffre, int num)
{
	DonneesImageRGB *img = NULL;
	char chemin_image[50];
	
	sprintf(chemin_image, "./img_learn_bmp/%d.%d.bmp", chiffre,num);
	img = lisBMPRGB(chemin_image);
	if(img == NULL)
	{
		perror("erreur lecture image");
		exit(1);
	}
	return img;
}

void determine_sortieModeleAttendue(int chiffre, MODELE modele)
{
	//resets the array values to 0
	memset(modele.sorties_attendues, 0, modele.nb_sorties*sizeof(double));

	//digit 0 is defined by all the outputs at 0
	//digit 1 is defined by sortie_attendue[0]=1 
	//etc..
	if(chiffre>0)
		modele.sorties_attendues[chiffre-1] = 1;
}

void remplit_modeleComplet_image(DonneesImageRGB *image, MODELE_COMPLET *modeleComplet)
{
	int **r , **v, **b; //matrices couleur
	int **g; //matrice gris
	//int **nb; //matrice noir et blanc
	int k=0;
	cree3MatricesInt(image,&b,&v,&r);
	creeMatNG_V2(28,28,b,v,r,&g);
	//seuillage_V2(28,28,100,g,&nb);
	seuillage(28,28,&g,100);

	for(int i = 0; i < 28; i++)
	{
		for(int j = 0; j < 28; j++)
		{
			modeleComplet->modeles->entrees[k].x = g[i][j];
			k++;
		}
	}	
}

void test_image_to_model(MODELE_COMPLET *modeleComplet)
{
	DonneesImageRGB *img = lit_imageModele(0,0);
	remplit_modeleComplet_image(img, modeleComplet);
	affiche_modele(modeleComplet->modeles[0]);
}

//creates a structured network of perceptrons thanks to the global model of data (number of inputs), 
//change the network caracteristics/values in the function as you want
RESEAU init_reseau(MODELE_COMPLET modeleComplet)
{
	RESEAU reseau;
	reseau.nb_couches = 2;
	reseau.couches = (COUCHE *)malloc(sizeof(COUCHE) * reseau.nb_couches);
	
	//initialisation du reseau
		//couche cachee
	reseau.couches[0].numero_couche = 0;
	reseau.couches[0].nb_perceptrons = 2;
		//couche sortie avec 9 perceptrons/sorties
	reseau.couches[1].numero_couche = 1;
	reseau.couches[1].nb_perceptrons = 9; 
	
	for(int i = 0 ; i< reseau.nb_couches ; i++)
	{
		reseau.couches[i].perceptrons = (PERCEPTRON *)malloc(sizeof(PERCEPTRON) * reseau.couches[i].nb_perceptrons);
		
		for(int j = 0; j< reseau.couches[i].nb_perceptrons; j++)
		{
			//ici chaque perceptron de chaque couche a <nb_entrees> entrees
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
	return reseau;
}

//initialisation des poids entre -0.5 et 0.5
void init_poids_alea_modeleComplet(MODELE_COMPLET *modeleComplet)
{

	for(int i = 0; i < modeleComplet->nb_modeles; i++)
	{
		for(int j = 0; j < modeleComplet->nb_entrees; j++)
		{
			(*modeleComplet).modeles[i].entrees[j].poids = (double)rand()/RAND_MAX - 0.5;
		}
	}		
}


void init_poids_alea_Reseau(RESEAU *reseau)
{
	int i,j,k;
	
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for(j = 0; j < reseau->couches[i].nb_perceptrons; j++)
		{
			for(k = 0; k < reseau->couches[i].perceptrons[j].nb_entrees; k++)
			{			
				(*reseau).couches[i].perceptrons[j].entrees[k].poids = (double)rand()/RAND_MAX - 0.5;
			}
		}
	}		
}

void init_biais_Reseau(RESEAU *reseau)
{
	int i,j;
	
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for(j = 0; j < reseau->couches[i].nb_perceptrons; j++)
		{
			(*reseau).couches[i].perceptrons[j].biais = 0.;
		}
	}		
}


//valeur aleatoire dans [a;b[
int rand_a_b(int a, int b){

    return rand()%(b-a)+a ;
}

int choix_modele(void)
{
	return rand_a_b(0,4);
}

void affiche_modele (MODELE modele)
{
	for (int i = 0; i < modele.nb_entrees; i++)
	{
		printf(" entree = %f\n",modele.entrees[i].x);
	}
	for(int i = 0; i < modele.nb_sorties; i++)
	{
		printf(" sortie attendue = %f\n",modele.sorties_attendues[i]);
	}
}

void affiche_modele_complet(MODELE_COMPLET modeleComplet)
{
	printf("Modele comprenant %d modeles avec %d entrees\n", modeleComplet.nb_modeles,modeleComplet.nb_entrees);
	for(int i = 0; i < modeleComplet.nb_modeles; i++)
	{
		printf("modele %d\n",i);
		affiche_modele(modeleComplet.modeles[i]);
	}		
}


void affiche_reseau(RESEAU reseau)
{
	int i,j,k;
	printf("AFFICHAGE RESEAU\n");
	printf("reseau avec %d couche(s)\n", reseau.nb_couches);
	for(i = 0; i < reseau.nb_couches; i++)
	{
		printf(" couche %d a %d perceptron(s)\n", i, reseau.couches[i].nb_perceptrons);
		for(j = 0; j < reseau.couches[i].nb_perceptrons; j++)
		{
			printf("  perceptron %d a %d entree(s)\n", j, reseau.couches[i].perceptrons[j].nb_entrees);
			for(k = 0; k < reseau.couches[i].perceptrons[j].nb_entrees; k++)
			{	
				printf("   entree = %f\n",reseau.couches[i].perceptrons[j].entrees[k].x);
				printf("   poids = %f\n",reseau.couches[i].perceptrons[j].entrees[k].poids);				
			}
			//printf("  sortie somme = %f\n",reseau.couches[i].perceptrons[j].sortie_somme);
			//printf("   sortie sigmoide = %f\n",reseau.couches[i].perceptrons[j].sortie_sigmoide);
			//printf("   sortie seuil = %f\n",reseau.couches[i].perceptrons[j].sortie_seuil);
			printf("  sortie = %f\n",reseau.couches[i].perceptrons[j].sortie);
			printf("  erreur globale = %f\n",reseau.couches[i].perceptrons[j].erreur_globale);
			printf("  erreur locale = %f\n",reseau.couches[i].perceptrons[j].erreur_locale);
			printf("  nb entrees couche suivante liees = %d\n",reseau.couches[i].perceptrons[j].nb_entrees_suivantes_liees);			
		}
	}		
}



double somme_neurone(PERCEPTRON perceptron)
{
  double sum = 0;
	int i = 0;
  do{
	  sum = sum + perceptron.entrees[i].x * perceptron.entrees[i].poids;
	  i++;
  }while(i<perceptron.nb_entrees);
  
	sum = sum + perceptron.biais;  //on ajoute le biais à la fin

	return sum;
}

double fonction_transfert_sigmoide( double somme)
{
	double valeur_sigmoide;
	
	valeur_sigmoide = 1/(1 + exp(-somme));
	//printf("somme = %f, val sigmoide = %f\n",somme, valeur_sigmoide);
	
	return valeur_sigmoide;
}

int fonction_transfert_seuil (double somme) 
{
	if(somme > 0)
		return 1;
	else
		return 0;
	
}

//a partir des entrees du reseau, determine la sortie (et les entrees) de chaque perceptron du reseau
void propagation_avant_selon_modele (RESEAU *reseau, MODELE modele)
{
	int i,j;

	double val_fonction_transfert;
	
	//initialisation des entrees du reseau a partir du modele
	reseau->couches[0].perceptrons[0].entrees[0].x = reseau->couches[0].perceptrons[1].entrees[0].x = modele.entrees[0].x ;
	reseau->couches[0].perceptrons[0].entrees[1].x = reseau->couches[0].perceptrons[1].entrees[1].x = modele.entrees[1].x ;
	
	
	printf("\nPROPAGATION AVANT\n");
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for(j = 0; j < reseau->couches[i].nb_perceptrons; j++)
		{
			//fait la somme du perceptron a partir de ses entrees et des poids correspondants
			reseau->couches[i].perceptrons[j].sortie_somme = somme_neurone(reseau->couches[i].perceptrons[j]);
			
			//fait la fonction transfert sigmoide pour chaque perceptron
			val_fonction_transfert = fonction_transfert_sigmoide(reseau->couches[i].perceptrons[j].sortie_somme);				
			reseau->couches[i].perceptrons[j].sortie_sigmoide = val_fonction_transfert;
			
			//enregistre la sortie de la fonction transfert du perceptron
			reseau->couches[i].perceptrons[j].sortie = val_fonction_transfert;
			//printf("sortie = %f\n", reseau->couches[i].perceptrons[j].sortie);
			
			//passe cette valeur à l'entree du perceptron de la couche suivante
			//si couche suivant il y a
			if( i+1 < reseau->nb_couches)
			{
				//si la sortie vient du perceptron 0, cela correspond à l'entree 0
				if( j == 0 )
				{
					reseau->couches[i+1].perceptrons[0].entrees[0].x = reseau->couches[i].perceptrons[j].sortie;
				}
				//si vient du perceptron 1, correspond a entree 1 du perceptron de la prochaine couche
				else if( j== 1 )
					reseau->couches[i+1].perceptrons[0].entrees[1].x = reseau->couches[i].perceptrons[j].sortie;
			}
			else //on a atteint la derniere couche donc on calcule l'erreur globale
			{
				(*reseau).couches[i].perceptrons[j].erreur_globale = erreur_globale_couche_finale((*reseau).couches[i].perceptrons[j] , modele.sorties_attendues[j]) ;
			}
		}
		//passe au perceptron suivant de la meme couche
	}
	//passe à la couche suivante (avec entrees determinees)
}


void propagation_avant_selon_entrees (RESEAU *reseau, int entree0, int entree1)
{
	int i,j;

	double val_fonction_transfert;
	
	//initialisation des entrees du reseau a partir du modele
	reseau->couches[0].perceptrons[0].entrees[0].x = reseau->couches[0].perceptrons[1].entrees[0].x = entree0;
	reseau->couches[0].perceptrons[0].entrees[1].x = reseau->couches[0].perceptrons[1].entrees[1].x = entree1 ;
	
	
	printf("\nPROPAGATION AVANT\n");
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for(j = 0; j < reseau->couches[i].nb_perceptrons; j++)
		{
			//fait la somme du perceptron a partir de ses entrees et des poids correspondants
			reseau->couches[i].perceptrons[j].sortie_somme = somme_neurone(reseau->couches[i].perceptrons[j]);
			
			//fait la fonction transfert sigmoide pour chaque perceptron
			val_fonction_transfert = fonction_transfert_sigmoide(reseau->couches[i].perceptrons[j].sortie_somme);				
			reseau->couches[i].perceptrons[j].sortie_sigmoide = val_fonction_transfert;
			
			//enregistre la sortie de la fonction transfert du perceptron
			reseau->couches[i].perceptrons[j].sortie = val_fonction_transfert;
			//printf("sortie = %f\n", reseau->couches[i].perceptrons[j].sortie);
			
			//passe cette valeur à l'entree du perceptron de la couche suivante
			//si couche suivant il y a
			if( i+1 < reseau->nb_couches)
			{
				//si la sortie vient du perceptron 0, cela correspond à l'entree 0
				if( j == 0 )
				{
					reseau->couches[i+1].perceptrons[0].entrees[0].x = reseau->couches[i].perceptrons[j].sortie;
				}
				//si vient du perceptron 1, correspond a entree 1 du perceptron de la prochaine couche
				else if( j== 1 )
					reseau->couches[i+1].perceptrons[0].entrees[1].x = reseau->couches[i].perceptrons[j].sortie;
			}
			else //on a atteint la derniere couche donc on affiche la sortie
			{
				printf("La sortie du perceptron %d de la couche sortie = %f\n", j, reseau->couches[i].perceptrons[j].sortie);
			}
		}
		//passe au perceptron suivant de la meme couche
	}
	//passe à la couche suivante (avec entrees determinees)
}



//dans ce réseau le signal d'erreur global et local ne comprend qu'une valeur (car une seuile sortie)
double erreur_globale_couche_finale( PERCEPTRON perceptron_final, double sortie_attendue )
{
	double e;
	
	e = sortie_attendue - perceptron_final.sortie ;
	printf("sortie attendue = %.0f, sortie finale = %f :\n\terreur globale finale = %f\n", sortie_attendue, perceptron_final.sortie, e);
		
	return e;
}
double erreur_locale_couche_finale( PERCEPTRON perceptron_final, double erreur_globale )
{
	double gradient;
	
	gradient = erreur_globale * perceptron_final.sortie * (1 - perceptron_final.sortie) ;
	printf("erreur globale finale = %f, sortie finale = %f :\n\terreur locale finale = %f\n", erreur_globale, perceptron_final.sortie, gradient);
		
	return gradient;
}

//enregistre l'erreur locale du perceptron dans ses entrées, sert pour la retropropagation
void enregistre_erreur_locale_dans_entreesPerceptron (PERCEPTRON *perceptron)
{
	for (int i = 0; i < perceptron->nb_entrees; i++)
	{
		perceptron->entrees[i].erreur_locale = perceptron->erreur_locale;
		printf("erreur locale enregistree dans entree %d = %f\n", i,perceptron->entrees[i].erreur_locale);
	}
}

void calcul_et_enregistre_erreurLocalePerceptron (PERCEPTRON *perceptron)
{
	int i;
	double mul = 0;
	double erreur_locale;
	
	for (i=0; i< perceptron->nb_entrees_suivantes_liees; i++)
	{
		mul += perceptron->entrees_suivantes_liees[i]->poids * perceptron->entrees_suivantes_liees[i]->erreur_locale;
		//printf("entree %d = %f avec poids = %f et erreur locale perceptron = %f\n",i, perceptron.entrees_suivantes_liees[i]->x,perceptron.entrees_suivantes_liees[i]->poids, perceptron.entrees_suivantes_liees[i]->erreur_locale);
		//printf("tour %d mul = %f\n",i, mul);
	}
	
	erreur_locale = perceptron->sortie * (1 - perceptron->sortie ) * mul;
	perceptron->erreur_locale = erreur_locale;
	printf("erreur locale perceptron couche cachee = %f\n", perceptron->erreur_locale);
}



void retropropagation (RESEAU *reseau)
{
	int i,j,k;
	double erreur_locale;
	
	printf("\nRETROPROPAGATION\n");
	for (i = (*reseau).nb_couches-1; i >= 0 ; i--) //rétropropagation à partir de la couche de sortie
	{
		printf("couche %d\n", i);
		for (j = 0; j < (*reseau).couches[i].nb_perceptrons; j++)//erreur pour chaque perceptron de la couche
		{
			printf(" perceptron %d\n", j);
			
			if( i == (*reseau).nb_couches-1 )
			{
				erreur_locale = erreur_locale_couche_finale((*reseau).couches[(*reseau).nb_couches-1].perceptrons[j] , (*reseau).couches[i].perceptrons[j].erreur_globale) ;
				//enregistrement dans structure
				(*reseau).couches[i].perceptrons[j].erreur_locale = erreur_locale;	
				enregistre_erreur_locale_dans_entreesPerceptron(&((*reseau).couches[i].perceptrons[j]));			
			}
			else   //calcule l'erreur locale de chaque perceptron des couches cachees
			{
				calcul_et_enregistre_erreurLocalePerceptron(&((*reseau).couches[i].perceptrons[j]));
			}
			
			printf("  ancien biais = %f, ", (*reseau).couches[i].perceptrons[j].biais);
			if( ACTIVATION_BIAIS == 1)
				(*reseau).couches[i].perceptrons[j].biais = (*reseau).couches[i].perceptrons[j].biais + LEARNING_SPEED * (*reseau).couches[i].perceptrons[j].erreur_locale;
			printf("  nouveau biais = %f\n", (*reseau).couches[i].perceptrons[j].biais);
			
			//modif des poids des entrees du perceptron
			for (k = 0; k < (*reseau).couches[i].perceptrons[j].nb_entrees ; k++)
			{
				printf("  entree %d: ancien poids = %f , ",k, (*reseau).couches[i].perceptrons[j].entrees[k].poids);
				(*reseau).couches[i].perceptrons[j].entrees[k].poids = (*reseau).couches[i].perceptrons[j].entrees[k].poids + LEARNING_SPEED * (*reseau).couches[i].perceptrons[j].erreur_locale * (*reseau).couches[i].perceptrons[j].entrees[k].x; // W* = W + n * erreur locale perceptron * entree correspondant a ce poids
				printf("nouveau poids = %f\n", (*reseau).couches[i].perceptrons[j].entrees[k].poids);
			}
		}
	}
}

bool erreur_insignifiante ( double erreur_globale)
{
	if(erreur_globale >= -pow(10, -2) && erreur_globale <= pow(10, -2)) //puissance à changer, plus elle est grande plus l'apprentissage est long, plus les poids sont précis
		return true;
	else return false;
}

//test si l'erreur de chaque perceptron de la sortie est negligeable, retourne false si au moins une erreur n'est pas negligeable
bool erreurs_reseau_insignifiantes (RESEAU reseau)
{
	bool erreur_negligeable;
	
	for(int i = 0; i < reseau.couches[reseau.nb_couches-1].nb_perceptrons; i++)
	{
		erreur_negligeable = erreur_insignifiante(reseau.couches[reseau.nb_couches-1].perceptrons[i].erreur_globale);
		if(erreur_negligeable)
			printf("erreur insignifiante pour le perceptron-sortie %d\n", i);
		else printf("erreur non negligeable pour le perceptron-sortie %d\n", i);		
		if(!erreur_negligeable)
			return false;
	}
	return true;
}


void enregistrement_biais_et_poids_reseau(RESEAU reseau)
{
	int i,j,k;
	FILE *fichier_poids;
	char * nom_fichier = "./fichier_poids.txt";
	fichier_poids = fopen(nom_fichier, "wt");
	if(fichier_poids == NULL)
	{
		perror("Ouverture fichier poids");
		exit(1); //si erreur d'ouverture on quitte avec code d'erreur
	}
	
	//ecrit le poids de chaque entree de chaque perceptron de chaque couche du reseau
	printf("Enregistrement des biais et des poids du reseau dans %s\n", nom_fichier);
	for(i = 0; i < reseau.nb_couches; i++)
	{
		for (j = 0; j < reseau.couches[i].nb_perceptrons ; j++)
		{
			fprintf(fichier_poids, "%f ", reseau.couches[i].perceptrons[j].biais);
			for (k = 0; k < reseau.couches[i].perceptrons[j].nb_entrees; k++)
			{
				fprintf(fichier_poids, "%f ", reseau.couches[i].perceptrons[j].entrees[k].poids);
			}
			
		}
	}
}

//recupere les poids enregistrés et les rentre dans le reseau
void recupere_biais_et_poids_enregistres (RESEAU *reseau)
{
	int i,j,k;
	FILE *fichier_poids;
	char * nom_fichier = "./fichier_poids.txt";
	fichier_poids = fopen(nom_fichier, "rt");
	if(fichier_poids == NULL)
	{
		perror("Ouverture fichier poids");
		exit(1); //si erreur d'ouverture on quitte avec code d'erreur
	}
	
	//lit le poids dans le fichier texte et le rentre dans son entree associee
	printf("Recuperation des poids du reseau dans %s\n", nom_fichier);
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for (j = 0; j < reseau->couches[i].nb_perceptrons ; j++)
		{
			fscanf(fichier_poids, "%lf ", &(reseau->couches[i].perceptrons[j].biais));
			for (k = 0; k < reseau->couches[i].perceptrons[j].nb_entrees; k++)
			{
				fscanf(fichier_poids, "%lf ", &(reseau->couches[i].perceptrons[j].entrees[k].poids));
			}
			
		}
	}	
}


int menu (void)
{
	int choix;
	char s [10] = {0};
	char *endPtr;
	do {
		printf("\n----------MENU------------\n\n");
		printf(". 1 pour apprentissage\n");
		printf(". 2 pour tester apprentissage\n");
		printf(". 3 pour quitter le programme\n");
		fgets(s, 10, stdin);
		choix = strtol(s,&endPtr,10);
	}while(endPtr == s || choix > 3);
	
	return choix;
}

int get_entree(void)
{
	int entree;
	char s [10] = {0};
	char *endPtr;
	
	do{
		printf("Choisissez 0 ou 1 comme entree:\n");
		fgets(s, 10, stdin);
		entree = strtol(s,&endPtr,10);
	}while(endPtr == s || entree > 1);
	
	return entree;
}

