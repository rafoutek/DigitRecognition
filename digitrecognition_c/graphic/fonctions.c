#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "BmpLib.h"
#include "NNdefinitions.h"
#include "fonctions.h"
#include "fonctionsTraitementImage.h"


//fonction de creation du modele complet
MODELE_COMPLET init_modeleComplet(int nb_modeles, int nb_entrees, int nb_sorties)
{	
	MODELE_COMPLET modeleComplet;
	modeleComplet.nb_modeles = nb_modeles;
	modeleComplet.modeles = (MODELE *)malloc(sizeof(MODELE) * modeleComplet.nb_modeles);  

	modeleComplet.nb_entrees = nb_entrees;
	modeleComplet.nb_sorties = nb_sorties;

	for(int i = 0; i < modeleComplet.nb_modeles ; i++)
	{
		modeleComplet.modeles[i] = init_modele(modeleComplet.nb_entrees,modeleComplet.nb_sorties);
	}
	return modeleComplet;
}

MODELE init_modele(int nb_entrees, int nb_sorties)
{
	MODELE modele;

	modele.nb_entrees = nb_entrees;
	modele.entrees = (ENTREE *)malloc(sizeof(ENTREE) * nb_entrees);
	modele.nb_sorties = nb_sorties;
	modele.sorties_attendues = (double *)malloc(sizeof(double) * nb_sorties);	

	return modele;
}

void remplit_imageModele(DonneesImageRGB *img, int chiffre, int num, char *chemin_image)
{
	sprintf(chemin_image, "./img_learn_bmp/%d.%d.bmp", chiffre,num);
	remplitBMPRGB(chemin_image, img);
	if(img == NULL)
	{
		perror("erreur lecture image");
		free(img);
		exit(1);
	}
}

DonneesImageRGB *lis_imageModele( int chiffre, int num, char *chemin_image)
{
	sprintf(chemin_image, "./img_learn_bmp/%d.%d.bmp", chiffre,num);
	DonneesImageRGB *img =  lisBMPRGB(chemin_image);
	if(img == NULL)
	{
		perror("erreur lecture image");
		libereDonneesImageRGB(&img);
		exit(1);
	}
	return img;
}

void determine_sortieModeleAttendue(int chiffre, MODELE *modele)
{
	//resets the array values to 0
	memset(modele->sorties_attendues, 0, modele->nb_sorties*sizeof(double));

	//digit 0 is defined by the sortie_attendue[0]=1 
	//digit 1 is defined by sortie_attendue[1]=1 
	//etc..
	modele->sorties_attendues[chiffre] = 1;
}

void remplit_modele_depuis_image(DonneesImageRGB *image, int ***r, int ***v, int ***b, int ***g, MODELE *modele)
{
	remplit_matricesCouleurInt(image,r,v,b);
	remplit_matriceGrise(28,28,*b,*v,*r, g);
	seuillage(28,28,g,100);
	conversionValeursMatriceInt_deTaille_etRemplissageEntreesModele(*g,28,28,modele);
}


void recopie_EntreesModele_dansEntreesReseau(MODELE modele, RESEAU *reseau)
{
	if(AFFICHAGE)
		printf("Recopie entrees modele dans reseau\n");
	for(int i = 0; i < reseau->couches[0].nb_neurones; i++)
	{
		for(int j = 0; j < reseau->couches[0].neurones[i].nb_entrees; j++)
		{
			reseau->couches[0].neurones[i].entrees[j].x = modele.entrees[j].x;
		}
	}
}

//valeur matrice: [0;255] -> entree modele ]0;1]
void conversionValeursMatriceInt_deTaille_etRemplissageEntreesModele(int **mat, int h, int l, MODELE *modele)
{
	int k=0;
	double b = 0;//pow(10,-1); //0 devient 0.1
	double a = (1.-b)/255; // 255 devient 1

	for(int i = 0; i < l; i++)
	{
		for(int j = 0; j < h; j++)
		{
			modele->entrees[k].x = a * mat[i][j] + b;
			k++;
		}	
	}
}

//affiche les entrees du premier neurone de la premiere couche
//doit contenir toutes les entrees du modele (comme tous les neurones de la premiere couche)
void affiche_entrees_reseau(RESEAU reseau)
{
	printf("Entrees du reseau:\n");
	for(int i = 0; i < reseau.couches[0].neurones[0].nb_entrees; i++)
	{
		printf(" entree reseau %d = %f\n",i,reseau.couches[0].neurones[0].entrees[i].x);
	}
}

// A REVOIR !!! MARCHE PAS
// set the number of neurones depending on the layer
// these constants are defined in definitions.h
void init_nb_neurones_couche(int couche, int *nb_neurones)
{
	switch(couche)
	{
		case 0:
			(*nb_neurones) = NB_NEURONES_LAYER_1;
			break;
		case 1:
			(*nb_neurones) = NB_NEURONES_LAYER_2;
			break;
		case 2:
			(*nb_neurones) = NB_NEURONES_LAYER_3;
			break;
	}
}

//creates a structured network of neurones thanks to the global model of data (number of inputs), 
//change the network caracteristics/values in the function as you want
RESEAU init_reseau(MODELE modele)
{
	RESEAU reseau;
	reseau.nb_couches = NB_LAYERS; // cf definitions.h
	reseau.couches = (COUCHE *)malloc(sizeof(COUCHE) * reseau.nb_couches);
	
	// numérotation des couches
	for(int i = 0; i < reseau.nb_couches; i++)
	{
		reseau.couches[i].numero_couche = i;
	}
	
	//couche cachee
	for(int i = 0; i< reseau.nb_couches-1; i++)
	{
		init_nb_neurones_couche(i,&(reseau.couches[i].nb_neurones));
		printf("nb neurones couche cachee %d = %d\n", i, reseau.couches[i].nb_neurones);
	}

	//couche sortie avec 10 neurones/sorties
	reseau.couches[reseau.nb_couches-1].nb_neurones = modele.nb_sorties; 
	

	for(int i = 0 ; i< reseau.nb_couches ; i++)
	{
		reseau.couches[i].neurones = (PERCEPTRON *)malloc(sizeof(PERCEPTRON) * reseau.couches[i].nb_neurones);
		
		for(int j = 0; j< reseau.couches[i].nb_neurones; j++)
		{
			if(i==0){
				//ici chaque neurone de la premiere couche cachee a <nb_entrees> entrees
				reseau.couches[i].neurones[j].nb_entrees = modele.nb_entrees;
			}
			else{
				//sinon chaque neurone des couches suivantes ont autant d'entrees que le nb de neurones de la couche precedente
				reseau.couches[i].neurones[j].nb_entrees = reseau.couches[i-1].nb_neurones;
			}
			reseau.couches[i].neurones[j].entrees = (ENTREE *)malloc(sizeof(ENTREE) * reseau.couches[i].neurones[j].nb_entrees);
		} 
		
		//liens entre entrees neurones de la couche et entrees suivantes de la couche precedente
		if(i > 0) 
		{
			for (int k = 0; k < reseau.couches[i-1].nb_neurones; k++)
			{
				reseau.couches[i-1].neurones[k].nb_entrees_suivantes_liees = reseau.couches[i].nb_neurones;
				reseau.couches[i-1].neurones[k].entrees_suivantes_liees = (ENTREE **)malloc(sizeof(ENTREE *) * reseau.couches[i].nb_neurones);
				
				for(int l = 0; l < reseau.couches[i].nb_neurones; l++)
				{
					reseau.couches[i-1].neurones[k].entrees_suivantes_liees[l] = reseau.couches[i].neurones[l].entrees ;
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
		for(j = 0; j < reseau->couches[i].nb_neurones; j++)
		{
			for(k = 0; k < reseau->couches[i].neurones[j].nb_entrees; k++)
			{			
				(*reseau).couches[i].neurones[j].entrees[k].poids = (double)rand()/RAND_MAX - 0.5;
			}
		}
	}		
}

void init_biais_Reseau(RESEAU *reseau)
{
	int i,j;
	
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for(j = 0; j < reseau->couches[i].nb_neurones; j++)
		{
			(*reseau).couches[i].neurones[j].biais = 0.;
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
		printf(" entree %d = %f\n",i,modele.entrees[i].x);
	}
	for(int i = 0; i < modele.nb_sorties; i++)
	{
		printf(" sortie attendue %d = %f\n",i,modele.sorties_attendues[i]);
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
		printf(" couche %d a %d neurone(s)\n", i, reseau.couches[i].nb_neurones);
		for(j = 0; j < reseau.couches[i].nb_neurones; j++)
		{
			printf("  neurone %d a %d entree(s)\n", j, reseau.couches[i].neurones[j].nb_entrees);
			for(k = 0; k < reseau.couches[i].neurones[j].nb_entrees; k++)
			{	
				printf("   entree = %f\n",reseau.couches[i].neurones[j].entrees[k].x);
				printf("   poids = %f\n",reseau.couches[i].neurones[j].entrees[k].poids);				
			}
			printf("  sortie = %e\n",reseau.couches[i].neurones[j].sortie);
			printf("  erreur globale = %e\n",reseau.couches[i].neurones[j].erreur_globale);
			printf("  erreur locale = %e\n",reseau.couches[i].neurones[j].erreur_locale);
			printf("  nb entrees couche suivante liees = %d\n",reseau.couches[i].neurones[j].nb_entrees_suivantes_liees);			
		}
	}		
}


//fait la somme pondérée du neurone et la retourne
double somme_neurone(PERCEPTRON neurone)
{
  double sum = 0;
	int i = 0;
  do{
	  sum = sum + neurone.entrees[i].x * neurone.entrees[i].poids;
	  i++;
  }while(i<neurone.nb_entrees);
  
	sum = sum + neurone.biais;  //on ajoute le biais à la fin

	return sum;
}

//fait la somme pondérée du neurone et l'enregistre dans la variable sum passée en parametre
void somme_neurone_V2(PERCEPTRON neurone, double *sum)
{
	int i = 0;
	*sum = 0;
  do{
	  *sum = *sum + neurone.entrees[i].x * neurone.entrees[i].poids;
	  i++;
  }while(i<neurone.nb_entrees);
  
	*sum = *sum + neurone.biais;  //on ajoute le biais à la fin
}

double fonction_transfert_sigmoide( double somme)
{
	double valeur_sigmoide;
	
	valeur_sigmoide = 1./(1. + exp(-somme));
	//printf("\nsomme = %f, val sigmoide = %e\n",somme, valeur_sigmoide);
	
	return valeur_sigmoide;
}

int fonction_transfert_seuil (double somme) 
{
	if(somme > 0)
		return 1;
	else
		return 0;
	
}

//a partir des entrees du reseau, determine la sortie (et les entrees) de chaque neurone du reseau
void propagation_avant_selon_modele (RESEAU *reseau, double *sorties_attendues)
{
	int i,j;
	//double val_fonction_transfert;

	if(AFFICHAGE)
		printf("\nPROPAGATION AVANT\n");
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for(j = 0; j < reseau->couches[i].nb_neurones; j++)
		{
			//fait la somme du neurone a partir de ses entrees et des poids correspondants
			somme_neurone_V2(reseau->couches[i].neurones[j], &(reseau->couches[i].neurones[j].sortie_somme));
			
			//fait la fonction transfert sigmoide pour chaque neurone
			reseau->couches[i].neurones[j].sortie_sigmoide = fonction_transfert_sigmoide(reseau->couches[i].neurones[j].sortie_somme);
			
			//enregistre la sortie de la fonction transfert du neurone, ici sigmoide
			reseau->couches[i].neurones[j].sortie = reseau->couches[i].neurones[j].sortie_sigmoide;
			
			//passe cette  valeur à l'entree du neurone de la couche suivante
			//si couche suivant il y a
			if( i+1 < reseau->nb_couches)
			{
				if(AFFICHAGE){
					printf("Couche cachee %d:\n", i);
					printf("Sortie neurone %d = %e\n",j, reseau->couches[i].neurones[j].sortie);
				}
				for(int k = 0; k < reseau->couches[i].neurones[j].nb_entrees_suivantes_liees ; k++)
				{
					reseau->couches[i].neurones[j].entrees_suivantes_liees[k][j].x = reseau->couches[i].neurones[j].sortie;
				}
				if(AFFICHAGE)
					printf("Passée à l'entree %d des neurones de la couche suivante\n",j);
			}
			else //on a atteint la derniere couche donc on calcule l'erreur globale
			{
				if(AFFICHAGE)
					printf("Perceptron final %d:\n",j);
				(*reseau).couches[i].neurones[j].erreur_globale = erreur_globale_couche_finale((*reseau).couches[i].neurones[j] , sorties_attendues[j]) ;
			}
		}
		//passe au neurone suivant de la meme couche
	}
	//passe à la couche suivante (avec entrees determinees)
}

char* demande_utilisateur_image_a_test(void)
{
	char *cheminImageTest = (char *)malloc(sizeof(char)*50);
	char nom[10];

	printf("Entrez le nom de l'image(sans l'extension): ");
	scanf("%s", nom);
	getchar();
	sprintf(cheminImageTest, "./img_learn_bmp/%s.bmp",nom);
	printf("Chemin de l'image à tester: %s\n", cheminImageTest);

	return cheminImageTest;
}

DonneesImageRGB* demande_et_lit_image_test(void)
{
	DonneesImageRGB *img = NULL;
	char *chemin_image;

	do{
		chemin_image = demande_utilisateur_image_a_test();
		img = lisBMPRGB(chemin_image);
		if(img == NULL)
			perror("erreur lecture image");
	}while(img == NULL);
	free(chemin_image);
	return img;
}

void propagation_avant (RESEAU *reseau)
{
	int i,j;
	double val_fonction_transfert;
	
	if(AFFICHAGE) printf("\nPROPAGATION AVANT\n");
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for(j = 0; j < reseau->couches[i].nb_neurones; j++)
		{
			//fait la somme du neurone a partir de ses entrees et des poids correspondants
			reseau->couches[i].neurones[j].sortie_somme = somme_neurone(reseau->couches[i].neurones[j]);
			
			//fait la fonction transfert sigmoide pour chaque neurone
			val_fonction_transfert = fonction_transfert_sigmoide(reseau->couches[i].neurones[j].sortie_somme);				
			reseau->couches[i].neurones[j].sortie_sigmoide = val_fonction_transfert;
			
			//enregistre la sortie de la fonction transfert du neurone
			reseau->couches[i].neurones[j].sortie = val_fonction_transfert;
			
			//passe cette valeur à l'entree des neurones de la couche suivante
			//si couche suivant il y a
			if( i+1 < reseau->nb_couches)
			{
				for(int k = 0; k < reseau->couches[i].neurones[j].nb_entrees_suivantes_liees ; k++)
				{
					reseau->couches[i].neurones[j].entrees_suivantes_liees[k][j].x = reseau->couches[i].neurones[j].sortie;
				}
			}
			else //on a atteint la derniere couche donc on affiche la sortie
			{
				if(AFFICHAGE) printf("La sortie du neurone %d de la couche sortie = %f\n", j, reseau->couches[i].neurones[j].sortie);
			}
		}
		//passe au neurone suivant de la meme couche
	}
	//passe à la couche suivante (avec entrees determinees)
}

// pour une image testée on affiche sa ressemblance avec chaque chiffre
void cree_et_affiche_classement_ressemblance(RESEAU reseau)
{
	CLASSEMENT tab_classement [10];

	//on recopie d'abord les sorties dans le tableau
	for(int i = 0; i < 10; i++)
	{
		tab_classement[i].chiffre = i;
		tab_classement[i].pourcentage_ressemblance = reseau.couches[reseau.nb_couches-1].neurones[i].sortie * 100;
	}

	// on trie
	int i,j;
	CLASSEMENT placeTemporaire;
	for (i=0; i<10; i++)
	{
		for(j=i; j<10; j++)
		{
			if(tab_classement[j].pourcentage_ressemblance > tab_classement[i].pourcentage_ressemblance) 
			{
				placeTemporaire = tab_classement[i];
				tab_classement[i] = tab_classement[j];
				tab_classement[j] = placeTemporaire;
			}
		}
	}
	// on affiche
	printf("\nClassement pourcentage ressemblance:\n");
	for(int k = 0; k < 10; k++)
	{
		printf("\t%d\t%f %%\n",tab_classement[k].chiffre, tab_classement[k].pourcentage_ressemblance);
	}
	
}

//dans ce réseau le signal d'erreur global et local ne comprend qu'une valeur (car une seuile sortie)
double erreur_globale_couche_finale( PERCEPTRON neurone_final, double sortie_attendue )
{
	double e;
	
	e = sortie_attendue - neurone_final.sortie ;
	if(AFFICHAGE)
		printf("sortie attendue = %.0f, sortie finale = %e :\n\terreur globale finale = %f\n", sortie_attendue, neurone_final.sortie, e);
		
	return e;
}
double erreur_locale_couche_finale( PERCEPTRON neurone_final, double erreur_globale )
{
	double gradient;
	
	gradient = erreur_globale * neurone_final.sortie * (1 - neurone_final.sortie) ;
	if(AFFICHAGE)
		printf("erreur globale finale = %e, sortie finale = %e :\n\terreur locale finale = %e\n", erreur_globale, neurone_final.sortie, gradient);
		
	return gradient;
}

//enregistre l'erreur locale du neurone dans ses entrées, sert pour la retropropagation
void enregistre_erreur_locale_dans_entreesPerceptron (PERCEPTRON *neurone)
{
	for (int i = 0; i < neurone->nb_entrees; i++)
	{
		neurone->entrees[i].erreur_locale = neurone->erreur_locale;
		//printf("erreur locale enregistree dans entree %d = %f\n", i,neurone->entrees[i].erreur_locale);
	}
}

void calcul_et_enregistre_erreurLocalePerceptron (PERCEPTRON *neurone, int num_neurone)
{
	int i;
	double mul = 0;
	
	for (i=0; i< neurone->nb_entrees_suivantes_liees; i++)
	{
		mul += neurone->entrees_suivantes_liees[i][num_neurone].poids * neurone->entrees_suivantes_liees[i]->erreur_locale;
		//printf("entree %d = %f avec poids = %f et erreur locale neurone = %f\n",i, neurone.entrees_suivantes_liees[i]->x,neurone.entrees_suivantes_liees[i]->poids, neurone.entrees_suivantes_liees[i]->erreur_locale);
		//printf("tour %d mul = %f\n",i, mul);
	}
	
	neurone->erreur_locale = neurone->sortie * (1 - neurone->sortie ) * mul;
	if(AFFICHAGE)
		printf("erreur locale neurone couche cachee = %e\n", neurone->erreur_locale);
}



void retropropagation (RESEAU *reseau)
{
	int i,j,k;
	double erreur_locale;
	if(AFFICHAGE)
		printf("\nRETROPROPAGATION\n");
	for (i = (*reseau).nb_couches-1; i >= 0 ; i--) //rétropropagation à partir de la couche de sortie
	{
		if(AFFICHAGE)
			printf("couche %d\n", i);
		for (j = 0; j < (*reseau).couches[i].nb_neurones; j++)//erreur pour chaque neurone de la couche
		{
			if(AFFICHAGE)
				printf(" neurone %d\n", j);			
			if( i == (*reseau).nb_couches-1 )
			{
				erreur_locale = erreur_locale_couche_finale((*reseau).couches[i].neurones[j] , (*reseau).couches[i].neurones[j].erreur_globale) ;
				//enregistrement dans structure
				(*reseau).couches[i].neurones[j].erreur_locale = erreur_locale;	
				enregistre_erreur_locale_dans_entreesPerceptron(&((*reseau).couches[i].neurones[j]));			
			}
			else   //calcule l'erreur locale de chaque neurone des couches cachees
			{
				calcul_et_enregistre_erreurLocalePerceptron(&((*reseau).couches[i].neurones[j]),j);
			}
			if(AFFICHAGE)
				printf("  ancien biais = %e, ", (*reseau).couches[i].neurones[j].biais);
			if( ACTIVATION_BIAIS == 1)
				(*reseau).couches[i].neurones[j].biais = (*reseau).couches[i].neurones[j].biais + LEARNING_SPEED * (*reseau).couches[i].neurones[j].erreur_locale;
			if(AFFICHAGE)
				printf("  nouveau biais = %e\n", (*reseau).couches[i].neurones[j].biais);
			
			//modif des poids des entrees du neurone
			for (k = 0; k < (*reseau).couches[i].neurones[j].nb_entrees ; k++)
			{
				if(AFFICHAGE){
					printf("  entree %d = %e", k,(*reseau).couches[i].neurones[j].entrees[k].x);
					printf(" erreur locale = %e \n\tancien poids  = %e\n", (*reseau).couches[i].neurones[j].erreur_locale , (*reseau).couches[i].neurones[j].entrees[k].poids);
				}
				(*reseau).couches[i].neurones[j].entrees[k].poids = (*reseau).couches[i].neurones[j].entrees[k].poids + LEARNING_SPEED * (*reseau).couches[i].neurones[j].erreur_locale * (*reseau).couches[i].neurones[j].entrees[k].x; // W* = W + n * erreur locale neurone * entree correspondant a ce poids
				if(AFFICHAGE)
					printf("\tnouveau poids = %e\n", (*reseau).couches[i].neurones[j].entrees[k].poids);
			}
		}
	}
}

bool erreur_insignifiante ( double erreur_globale)
{
	if(erreur_globale >= -pow(10, -ERROR) && erreur_globale <= pow(10, -ERROR)) //puissance à changer, plus elle est grande plus l'apprentissage est long, plus les poids sont précis
		return true;
	else return false;
}

//test si l'erreur de chaque neurone de la sortie est negligeable, retourne false si au moins une erreur n'est pas negligeable
bool erreurs_reseau_insignifiantes (RESEAU reseau)
{
	bool erreur_negligeable;
	
	for(int i = 0; i < reseau.couches[reseau.nb_couches-1].nb_neurones; i++)
	{
		erreur_negligeable = erreur_insignifiante(reseau.couches[reseau.nb_couches-1].neurones[i].erreur_globale);
		if(erreur_negligeable)
		{
			if(AFFICHAGE)
				printf("\nerreur insignifiante pour le neurone-sortie %d\n", i);
		}
		else 
		{
			if(AFFICHAGE)
				printf("\nerreur non negligeable pour le neurone-sortie %d\n", i);		
			return false;
		}
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
	
	//ecrit le poids de chaque entree de chaque neurone de chaque couche du reseau
	printf("Enregistrement des biais et des poids du reseau dans %s\n", nom_fichier);
	for(i = 0; i < reseau.nb_couches; i++)
	{
		for (j = 0; j < reseau.couches[i].nb_neurones ; j++)
		{
			fprintf(fichier_poids, "%f ", reseau.couches[i].neurones[j].biais);
			for (k = 0; k < reseau.couches[i].neurones[j].nb_entrees; k++)
			{
				fprintf(fichier_poids, "%f ", reseau.couches[i].neurones[j].entrees[k].poids);
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
	printf("Recuperation des biais et des poids du reseau dans %s\n", nom_fichier);
	for(i = 0; i < reseau->nb_couches; i++)
	{
		for (j = 0; j < reseau->couches[i].nb_neurones ; j++)
		{
			fscanf(fichier_poids, "%lf ", &(reseau->couches[i].neurones[j].biais));
			for (k = 0; k < reseau->couches[i].neurones[j].nb_entrees; k++)
			{
				fscanf(fichier_poids, "%lf ", &(reseau->couches[i].neurones[j].entrees[k].poids));
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

// fonction qui exectute une boucle, doit etre exécutée plsrs fois
// va être exécutée dans temporisation
// donc il faut sortir de la fonction les variables qui nous interessent pour l'affichage sur le graph
void apprentissage(int ***r, int ***v, int ***b,int ***g, DonneesImageRGB *img, MODELE *modele, RESEAU *reseau, char *chemin_image,  int *erreurs_modeles, int *nb_boucles)
{
	int nb_modeles_a_apprendre = 0;
	*erreurs_modeles = 0;

	//on peut selectionner les modeles a prendre en compte pour l'apprentissage ici
	for( int chiffre = 0; chiffre <= 9; chiffre++)
	{
		for( int num = 0; num < 50; num++)
		{
			nb_modeles_a_apprendre++;
			if(AFFICHAGE)
				printf("\n\nMODELE %d.%d\n",chiffre,num);

			img = lis_imageModele(chiffre,num,chemin_image);
			remplit_modele_depuis_image(img,r,v,b,g, modele);
			determine_sortieModeleAttendue(chiffre,modele);
			recopie_EntreesModele_dansEntreesReseau(*modele, reseau);
			propagation_avant_selon_modele(reseau, modele->sorties_attendues);

			if( erreurs_reseau_insignifiantes(*reseau) ){
				if(AFFICHAGE)
					printf("erreurs des sorties du reseau negligeables pour le modele %d.%d\n", chiffre,num);
			}
			else 
			{
				if(AFFICHAGE){
					printf("au moins une sortie du reseau a une erreur non negligeable\n");
					printf("modele %d.%d non satisfait\n",chiffre,num);	
				}			
				(*erreurs_modeles)++;
				retropropagation(reseau);
			} 
			libereDonneesImageRGB(&img); // libere l'espace allouée à la lecture de l'image modèle
		}
	}
	//printf("nb boucle = %d, erreurs = %d\n", *nb_boucles,*erreurs_modeles);

	if(AFFICHAGE || (*nb_boucles)%100==0 || *erreurs_modeles==0){
		printf("\nIl y a eu %d modele(s) erronés sur %d\n",*erreurs_modeles, nb_modeles_a_apprendre);
		printf("boucle n°%d\n", *nb_boucles);
	}
}