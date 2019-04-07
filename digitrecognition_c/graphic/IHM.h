void chargeImagesIHM(DonneesImageRGB **retour,DonneesImageRGB **home,DonneesImageRGB **croix,DonneesImageRGB **logo);
void libereImagesIHM(DonneesImageRGB **retour,DonneesImageRGB **home,DonneesImageRGB **croix,DonneesImageRGB **logo);


void monIHM(zone zQuit, zone zHome, zone zRetour, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, DonneesImageRGB *logo, int numpage);
void afficheTitre(zone zTitre); //affiche chaine en grand dans la zone titre
void afficheImage(zone zITest, DonneesImageRGB *image);//affiche image dans zone

void initZones(zone *zQuit, zone *zHome, zone *zRetour, zone *zTitre, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix, char *titre);
void redimensionneZones(zone *zQuit, zone *zHome, zone *zRetour, zone *zTitre, DonneesImageRGB *retour, DonneesImageRGB *home, DonneesImageRGB *croix);
void initGraph(zone *zGraph);
void redimmensionneGraph(zone *zGraph);

void afficheAxesGraph(zone zGraph);

void affichePointFonction(zone zGraph );
void affichePointDansGraph(int nb_boucle, int erreurs_modeles, zone zGraph);
void afficheTousLesPoints(int *erreurs, int nb_boucles, zone zGraph);

