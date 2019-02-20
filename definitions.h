#include <stdbool.h>

typedef struct ENTREE ENTREE;
struct ENTREE{
	double x; 
	double poids;
	double erreur_locale;
};

//structures du modèle (exemples à entrer à la main)
typedef struct MODELE MODELE;
struct MODELE{
	int nb_entrees;
	ENTREE *entrees;
	int nb_sorties;
	double *sorties_attendues;
};

typedef struct MODELE_COMPLET MODELE_COMPLET;
struct MODELE_COMPLET{
	int nb_entrees;
	int nb_sorties;
	int nb_modeles;
	MODELE *modeles;
};

//structures du réseau
typedef struct PERCEPTRON PERCEPTRON;
struct PERCEPTRON{
	double biais;
	int nb_entrees;
	ENTREE *entrees;
	double sortie_somme;
	double sortie_sigmoide, sortie_seuil, sortie;
	double erreur_globale, erreur_locale; //erreur globale si perceptron appartient à la couche sortie
	int nb_entrees_suivantes_liees;
	ENTREE **entrees_suivantes_liees; //sortie du perceptron va à ces entrées
};

typedef struct COUCHE COUCHE;
struct COUCHE{
	int numero_couche;
	int nb_perceptrons;
	PERCEPTRON *perceptrons;
};

typedef struct RESEAU RESEAU;
struct RESEAU{
	int nb_couches;
	COUCHE *couches;
};
