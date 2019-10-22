

typedef enum {true,false} bool;
typedef struct {
     char nom[55];
     char mot_passe[15];
     int meilleur_score;
     int somme_score;
     int nbr_partie_jouee;
}joueur;

typedef struct {
     char nom[55];
     char mot_passe[15];
}administrateur;

typedef struct {
     char question[150];
     char reponses[4][30];
     int reponse_correcte;
     int niveau;
     int reponse_proche;
}QUESTION;

typedef struct _LIST_JOUEURS {
     joueur element ;
    struct _LIST_JOUEURS * suivant;
}LIST_JOUEURS;

typedef struct _LIST_QUESTIONS {
    QUESTION element ;
    struct _LIST_QUESTIONS * suivant;
}LIST_QUESTIONS;

int rechercher(char *chaine,char *mot_cle);

joueur affecter_joueur(char * nom,char * mot_passe,int meilleur_score,int somme_score,int nbr_partie_jouee);

QUESTION affecter_question(char *question,char *reponse1,char *reponse2,char *reponse3,char *reponse4,int reponse_correcte,int niveau,int reponse_proche);

QUESTION copier_question(QUESTION q1);

LIST_JOUEURS * inserer_fin(joueur element,LIST_JOUEURS *l);

LIST_JOUEURS * supprimer(char *nom,LIST_JOUEURS *l);

LIST_JOUEURS * vider(LIST_JOUEURS *l);

LIST_QUESTIONS * inserer_fin_q(QUESTION element,LIST_QUESTIONS *l);

LIST_QUESTIONS * supprimer_q(char *e,LIST_QUESTIONS *l);

LIST_QUESTIONS * vider_q(LIST_QUESTIONS *l);


