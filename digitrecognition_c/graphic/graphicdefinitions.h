#define AFFICHAGE false
#define LARGEUR_FENETRE  800
#define HAUTEUR_FENETRE  600

#define ECART_AXES 30 // ecart du bord de l'ihm

#define MAX_Y 500. // max ordonnee
#define INTERVALLE_Y 50.

#define MAX_X 5000. // max abscisse
#define INTERVALLE_X 500.


typedef struct zone {
	int longueur, hauteur; //sert à determiner les coordonnees, ou inversement
	int espaceHaut, espaceBas; //peut servir dans le redimmensionnement en hauteur
	int espaceGauche, espaceDroite; //peut servir dans le redimmensionnement en largeur
	int xmin, ymin, xmax, ymax; // coordonnees de la diagonale de la zone rectangulaire
	char texte[30];
		
	}zone; // définition d'une zone cliquable


typedef struct _zones{
	zone z1, z2, z3, z4, z5, z6, z7, z8, z9;
	}_zones;

