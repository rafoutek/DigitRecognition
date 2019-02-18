void init_poids_alea_modeleComplet(MODELE_COMPLET *modeleComplet);
void init_poids_alea_Reseau(RESEAU *reseau);
void init_biais_Reseau(RESEAU *reseau);

void affiche_modele (MODELE modele);
void affiche_modele_complet(MODELE_COMPLET modeleComplet);
void affiche_reseau(RESEAU reseau);

double somme_neurone(PERCEPTRON perceptron);
double fonction_transfert_sigmoide(double somme);
int fonction_transfert_seuil(double somme);
void propagation_avant_selon_modele (RESEAU *reseau, MODELE modele);
void propagation_avant_selon_entrees (RESEAU *reseau, int entree0, int entree1);
int rand_a_b(int a, int b);
int choix_modele(void);



double erreur_globale_couche_finale( PERCEPTRON perceptron_final, double sortie_attendue);
double erreur_locale_couche_finale( PERCEPTRON perceptron_final, double erreur_globale);
void retropropagation (RESEAU *reseau);


bool erreur_insignifiante ( double erreur_globale);
bool erreurs_reseau_insignifiantes (RESEAU reseau);

void enregistrement_biais_et_poids_reseau(RESEAU reseau);
void recupere_biais_et_poids_enregistres (RESEAU *reseau);

int menu (void);
int get_entree(void);



