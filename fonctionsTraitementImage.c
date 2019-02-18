#include <stdlib.h>	// Pour pouvoir utiliser void exit(int)
#include <stdio.h>		// Inclusion classique
#include <stdbool.h>	// Inclusion classique
#include <string.h>		// pour les chaines de caracteres
#include <math.h>
#include <complex.h>
#include <float.h>

#include "BmpLib.h"
#include "definitions.h"
#include "fonctionsTraitementImage.h"

#ifndef M_PI
#define M_PI 3.141592654
#endif

//allouer les matrices----------------
int ** alloueMatriceInt(int h, int l)
{
	int ** m;
	int i;
	m = (int **)malloc(sizeof(int *)*h);
	for(i=0;i<h;i++)
	{
		m[i] = (int *)malloc(sizeof(int)*l);
	}
	return m;
}

float ** alloueMatriceFloat(int h, int l)
{
	float ** m;
	int i;
	m = (float **)malloc(sizeof(float *)*h);
	for(i=0;i<h;i++)
	{
		m[i] = (float *)malloc(sizeof(float)*l);
	}
	return m;
}

//liberer les matrices----------------------
void libereMatriceInt(int h, int l,int ** m)
{
	int i;
	for(i=0;i<h;i++)
	{
		free(m[i]); 
	}

	free(m);
}

void libereMatriceFloat(int h, int l,float ** m)
{
	int i;
	for(i=0;i<h;i++)
	{
		free(m[i]); 
	}
	
	free(m);
}

//creer les 3 matrices a partir de l'image-------------
void cree3MatricesFloat(DonneesImageRGB * image, float *** bleu, float *** vert, float *** rouge)
{
	float ** b, ** v, ** r;
	int i,j,k = 0; //seulement k initialisé à 0
	r = alloueMatriceFloat(image->hauteurImage,image->largeurImage);
	v = alloueMatriceFloat(image->hauteurImage,image->largeurImage);
	b = alloueMatriceFloat(image->hauteurImage,image->largeurImage);
	for(i=0;i<image->hauteurImage;i++)
	{
		for(j=0;j<image->largeurImage;j++)
		{
			b[i][j] = image->donneesRGB[k];
			k++;
			v[i][j] = image->donneesRGB[k];
			k++;
			r[i][j] = image->donneesRGB[k];
			k++;
		}
	}
	
	
	* rouge = r;
	* vert = v;
	* bleu = b;
}

//initialisation des matrices-------------------------------
void initMatInt(int **mat, int h, int l)
{
	for(int i=0; i<h; i++)
		for(int j=0; j<l; j++)
			mat[i][j] = 0; // initialise toutes les cases à 0

}

void initMatFloat(float **mat, int h, int l)
{
	for(int i=0; i<h; i++)
		for(int j=0; j<l; j++)
			mat[i][j] = 0; // initialise toutes les cases à 0

}

//affiche matrices--------------------------------
void afficheMatInt(int **mat, int h, int l)
{
	for( int i=0 ; i<h ; i++)
	{
		for(int j=0; j<l; j++)
		{
			printf("mat[%d][%d] = %d\n", i,j,mat[i][j]);
		}
	}
}

void afficheMatFloat(float **mat, int h, int l) //peut servir pour debug
{
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l; j++)
		{
			if(mat[i][j] <0)
				printf("mat[%d][%d] = %f\n",i,j,mat[i][j]);
		}
		printf("\n");
	}
}


void allocation4MatricesInt(DonneesImageRGB * image, int *** b, int *** v, int *** r, int ***s)
{
	int h,l;
	h = image->hauteurImage;
	l = image->largeurImage;

	//allocation memoire des differentes matrices
	*r = alloueMatriceInt(h,l);
	*v = alloueMatriceInt(h,l);
	*b = alloueMatriceInt(h,l);
	*s = alloueMatriceInt(h,l);
}



void cree3MatricesInt(DonneesImageRGB * image, int *** bleu, int *** vert, int *** rouge)
{
	int ** b, ** v, ** r;
	int i,j,k = 0; //seulement k initialisé à 0
	r = alloueMatriceInt(image->hauteurImage,image->largeurImage);
	v = alloueMatriceInt(image->hauteurImage,image->largeurImage);
	b = alloueMatriceInt(image->hauteurImage,image->largeurImage);
	for(i=0;i<image->hauteurImage;i++)
	{
		for(j=0;j<image->largeurImage;j++)
		{
			b[i][j] = image->donneesRGB[k];
			k++;
			v[i][j] = image->donneesRGB[k];
			k++;
			r[i][j] = image->donneesRGB[k];
			k++;
		}
	}


	* rouge = r;
	* vert = v;
	* bleu = b;

}



DonneesImageRGB *creeImage(int h, int l, int **r, int **v, int **b)
{
	DonneesImageRGB *image;
	int i,j, k=0;

	image = (DonneesImageRGB *)malloc(sizeof(DonneesImageRGB));
	image->largeurImage = l;
	image->hauteurImage = h;
	image->donneesRGB = (unsigned char *)malloc(sizeof(unsigned char) *l*h*3);

	for(i=0; i<h; i++)
	{
		for(j=0; j<l; j++)
		{
			image->donneesRGB[k++] = b[i][j];
			image->donneesRGB[k++] = v[i][j];
			image->donneesRGB[k++] = r[i][j];
		}
	}

	return image;
}


void creeMatNG(DonneesImageRGB * image, int ** b, int ** v, int ** r, int ***s)
{
	printf("\nCréation matrice niveaux de gris en cours...\n");
	int h,l;
	h = image->hauteurImage;
	l = image->largeurImage;
	*s=alloueMatriceInt(h,l);
	for( int i=0 ; i<h ; i++)
	{
		for(int j=0; j<l; j++)
		{	
			(*s)[i][j] = (int) (0.2125*r[i][j] + 0.7154*v[i][j] + 0.0721*b[i][j]);
			//printf("%d\n",(*s)[i][j]);
		}
	}
	printf("Matrice niveaux de gris faite\n");
}


void initTab(float tab[256] , int taille)
{
	for (int i = 0; i < taille; i++)
	{
		tab[i] = 0; //remplie le tab de 0
	}
	
}

void creeHistogramme (int h, int l, int **mat, float histogramme [256])
{
	printf("\nCréation tableau histogramme en cours...\n");
	
	for( int i=0 ; i<h ; i++)
	{
		for(int j=0; j<l; j++)
		{
			histogramme[ mat[i][j] ] ++; //ce tableau contient par ex à l'indice 100 le nombre de pixels de l'image grise de valeur 100
		}
	}
	printf("Tableau histogramme fait\n");
}

void afficheTab(float *tab, int taille)
{
	for (int i=0; i<taille; i++)
	{
		if(i==0)  printf("tab[%d] = %f noir\n", i, tab[i]);
		else if(i==255) printf("tab[%d] = %f blanc\n", i, tab[i]);
		else printf("tab[%d] = %f\n", i, tab[i]);
	}
}



int maxTab (float tab[256], int taille)
{
	float max=0;
	
	for (int i=0; i<taille; i++)
	{
		if (tab[i] > max)
		{
			max = tab[i];
		}
	}
	
	return max;
}

void tabRatio (float histogramme[256])
{
	//float *tab_ratio = (float*)malloc(sizeof(float) * 256); 
	
	int max = maxTab(histogramme, 256);
	//printf("max histogramme = %d\n", max);
	
	for (int i = 0; i < 256 ; i++)
	{
		histogramme[i] = (histogramme[i] * 256) / max;
	}
	//return tab_ratio; 
}

void creeMatHistogramme (float histogramme[256], int ***hist)
{
	printf("\nCréation matrice filtre median en cours...\n");
	*hist = alloueMatriceInt(256, 256); //on alloue la mémoire de la matrice
	
	for (int j = 0; j < 256; j++)
	{
		for (int i = 0; i < 256; i++)
		{
			if(i > histogramme[j] ) //si 1 pixel de valeur 83, à l'indice 83 de la matrice il y aura 1 pixel noir et le reste de la colonne sera blanc
				(*hist)[i][j] = 255;  //le blanc sert de "fond"
				
			else (*hist)[i][j] = 0; //on met du noir pour afficher le ratio du nombre de pixels de la meme couleur (du meme gris)	 
		}
	}
	printf("Matrice histogramme créée\n");
}
 
 
float compare_histogramme (float histogramme1[256], float histogramme2[256])
{
	float resultat = 0;
	
	for (int i = 0; i < 256 ; i++)
	{
		resultat += pow(histogramme1[i] - histogramme2[i], 2);
	}
	
	return resultat;
}


void seuillage(int h, int l, int ***s, int seuil)
{
	printf("Seuillage de la matrice...\n");
	for( int i=0 ; i<h ; i++)
	{
		for(int j=0; j<l; j++)
		{	
			//si inferieur au seuil noir, sinon blanc
			if((*s)[i][j] < seuil)
			{
				(*s)[i][j]=0;
			}
			else{
				(*s)[i][j]=255;
			}
		}
	}
}

void tricroissant_tabmediane(int tab[8])
{
     int valeurtemporaire, i, j;
 
     for (i=0; i<9; i++)
     {
        for(j=i; j<9; j++)
        {
            if(tab[j]<tab[i]) 
            {
                valeurtemporaire = tab[i];
                tab[i] = tab[j];
                tab[j] = valeurtemporaire;
            }
        }
 
     }
}


void filtreMedian3x3( int **matrice, int ***mat_mediane, DonneesImageRGB *image)
{
	printf("\nCréation matrice filtre median en cours...\n");
	int i,j;
	int tab[9] ; //tab necessaire au tri des 9 valeurs pour trouver la valeur médiane
	//int **mat;
	
	//mat = alloueMatriceInt(image->hauteurImage, image->largeurImage);
	
	
	for(i=0; i<image->hauteurImage; i++){
		for(j=0; j<image->largeurImage ; j++){
			
			if(i==0 || i== image->hauteurImage -1 || j==0 || j==image->largeurImage-1 )
				(*mat_mediane)[i][j] = matrice[i][j] ;
				
			else{
				
				tab[0] = matrice[i][j] ;
				tab[1] = matrice[i-1][j] ;
				tab[2] = matrice[i+1][j] ;
				tab[3] = matrice[i][j-1] ;
				tab[4] = matrice[i][j+1] ;
				tab[5] = matrice[i-1][j-1] ;
				tab[6] = matrice[i+1][j+1] ;
				tab[7] = matrice[i-1][j+1] ;
				tab[8] = matrice[i+1][j-1] ;
				
				tricroissant_tabmediane(tab); //trie le tableau des valeurs récupérées
				/*printf("%d,%d\n",i,j);
				printf("h=%d, l=%d\n",image->hauteurImage,image->largeurImage);*/
				(*mat_mediane)[i][j] = tab[4];
				}
			}
		}
	printf("Matrice filtre median créée\n");
			
	//*mat_mediane = mat ;
}

void creeMatMedian(int **matrice, int ***mat_mediane, DonneesImageRGB *image)
{
	*mat_mediane = alloueMatriceInt(image->hauteurImage, image->largeurImage);
	filtreMedian3x3(matrice, mat_mediane, image);
}


//fonctions pour les filtres gradients--------------------------------------

void creeMatRobertsX( int **mat, int ***RobertsX, int h, int l)
{
	*RobertsX = alloueMatriceInt(h,l);
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			if(i==0 || i== h-1 || j==0 || j==l-1 )
			{
				(*RobertsX)[i][j] = mat[i][j];
			}
			else{
				(*RobertsX)[i][j] = mat[i][j+1] - mat[i][j];
			}
		}
			
	}
}

void creeMatRobertsY( int **mat, int ***RobertsY, int h, int l)
{
	*RobertsY = alloueMatriceInt(h,l);
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			if(i==0 || i== h-1 || j==0 || j==l-1 )
			{
				(*RobertsY)[i][j] = mat[i][j];
			}
			else{
				(*RobertsY)[i][j] = mat[i+1][j] - mat[i][j];
			}
		}
			
	}
}


void creeMatRoberts ( int **mat,int ***Roberts, int ***RobertsX,int ***RobertsY, int h, int l)
{
	printf("\nMatrice filtre gradient Robert en cours...\n");
	*Roberts = alloueMatriceInt(h,l);
	
	creeMatRobertsX(mat, RobertsX, h, l);
	creeMatRobertsY(mat, RobertsY, h, l);
		
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			(*Roberts)[i][j] = sqrt( pow((*RobertsX)[i][j], 2) + pow((*RobertsY)[i][j], 2) );
		}
			
	}
	printf("Matrice filtre gradient Robert faite\n");
}


void creeMatPrewittX( int **mat, int ***PrewittX, int h, int l)
{
	*PrewittX = alloueMatriceInt(h,l);
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			if(i==0 || i== h-1 || j==0 || j==l-1 )
			{
				(*PrewittX)[i][j] = mat[i][j];
			}
			else{
				(*PrewittX)[i][j] =  mat[i+1][j+1] + mat[i][j+1]+mat[i-1][j+1] - (mat[i+1][j-1] + mat[i][j-1]+mat[i-1][j-1]);
			}
		}
	}
}

void creeMatPrewittY( int **mat, int ***PrewittY, int h, int l)
{
	*PrewittY = alloueMatriceInt(h,l);
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			if(i==0 || i== h-1 || j==0 || j==l-1 )
			{
				(*PrewittY)[i][j] = mat[i][j];
			}
			else{
				(*PrewittY)[i][j] =  mat[i-1][j-1] + mat[i-1][j] +mat[i-1][j+1] - (mat[i+1][j-1] + mat[i+1][j] +mat[i+1][j+1]);
			}
		}
			
	}
}


void creeMatPrewitt( int **mat,int ***Prewitt, int ***PrewittX,int ***PrewittY, int h, int l)
{
	printf("\nCréation matrice filtre gradient Prewitt en cours...\n");
	*Prewitt = alloueMatriceInt(h,l);
	
	creeMatPrewittX(mat, PrewittX, h, l);
	
	creeMatPrewittY(mat, PrewittY, h, l);

	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			(*Prewitt)[i][j] = sqrt( pow((*PrewittX)[i][j], 2) + pow((*PrewittY)[i][j], 2) );
			if( (*Prewitt)[i][j]  >255)
				(*Prewitt)[i][j] = 255;
			else if( (*Prewitt)[i][j] <0)
				(*Prewitt)[i][j] = 0;
		}		
	}
	printf("Matrice filtre gradient Prewitt faite\n");
}

void creeMatSobelX( int **mat, int ***SobelX, int h, int l)
{
	*SobelX = alloueMatriceInt(h,l);
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			if(i==0 || i== h-1 || j==0 || j==l-1 )
			{
				(*SobelX)[i][j] = mat[i][j];
			}
			else{
				(*SobelX)[i][j] =  mat[i+1][j+1] + 2*mat[i][j+1]+mat[i-1][j+1] - (mat[i+1][j-1] + 2*mat[i][j-1]+mat[i-1][j-1]);
			}
		}
	}
}

void creeMatSobelY( int **mat, int ***SobelY, int h, int l)
{
	*SobelY = alloueMatriceInt(h,l);
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			if(i==0 || i== h-1 || j==0 || j==l-1 )
			{
				(*SobelY)[i][j] = mat[i][j];
			}
			else{
				(*SobelY)[i][j] =  mat[i-1][j-1] + 2*mat[i-1][j] +mat[i-1][j+1] - (mat[i+1][j-1] + 2*mat[i+1][j] +mat[i+1][j+1]);
			}
		}
	}
}


void creeMatSobel( int **mat,int ***Sobel, int ***SobelX,int ***SobelY, int h, int l)
{
	printf("\nCréation matrice filtre gradient Sobel en cours...\n");
	*Sobel = alloueMatriceInt(h,l);
	
	creeMatSobelX(mat, SobelX, h, l);
	creeMatSobelY(mat, SobelY, h, l);
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			(*Sobel)[i][j] = sqrt( pow((*SobelX)[i][j], 2) + pow((*SobelY)[i][j], 2) );
			if( (*Sobel)[i][j]  >255)
				(*Sobel)[i][j] = 255;
			else if( (*Sobel)[i][j] <0)
				(*Sobel)[i][j] = 0;
		}
	}
	printf("Matrice filtre gradient Sobel faite\n");
}

//fonctions pour les transformées Hough et Radon-----------------------------------

void creeHough (int **mat, int ***matHough, int h, int l) //matHough a pour axes la distance du centre r(y) par l'angle (x)
{
	printf("\nCréation matrice transformée de Hough (détection des lignes) en cours...\n");
	int tetaMax = 180;
	double maxRho = sqrt( pow(h , 2) + pow(l, 2) ); // diagonale de l'image donc distance max de l'origine
	double rho ;/*= (int)maxRho++;*/
	int rho_absolu;
	int teta;
	double angleRad;
	
		
	*matHough = alloueMatriceInt((int)maxRho, (int)tetaMax); //la matrice fera la distance max du centre par l'angle max
	initMatInt(*matHough, maxRho, tetaMax); //maxRho et tetaMax sont castées en int
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			if(mat[i][j] == 255) //si on rencontre un pixel blanc..
			{
				for (teta = 0; teta < tetaMax; teta++)
				{
					angleRad = (M_PI*teta)/tetaMax;
					//printf("angleRad = %f\n", angleRad);
					rho = j * cos(angleRad) + i * sin(angleRad);
					
					if(rho > 0)
					{
						rho_absolu = (int)rho ;
						//printf("rho absolu: %d cosTeta:%f\n", rho_absolu,cos(teta));
						(*matHough)[rho_absolu][teta]++ ;
					}
				}
			}
		}
	}
	
	printf("matrice de la transformée de Hough faite\n");
}

void creeRadon (int **mat, int ****matRadon, int h, int l) //matHough a pour axes la distance du centre r(y) par l'angle (x)
{
	printf("\nCréation matrice transformée de Radon (détection des cercles) en cours...\n");
	double rMax = sqrt( pow(h , 2) + pow(l, 2) ); // diagonale de l'image donc distance max de l'origine
	int r;
	int xCentre, yCentre;
	int rCarre;
	
	int ***mRadon = (int ***)malloc(sizeof(int**) * (int)rMax);
	//printf("int rMax:%d\n", (int)rMax);
	for(r=0; r<(int)rMax; r++)
	{
		mRadon[r] = alloueMatriceInt(h, l); //la matrice fera la meme taille que l'image
		initMatInt(mRadon[r], h, l);
	}
	
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			if(mat[i][j] == 255) //si on rencontre un pixel blanc..
			{
				for (yCentre = 0; yCentre < h; yCentre++)
				{
					for (xCentre = 0; xCentre < l ; xCentre++)
					{
						rCarre = pow(j-xCentre, 2) + pow(i-yCentre, 2);
						r = (int)sqrt(rCarre);
						//printf("r:%d\n", r);
						
						mRadon[r][yCentre][xCentre]++ ;
					}			
				}
			}
		}
	}
	*matRadon = mRadon;
	printf("matrice de la transformée de Radon faite\n");

}

//fonctions pour seuillage automatique------------------------------------------
	
float compute_zero_order_cumulative_moment(float *hist, int k)
{
	float zero_order_cumulative_moment = 0;
	for (int i = 0; i < k; i++)
	{
		zero_order_cumulative_moment += hist[i];
	}
	return zero_order_cumulative_moment;
}

float compute_first_order_cumulative_moment(float *hist, int k)
{
	float first_order_cumulative_moment = 0;

	for (int i = 0; i < k; i++)
	{
		first_order_cumulative_moment += i*hist[i];
	}
	return first_order_cumulative_moment;
}
float compute_variance_class_separability(float uT,float wk, float uk)
{
	return pow((uT*wk-uk),2)/(wk*(1-wk));
}

int otsu( int h, int l, float hist[256] )
{
	printf("Seuillage automatique méthode d'Otsu en cours...\n");

 // Normalisation de l'histogramme des niveau de gris
	int N = h*l;
	//printf("N=%d\n",N);
	
	for (int i = 0; i < 256; i++)
	{	
		hist[i] = hist[i] / N;
	}
	float w[256],u[256],uT;

	for (int i = 0; i < 256; i++)
	{
		w[i] = compute_zero_order_cumulative_moment(hist, i);
		u[i] = compute_first_order_cumulative_moment(hist, i);
	}
	uT = compute_first_order_cumulative_moment(hist, 256);

	float variance_class_separability_max = -1;
	float best_threesold = 0;
	for (int i = 0; i < 256; i++)
	{
		int vk = compute_variance_class_separability(uT, w[i], u[i]);

		if (vk > variance_class_separability_max)
		{
			variance_class_separability_max = vk;
			best_threesold = i;
		}
	}
	
	printf("seuil auto: %d\n", (int)best_threesold);
	return (int)best_threesold;
}	
	


//fonctions pour recréer droites et cercles--------------------------------

void initMatInv(int **mat, int h, int l)
{
	for(int i=0; i<h; i++)
	{
		for(int j=0; j<l ; j++)
		{
			mat[i][j] = 255;
		}
	}
}
void recreeDroite(int ***matNew , int **matHough,int h,int l )
{
	printf("\nCréation de la matrice des droites à partir de la transformée de Hough...\n");
	int tetaMax = 180;
	double maxRho = sqrt( pow(h , 2) + pow(l, 2) ); // diagonale de l'image donc distance max de l'origine
	int rho ;
	int teta;
	int nombreDroiteMax=300;
	int **temponSol;
	int tempon=0;
	temponSol = alloueMatriceInt(4,nombreDroiteMax);
	initMatInt(temponSol, 4, nombreDroiteMax);
	double test;
	int **mat = alloueMatriceInt(h, l); //la matrice fera la distance max du centre par l'angle max
	initMatInv(mat, h, l);
	
		for( rho=0; rho<(int)maxRho ; rho++)
		{
			
				for (teta = 0; teta < tetaMax; teta++)
				{
					if(matHough[rho][teta] > 160){
						
						temponSol[0][tempon]=matHough[rho][teta] ;
						temponSol[1][tempon]= (int) rho;
						temponSol[2][tempon]=teta;
						tempon++;
						temponSol[3][0]=tempon;
					}
				}
			
		}
		
	for(int t=0; t<temponSol[3][0];t++){
		for(int i=0; i<h; i++)      //i=x ?
		{
			for(int j=0; j<l ; j++)
			{	//test du pixel blanc
				test=i*(-cos(temponSol[2][t])/sin(temponSol[2][t]))+(temponSol[1][t]/sin(temponSol[2][t]));   //j=y ?
				if(j+0.1<=test && test<=j+0.1){
					if( *matNew[i][j]==1)
						mat[i][j]=0;
				}
			}
		}
	}
	/*for(int t=0; t<*temponSol[3][0];t++){
		if(teta!=0){
			ligne(0,(rho/sin(teta)),100*(-cos(teta)/sin(teta)),100+(rho/sin(teta)));
		}
		else
			ligne(0,rho,0,rho);
	}*/
	*matNew = mat;
	printf("Matrice des droites créée\n");

}

void recreeCercle(int ***matNew , int ***matRadon,int h,int l ){
	printf("\nCréation de la matrice des cercles à partir de la transformée de Radon...\n");
	int **temponSol;
	int tempon=0;
	int nombreCercleMax=100;
	temponSol = alloueMatriceInt(5,nombreCercleMax);
	initMatInt(temponSol, 5, nombreCercleMax);
	int test;
	int **mat = alloueMatriceInt(h, l); //la matrice fera la distance max du centre par l'angle max
	initMatInv(mat, h, l);
	double Rmax = sqrt( pow(h , 2) + pow(l, 2) );
	for(int r=0;r<(int)Rmax;r++)
	{
		for( int yCentre = 0; yCentre < h; yCentre++)
		{
			
				for (int xCentre = 0; xCentre < l ; xCentre++)
				{
					if(matRadon[r][yCentre][xCentre] > 150){
						
						temponSol[0][tempon]=matRadon[r][yCentre][xCentre];
						temponSol[1][tempon]= r;
						temponSol[2][tempon]=yCentre;
						temponSol[3][tempon]=xCentre;
						tempon++;
						temponSol[4][0]=tempon;
					}
				}
			
		}
	}
	for(int t=0; t<temponSol[3][0];t++){
		for(int i=0; i<h; i++)      //i=x ?
		{
			for(int j=0; j<l ; j++)
			{
				test=pow((i-temponSol[3][tempon]),2)+pow((j-temponSol[2][tempon]),2);   //j=y ?
				if(pow(test,2)==pow(temponSol[1][tempon],2))
					mat[i][j]=0;
			}
		}
	}
	*matNew = mat;
	printf("Matrice des cercles créée\n");
}

