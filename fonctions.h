MODELE_COMPLET init_modeleComplet(int nb_modeles, int nb_entrees, int nb_sorties);
MODELE init_modele(int nb_entrees, int nb_sorties);

void remplit_imageModele(DonneesImageRGB *img, int chiffre, int num, char *chemin_image);
DonneesImageRGB *lis_imageModele(int chiffre, int num, char *chemin_image);
void determine_sortieModeleAttendue(int chiffre, MODELE *modele);
void remplit_modele_depuis_image(DonneesImageRGB *image, int ***r, int ***v, int ***b, int ***g, MODELE *modele);
void recopie_EntreesModele_dansEntreesReseau(MODELE modele, RESEAU *reseau);
void conversionValeursMatriceInt_deTaille_etRemplissageEntreesModele(int **mat, int h, int l, MODELE *modele);
void affiche_entrees_reseau(RESEAU reseau);

RESEAU init_reseau(MODELE modele);


void init_poids_alea_modeleComplet(MODELE_COMPLET *modeleComplet);
void init_poids_alea_Reseau(RESEAU *reseau);
void init_biais_Reseau(RESEAU *reseau);

void affiche_modele (MODELE modele);
void affiche_modele_complet(MODELE_COMPLET modeleComplet);
void affiche_reseau(RESEAU reseau);

double somme_neurone(PERCEPTRON perceptron);
void somme_neurone_V2(PERCEPTRON perceptron, double *sum);
double fonction_transfert_sigmoide(double somme);
int fonction_transfert_seuil(double somme);
void propagation_avant_selon_modele (RESEAU *reseau, double *sorties_attendues);

char* demande_utilisateur_image_a_test(void);
DonneesImageRGB* demande_et_lit_image_test(void);
void propagation_avant (RESEAU *reseau);
void cree_et_affiche_classement_ressemblance(RESEAU reseau);
int rand_a_b(int a, int b);
int choix_modele(void);



double erreur_globale_couche_finale( PERCEPTRON perceptron_final, double sortie_attendue);
double erreur_locale_couche_finale( PERCEPTRON perceptron_final, double erreur_globale);
void enregistre_erreur_locale_dans_entreesPerceptron (PERCEPTRON *perceptron);
void calcul_et_enregistre_erreurLocalePerceptron (PERCEPTRON *perceptron, int num_perceptron);
void retropropagation (RESEAU *reseau);


bool erreur_insignifiante ( double erreur_globale);
bool erreurs_reseau_insignifiantes (RESEAU reseau);

void enregistrement_biais_et_poids_reseau(RESEAU reseau);
void recupere_biais_et_poids_enregistres (RESEAU *reseau);

int menu (void);
int get_entree(void);



