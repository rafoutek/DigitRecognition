
//fonctions gestion matrices

int ** alloueMatriceInt(int h, int l);
float ** alloueMatriceFloat(int h, int l);

void libereMatriceInt(int h, int l,int ** m);
void libereMatriceFloat(int h, int l,float ** m);

void initMatInt(int **mat, int h, int l);
void initMatFloat(float **mat, int h, int l);

void afficheMatInt(int **mat, int h, int l);
void afficheMatFloat(float **mat, int h, int l); //peut servir pour debug


void allocation4MatricesInt(DonneesImageRGB * image, int *** b, int *** v, int *** r, int ***s);

void cree3MatricesFloat(DonneesImageRGB * image, float *** bleu, float *** vert, float *** rouge);
void cree3MatricesInt(DonneesImageRGB * image, int *** bleu, int *** vert, int *** rouge);

DonneesImageRGB *creeImage(int h, int l, int **r, int **v, int **b);

void creeMatNG(DonneesImageRGB * image, int ** b, int ** v, int ** r, int ***s);
void creeMatNG_V2(int h, int l, int ** b, int ** v, int ** r, int ***s);
void remplaceValeurs_par_dansMatriceInt_deTaille_(int val1, int val2, int ***mat,int h,int l);
void initTab(float tab[256] , int taille);

void creeHistogramme (int h, int l, int **s, float histogramme [256]);
void afficheTab(float *tab, int taille);

int maxTab (float tab[256], int taille);
void tabRatio (float histogramme[256]);
void creeMatHistogramme (float histogramme[256], int ***hist);
float compare_histogramme (float histogramme1[256], float histogramme2[256]);

void seuillage(int h, int l, int ***s, int seuil);
void seuillage_V2(int h, int l, int seuil, int **g, int ***nb);
float compute_zero_order_cumulative_moment(float *hist, int k);
float compute_first_order_cumulative_moment(float *hist, int k);
float compute_variance_class_separability(float uT,float wk, float uk);
int otsu(int h, int l, float hist[256] );

void tricroissant_tabmediane(int tab[8]);
void filtreMedian3x3( int **matrice, int ***mat_mediane, DonneesImageRGB *image);
void creeMatMedian(int **matrice, int ***mat_mediane, DonneesImageRGB *image);

void creeMatRobertsX( int **mat, int ***RobertsX, int h, int l);
void creeMatRobertsY( int **mat, int ***RobertsY, int h, int l);
void creeMatRoberts ( int **mat,int ***Roberts, int ***RobertsX,int ***RobertsY, int h, int l);

void creeMatPrewittX( int **mat, int ***PrewittX, int h, int l);
void creeMatPrewittY( int **mat, int ***PrewittY, int h, int l);
void creeMatPrewitt( int **mat,int ***Prewitt, int ***PrewittX,int ***PrewittY, int h, int l);

void creeMatSobelX( int **mat, int ***SobelX, int h, int l);
void creeMatSobelY( int **mat, int ***SobelY, int h, int l);
void creeMatSobel( int **mat,int ***Sobel, int ***SobelX,int ***SobelY, int h, int l);



//void initMat(int **mat, int h, int l);
void creeHough (int **mat, int ***matHough, int h, int l);
void creeRadon (int **mat, int ****matRadon, int h, int l);

void initMatInv(int **mat, int h, int l);
void recreeDroite(int ***matNew , int **matHough,int h,int l );
void recreeCercle(int ***matNew , int ***matRadon,int h,int l );

