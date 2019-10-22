

typedef struct{
char nom_meilleures_scores[6][55];
int utilise_50[6];
int utilise_25[6];
int utilise_s[6];
int meilleure_scores[6];
int nbr_reponse_juste[15];

}statistiques;


void ecran_menu_jeu();
void ecran_jeu();
void randomizer(int *t);
void decom(char *question ,char *str);
void init_question();
void button_a_clique();
void button_b_clique();
void button_c_clique();
void button_d_clique();
void abandonner();
void butt_25_clique();
void butt_50_clique();
void game_over();
void switch_clique();
void enregistrer_statistiques(int score,int u_50,int u_25,int u_s);
void ecran_stats_globale();
void quitter_ecran_stats_gb();
void quitter_ecran_stats_j();
void ecran_stats_joueur();
void ecran_champion();
void quitter_ecran_champion();
void supprimer_compte();
void quitter_menu_jeu();
