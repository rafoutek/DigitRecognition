MODELE_COMPLET init_modeleComplet(int nb_modeles, int nb_entrees, int nb_sorties);
void remplit_modeleComplet_XOR(MODELE_COMPLET *modeleComplet);

DonneesImageRGB* lit_imageModele(int chiffre, int num);
void determine_sortieModeleAttendue(int chiffre, MODELE modele);
void remplit_modele_depuis_image(DonneesImageRGB *image, MODELE *modele);
void test_image_to_model(MODELE *modele);
void recopie_EntreesModele_dansEntreesReseau(MODELE modele, RESEAU *reseau);
void conversionValeursMatriceInt_deTaille_etRemplissageEntreesModele(int **mat, int h, int l, MODELE *modele);
void affiche_entrees_reseau(RESEAU reseau);

RESEAU init_reseau(MODELE_COMPLET modeleComplet);


void init_poids_alea_modeleComplet(MODELE_COMPLET *modeleComplet);
void init_poids_alea_Reseau(RESEAU *reseau);
void init_biais_Reseau(RESEAU *reseau);

void affiche_modele (MODELE modele);
void affiche_modele_complet(MODELE_COMPLET modeleComplet);
void affiche_reseau(RESEAU reseau);

double somme_neurone(PERCEPTRON perceptron);
double fonction_transfert_sigmoide(double somme);
int fonction_transfert_seuil(double somme);
void propagation_avant_selon_modele (RESEAU *reseau, double *sorties_attendues);
void propagation_avant_selon_entrees (RESEAU *reseau, int entree0, int entree1);
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



