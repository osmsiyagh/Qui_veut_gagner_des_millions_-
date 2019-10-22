#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "jeu.h"
#include "joueur_question.h"
#include "util.h"
#include "administration.h"

extern joueur j_courant;
extern GtkWidget *fenetre_accueil;
extern LIST_JOUEURS *list_j;

// les elements de la fenetre menu du jeu
GtkWidget *butt_nouv_jeu,*butt_statistique_joueur,*butt_stat_global,*butt_supprimer,*butt_quitter;
GtkWidget *fenetre_menu_jeu,*cadre_menu_jeu;

// les elements de la fenetre du jeu
GtkWidget *fenetre_jeu,*cadre_jeu,*image_principale,*butt_rep[4],*label_rep[4],*label_ques,*butt_50,*butt_25,*butt_s,*butt_ab;
GtkWidget *image_50,*image_25,*image_s,*image_ab;
GtkWidget *image_non_50,*image_non_25,*image_non_s;
bool utilise_50,utilise_25,utilise_s;
int u_50,u_25,u_s;
int rand_rep[4],taille[15],indice[15],q_aleatoire,niveau;
int prix[16]={0,200,300,500,800,1500,3000,6000,12000,24000,48000,72000,100000,150000,300000,1000000};
const char alp[4]="ABCD";
int nbr_reponse_juste[15];
char text_span_1[100]="<span foreground=\"white\" >",text_span_2[30]="</span>";
int affiche_rep[4];
QUESTION **mat_question;
QUESTION q;
FILE *fp;
int cpt;

// les elements de la fenetre des statistiques globales
GtkWidget *fenetre_stat_gb,*cadre_stat_gb,*label_nom_m[6],*label_score[6],*image_joker[3][6],*label_nbr_p[15],*label_jr_e;
GtkWidget *butt_ok_s,*image_st;

// les elements de la fenetre des statistiques du joueur ;
GtkWidget *fenetre_stat_j,*cadre_stat_j,*label_m_score,*label_somme_score,*label_nbr_pa;
GtkWidget *butt_ok_sj,*image_st_j;

// les elements de la fenetre champion

GtkWidget * fenetre_champion,*cadre_champion,*butt_ok_c,*image_champion;



void game_over()
{
GtkWidget *pQuestion;
joueur j;
if(prix[niveau-1]<200)
{

    msgbox1(fenetre_jeu,"tu as perdu! ");
    enregistrer_statistiques(0,u_50,u_25,u_s);

}
else if(prix[niveau-1]<1500)
{

    msgbox1(fenetre_jeu,"tu as perdu! vous avez gagne 200DH");
    enregistrer_statistiques(200,u_50,u_25,u_s);

}
else if(prix[niveau-1]<48000)
{
    msgbox1(fenetre_jeu,"tu as perdu! vous avez gagne 1500DH");
    enregistrer_statistiques(1500,u_50,u_25,u_s);

}
else if((prix[niveau-1])<1000000)
{
    msgbox1(fenetre_jeu,"tu as perdu! vous avez gagne 48000DH");
    enregistrer_statistiques(48000,u_50,u_25,u_s);

}
gtk_widget_destroy(fenetre_jeu);


}
void switch_clique()
{
char str[400],exp[450];
int q_rand;
const char alp[4]="ABCD";
int i;
utilise_50=false;
utilise_25=false;
utilise_s=true;
u_s=0;
gtk_widget_hide(image_s);
gtk_widget_hide(butt_s);
gtk_widget_show(image_non_s);
msgbox1(fenetre_jeu,"Vous avez utilis� le joker SWITCH");
randomizer(rand_rep);
do{
q_rand=(int )rand()%(taille[niveau-1]);
}while(q_rand==q_aleatoire);
q_aleatoire=q_rand;
q=copier_question(mat_question[niveau-1][q_rand]);
decom(q.question,str);
sprintf(exp,"%s%s%s",text_span_1,str,text_span_2);
gtk_label_set_text(GTK_LABEL(label_ques),exp);
gtk_label_set_use_markup(GTK_LABEL(label_ques), TRUE);
for(i=0;i<4;i++)
{
sprintf(exp,"%s%c  :%s%s",text_span_1,alp[i],q.reponses[rand_rep[i]],text_span_2);
gtk_label_set_text(GTK_LABEL(label_rep[i]),exp);
gtk_label_set_use_markup(GTK_LABEL(label_rep[i]), TRUE);
}
for(i=0;i<4;i++)
{
gtk_widget_show(label_rep[i]);
gtk_widget_show(butt_rep[i]);
}
}
void butt_50_clique()
{
    utilise_50=true;
    u_50=0;
    GtkWidget *pQuestion;
    int i;
    gtk_widget_hide(image_50);
    gtk_widget_hide(butt_50);
    gtk_widget_show(image_non_50);
    msgbox1(fenetre_jeu,"Vous avez utilis� le joker 50");
    if(utilise_25==false)
    {
    for(i=0;i<4;i++)
    {
        if((rand_rep[i]!=q.reponse_correcte)&&(rand_rep[i]!=q.reponse_proche))
        {
             gtk_widget_hide(label_rep[i]);
             gtk_widget_hide(butt_rep[i]);
             affiche_rep[i]=0;
        }
    }
    }
    else
    {
     for(i=0;i<4;i++)
    {
        if(rand_rep[i]!=q.reponse_correcte)
        {
             gtk_widget_hide(label_rep[i]);
             gtk_widget_hide(butt_rep[i]);
             affiche_rep[i]=0;
        }
    }
    }
}
void butt_25_clique()
{

    int ra,c;
    utilise_25=true;
    u_25=0;

    int i;
    gtk_widget_hide(image_25);
    gtk_widget_hide(butt_25);
    gtk_widget_show(image_non_25);
    msgbox1(fenetre_jeu,"Vous avez utilis� le joker 25");
    if(utilise_50==true)
    {
        for(i=0;i<4;i++)
        {
            if(rand_rep[i]==q.reponse_proche)
            {
                gtk_widget_hide(label_rep[i]);
                gtk_widget_hide(butt_rep[i]);
                affiche_rep[i]=0;
                i=4;
            }
        }
    }
    else
    {
    ra=(int)rand()%2;
    c=0;
    for(i=0;i<4;i++)
    {
        if(((rand_rep[i])!=q.reponse_correcte)&&(rand_rep[i]!=q.reponse_proche)&&(c==ra))
        {
            gtk_widget_hide(label_rep[i]);
            gtk_widget_hide(butt_rep[i]);
            affiche_rep[i]=0;
            i=4;

        }
        else if((rand_rep[i]!=q.reponse_correcte)&&(rand_rep[i]!=q.reponse_proche))
        {
            c++;
        }
    }
    }
}
void abandonner()
{
    char str[100];
    if(prix[niveau-1]<200)
    {

        sprintf(str,"Vous avez abandonne !");
        msgbox1(fenetre_jeu,str);
        gtk_widget_destroy(fenetre_jeu);
        enregistrer_statistiques(prix[niveau-1],u_50,u_25,u_s);

    }
    else if(prix[niveau-1]<1500)
    {

        sprintf(str,"Vous avez abandonne ,vous gagner 200 DH");
        msgbox1(fenetre_jeu,str);
        gtk_widget_destroy(fenetre_jeu);
        enregistrer_statistiques(prix[niveau-1],u_50,u_25,u_s);

    }
    else if(prix[niveau-1]<48000)
    {
        sprintf(str,"Vous avez abandonne ,vous gagner 1500 DH");
        msgbox1(fenetre_jeu,str);
        gtk_widget_destroy(fenetre_jeu);
        enregistrer_statistiques(prix[niveau-1],u_50,u_25,u_s);
    }
    else if((prix[niveau-1])<1000000)
    {
        sprintf(str,"Vous avez abandonne ,vous gagner 48000 DH");
        msgbox1(fenetre_jeu,str);
        gtk_widget_destroy(fenetre_jeu);
        enregistrer_statistiques(prix[niveau-1],u_50,u_25,u_s);
        //supprimer_sauvegarde();
    }


}

void randomizer(int *t)
{
int cpt=4,temp,i,ran;
for(i=0;i<4;i++)
{
t[i]=i;
}
cpt=4;
    for(i=0;i<cpt;i++)
    {
       ran=(int )rand()%cpt;
       temp=t[ran];
       t[ran]=t[cpt-1];
       t[cpt-1]=temp;
       cpt--;
    }
}
void button_a_clique()
{
    GtkWidget *pQuestion;
    char exp[300];
    int i,ind_rech;
    if(q.reponse_correcte==rand_rep[0])
    {
     msgbox1(fenetre_jeu,"Reponse correcte");
     nbr_reponse_juste[niveau-1]++;
    if(niveau==15)
    {
        ecran_champion();
        gtk_widget_destroy(fenetre_jeu);
        enregistrer_statistiques(1000000,u_50,u_25,u_s);
    }
    else
    {

      init_question();
    }

    }
    else
    {
        for(i=0;i<4;i++)
        {
          if(rand_rep[i]==q.reponse_correcte) ind_rech=i,i=4;
        }
        sprintf(exp,"Fausse reponse ! La bonne reponse est  %c :%s",alp[ind_rech],q.reponses[q.reponse_correcte]);
        msgbox(fenetre_jeu,exp);
        game_over();
    }
}
void button_b_clique()
{
    GtkWidget *pQuestion;
    char exp[300];
    int i,ind_rech;
    if(q.reponse_correcte==rand_rep[1])
    {
    msgbox1(fenetre_jeu,"Reponse correcte");
    nbr_reponse_juste[niveau-1]++;
    if(niveau==15)
    {
        ecran_champion();
        gtk_widget_destroy(fenetre_jeu);
        enregistrer_statistiques(1000000,u_50,u_25,u_s);
    }
    else
    {

      init_question();
    }

    }
    else
    {
        for(i=0;i<4;i++)
        {
          if(rand_rep[i]==q.reponse_correcte) ind_rech=i,i=4;
        }
        sprintf(exp,"Fausse reponse ! La bonne reponse est  %c :%s ",alp[ind_rech],q.reponses[q.reponse_correcte]);
        msgbox(fenetre_jeu,exp);
        game_over();
    }
}
void button_c_clique()
{
    GtkWidget *pQuestion;
    char exp[300];
    int i,ind_rech;
    if(q.reponse_correcte==rand_rep[2])
    {
    msgbox1(fenetre_jeu,"Reponse correcte");
     nbr_reponse_juste[niveau-1]++;
    if(niveau==15)
    {
        ecran_champion();
        gtk_widget_destroy(fenetre_jeu);
        enregistrer_statistiques(1000000,u_50,u_25,u_s);
    }
    else
    {

      init_question();
    }

    }
    else
    {
        for(i=0;i<4;i++)
        {
          if(rand_rep[i]==q.reponse_correcte) ind_rech=i,i=4;
        }
        sprintf(exp,"Fausse reponse ! La bonne reponse est  %c :%s ",alp[ind_rech],q.reponses[q.reponse_correcte]);
        msgbox(fenetre_jeu,exp);
        game_over();
    }
}
void button_d_clique()
{
    GtkWidget *pQuestion;
    char exp[300];
    int i,ind_rech;
    if(q.reponse_correcte==rand_rep[3])
    {
    msgbox(fenetre_jeu,"Reponse correcte");
    nbr_reponse_juste[niveau-1]++;
    if(niveau==15)
    {
        ecran_champion();
        gtk_widget_destroy(fenetre_jeu);
        enregistrer_statistiques(1000000,u_50,u_25,u_s);
    }
    else
    {

      init_question();
    }

    }
    else
    {
        for(i=0;i<4;i++)
        {
          if(rand_rep[i]==q.reponse_correcte) ind_rech=i,i=4;
        }
        sprintf(exp,"Desole reponse fausse la reponse correcte est  %c :%s ",alp[ind_rech],q.reponses[q.reponse_correcte]);
        msgbox(fenetre_jeu,exp);
        game_over();
    }
}
void decom(char *question ,char *str)
{
    int i=0,cpt=65,k=0;

    while(i<3)
    {
          while(question[cpt]!=' ')
           {
               cpt--;
           }
           while(k<cpt)
           {
               str[k]=question[k];
               k++;
           }
           str[k]='\n';k++;
           cpt=cpt+65;
           i++;
    }
    str[k]='\0';
}
void init_question()
{
char str[400],exp[450];
const char alp[4]="ABCD";
int i;
utilise_50=false;
utilise_25=false;
utilise_s=false;
randomizer(rand_rep);
niveau++;
q_aleatoire=(int )rand()%(taille[niveau-1]);
q=copier_question(mat_question[niveau-1][q_aleatoire]);

decom(q.question,str);
sprintf(exp,"%s%s%s",text_span_1,str,text_span_2);
gtk_label_set_text(GTK_LABEL(label_ques),exp);
gtk_label_set_use_markup(GTK_LABEL(label_ques), TRUE);

for(i=0;i<4;i++)
{
sprintf(exp,"%s%c  :%s%s",text_span_1,alp[i],q.reponses[rand_rep[i]],text_span_2);
gtk_label_set_text(GTK_LABEL(label_rep[i]),exp);
gtk_label_set_use_markup(GTK_LABEL(label_rep[i]), TRUE);

}
for(i=0;i<4;i++)
{
gtk_widget_show(label_rep[i]);
gtk_widget_show(butt_rep[i]);
affiche_rep[i]=1;
}
}



void ecran_jeu()
{
char exp[300];
int rep=0;
strcpy(exp,"saves\\");
strcat(exp,j_courant.nom);
strcat(exp,".bin");
fp=fopen(exp,"rb");
if(fp!=NULL)
{
rep=msgbox2(fenetre_menu_jeu,"Vous avez d�ja une partie sauvegard� si vous voulez un nouveau jeu vous supprimerer la partie sauvegard�e ");
fclose(fp);
}
if((fp==NULL)||(rep==1))
{
srand(time(NULL));
fclose(fp);
remove(exp);
int i;
for(i=0;i<15;i++)
{
nbr_reponse_juste[i]=0;
}
for(i=0;i<4;i++)
{
affiche_rep[i]=1;
}
//FMOD_System_PlaySound(s, FMOD_CHANNEL_FREE, musique, 0, NULL);
u_50=1;
u_25=1;
u_s=1;
cadre_jeu=gtk_fixed_new();
fenetre_jeu=gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_modal(GTK_WINDOW(fenetre_jeu),TRUE);
gtk_window_set_decorated(GTK_WINDOW(fenetre_jeu),FALSE);
gtk_container_add(GTK_CONTAINER(fenetre_jeu),cadre_jeu);
image_principale=gtk_image_new_from_file("background2.jpg");
image_50=gtk_image_new_from_file("r_50.png");
image_25=gtk_image_new_from_file("r_25.png");
image_s=gtk_image_new_from_file("r_s.png");
image_non_50=gtk_image_new_from_file("non_r_50.png");
image_non_25=gtk_image_new_from_file("non_r_25.png");
image_non_s=gtk_image_new_from_file("non_r_s.png");
image_ab=gtk_image_new_from_file("sortir.png");
//image_cercle=gtk_image_new_from_file("cercle.png");
butt_50=gtk_button_new();
butt_25=gtk_button_new();
butt_s=gtk_button_new();
butt_ab=gtk_button_new();
//butt_sauvegarder=gtk_button_new_with_label("Enregistrer la partie ");
for(i=0;i<4;i++)
{

label_rep[i]=gtk_label_new("");
butt_rep[i]=gtk_button_new();
}

label_ques=gtk_label_new("");
gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_50,219,34);
gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_25,219,94);
gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_s,219,154);
gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_ab,700,154);
//gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_sauvegarder,250,7);
gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_rep[0],207,386);
gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_rep[1],512,386);
gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_rep[2],207,490);
gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_rep[3],512,490);
gtk_fixed_put(GTK_FIXED(cadre_jeu),image_principale,0,0);
gtk_fixed_put(GTK_FIXED(cadre_jeu),label_rep[0],207,386);
gtk_fixed_put(GTK_FIXED(cadre_jeu),label_rep[1],512,386);
gtk_fixed_put(GTK_FIXED(cadre_jeu),label_rep[2],207,490);
gtk_fixed_put(GTK_FIXED(cadre_jeu),label_rep[3],512,490);
gtk_fixed_put(GTK_FIXED(cadre_jeu),label_ques,207,250);
gtk_fixed_put(GTK_FIXED(cadre_jeu),image_50,219,34);
gtk_fixed_put(GTK_FIXED(cadre_jeu),image_25,219,94);
gtk_fixed_put(GTK_FIXED(cadre_jeu),image_s,219,154);
gtk_fixed_put(GTK_FIXED(cadre_jeu),image_non_50,219,34);
gtk_fixed_put(GTK_FIXED(cadre_jeu),image_non_25,219,94);
gtk_fixed_put(GTK_FIXED(cadre_jeu),image_non_s,219,154);
gtk_fixed_put(GTK_FIXED(cadre_jeu),image_ab,700,154);
//gtk_fixed_put(GTK_FIXED(cadre_jeu),butt_sauvegarder,620,7);
for(i=0;i<4;i++)
{
gtk_widget_set_size_request(butt_rep[i],260,60);
gtk_label_set_use_markup(GTK_LABEL(label_rep[i]), TRUE);
}

gtk_label_set_use_markup(GTK_LABEL(label_ques), TRUE);

gtk_widget_set_size_request(butt_50,52,37);
gtk_widget_set_size_request(butt_25,52,37);
gtk_widget_set_size_request(butt_s,52,37);
gtk_widget_set_size_request(butt_ab,52,37);
gtk_window_set_modal(GTK_WINDOW(fenetre_menu_jeu),FALSE);

gtk_window_set_position(GTK_WINDOW(fenetre_jeu),GTK_WIN_POS_CENTER);
//g_signal_connect(G_OBJECT(fenetre_jeu),"destroy", G_CALLBACK(stopper_music), NULL);
niveau=0;
QUESTION q;
fp=fopen("questions.bin","rb");
for(i=0;i<15;i++)
{
taille[i]=0;
indice[i]=0;
}
while(fread(&q,sizeof(QUESTION),1,fp)!=0)
{
   taille[q.niveau-1]++;
}
mat_question=(QUESTION**)malloc(15*sizeof(QUESTION*));
for(i=0;i<15;i++)
{
if(taille[i]!=0) mat_question[i]=(QUESTION *)malloc(taille[i]*sizeof(QUESTION));
}
rewind(fp);
while(fread(&q,sizeof(QUESTION),1,fp)!=0)
{
mat_question[(q.niveau)-1][indice[(q.niveau)-1]]=copier_question(q);
indice[(q.niveau)-1]++;

}
fclose(fp);
init_question();
g_signal_connect(G_OBJECT(butt_rep[0]),"pressed", G_CALLBACK(button_a_clique),NULL);
g_signal_connect(G_OBJECT(butt_rep[1]),"pressed", G_CALLBACK(button_b_clique),NULL);
g_signal_connect(G_OBJECT(butt_rep[2]),"pressed", G_CALLBACK(button_c_clique),NULL);
g_signal_connect(G_OBJECT(butt_rep[3]),"pressed", G_CALLBACK(button_d_clique),NULL);
g_signal_connect(G_OBJECT(butt_50),"pressed", G_CALLBACK(butt_50_clique),NULL);
g_signal_connect(G_OBJECT(butt_25),"pressed", G_CALLBACK(butt_25_clique),NULL);
g_signal_connect(G_OBJECT(butt_ab),"pressed", G_CALLBACK(abandonner),NULL);
g_signal_connect(G_OBJECT(butt_s),"pressed", G_CALLBACK(switch_clique),NULL);
gtk_widget_show_all(fenetre_jeu);
gtk_widget_hide(image_non_50);
gtk_widget_hide(image_non_25);
gtk_widget_hide(image_non_s);
}
}
void ecran_menu_jeu()
{
fenetre_menu_jeu=gtk_window_new(GTK_WINDOW_TOPLEVEL);
cadre_menu_jeu=gtk_fixed_new();
gtk_window_set_title(GTK_WINDOW(fenetre_menu_jeu),"Menu du jeu");
gtk_window_set_modal(GTK_WINDOW(fenetre_menu_jeu),TRUE);
gtk_window_set_resizable(GTK_WINDOW(fenetre_menu_jeu), FALSE);
gtk_window_set_default_size(GTK_WINDOW(fenetre_menu_jeu),200,60);
gtk_container_add(GTK_CONTAINER(fenetre_menu_jeu),cadre_menu_jeu);
butt_nouv_jeu=gtk_button_new_with_label("Nouvelle pqrtie");
butt_statistique_joueur=gtk_button_new_with_label("Mes statistiques");
butt_stat_global=gtk_button_new_with_label("Statistique globale");
butt_supprimer=gtk_button_new_with_label("Supprimer mon compte");
butt_quitter=gtk_button_new_with_label("Quitter");
gtk_container_add(GTK_CONTAINER(fenetre_menu_jeu),cadre_menu_jeu);
gtk_fixed_put(GTK_FIXED(cadre_menu_jeu),butt_nouv_jeu,0,14);
gtk_fixed_put(GTK_FIXED(cadre_menu_jeu),butt_statistique_joueur,0,68);
gtk_fixed_put(GTK_FIXED(cadre_menu_jeu),butt_stat_global,0,122);
gtk_fixed_put(GTK_FIXED(cadre_menu_jeu),butt_supprimer,0,176);
gtk_fixed_put(GTK_FIXED(cadre_menu_jeu),butt_quitter,0,230);
gtk_window_set_position(GTK_WINDOW(fenetre_menu_jeu),GTK_WIN_POS_CENTER);
gtk_widget_set_size_request(butt_nouv_jeu,417,27);
gtk_widget_set_size_request(butt_statistique_joueur,417,27);
gtk_widget_set_size_request(butt_stat_global,417,27);
gtk_widget_set_size_request(butt_supprimer,417,27);
gtk_widget_set_size_request(butt_quitter,417,27);
g_signal_connect(G_OBJECT(butt_nouv_jeu),"clicked", G_CALLBACK(ecran_jeu),NULL);
g_signal_connect(G_OBJECT(butt_stat_global),"clicked", G_CALLBACK(ecran_stats_globale),NULL);
g_signal_connect(G_OBJECT(butt_statistique_joueur),"clicked", G_CALLBACK(ecran_stats_joueur),NULL);
g_signal_connect(G_OBJECT(butt_supprimer),"clicked", G_CALLBACK(supprimer_compte),NULL);
g_signal_connect(G_OBJECT(butt_quitter),"clicked", G_CALLBACK(quitter_menu_jeu),NULL);
gtk_widget_show_all(fenetre_menu_jeu);

}


void enregistrer_statistiques(int score,int u_50,int u_25,int u_s)
{
int i,j,temp,pmax;
int tab_score[7][2];
fp=fopen("stats_global.bin","r+b");
statistiques st,st2;
joueur jr;
fread(&st,sizeof(statistiques),1,fp);
for(i=0;i<15;i++) (st2.nbr_reponse_juste)[i]=(st.nbr_reponse_juste)[i]+nbr_reponse_juste[i];
for(i=0;i<6;i++)
{
  tab_score[i][0]=(st.meilleure_scores)[i]+3*(st.utilise_50)[i]+2*(st.utilise_25)[i]+1*(st.utilise_s)[i];
  tab_score[i][1]=i;
}

  tab_score[6][0]=score+3*u_50+2*u_25+u_s;
  tab_score[6][1]=6;

for(i=0;i<6;i++)
{
   pmax=i;
   for(j=i+1;j<7;j++)
   {
     if(tab_score[j][0]>tab_score[pmax][0])
     {
     temp=tab_score[pmax][0];
     tab_score[pmax][0]=tab_score[j][0];
     tab_score[j][0]=temp;
     temp=tab_score[pmax][1];
     tab_score[pmax][1]=tab_score[j][1];
     tab_score[j][1]=temp;
     pmax=j;
     }
   }
}

for(i=0;i<6;i++)
{
    if(tab_score[i][1]==6)
    {
    strcpy((st2.nom_meilleures_scores)[i],j_courant.nom);
    (st2.utilise_50)[i]=u_50;
    (st2.utilise_25)[i]=u_25;
    (st2.utilise_s)[i]=u_s;
    (st2.meilleure_scores)[i]=score;
    }
    else
    {
    strcpy((st2.nom_meilleures_scores)[i],(st.nom_meilleures_scores)[tab_score[i][1]]);
    (st2.utilise_50)[i]=(st.utilise_50)[tab_score[i][1]];
    (st2.utilise_25)[i]=(st.utilise_25)[tab_score[i][1]];
    (st2.utilise_s)[i]=(st.utilise_s)[tab_score[i][1]];
    (st2.meilleure_scores)[i]=(st.meilleure_scores)[tab_score[i][1]];
    }
}
rewind(fp);
fwrite(&st2,sizeof(statistiques),1,fp);
fclose(fp);
fp=fopen("joueurs.bin","r+b");
int tr=0;
while((tr==0)&&(fread(&jr,sizeof(joueur),1,fp)!=0))
{
    if(strcmp(jr.nom,j_courant.nom)==0)
    {
        tr=1;
    }
}
fseek(fp,-sizeof(joueur),SEEK_CUR);
if(j_courant.meilleur_score<score) j_courant.meilleur_score=score;
j_courant.somme_score=j_courant.somme_score+score;
j_courant.nbr_partie_jouee=j_courant.nbr_partie_jouee+1;
fwrite(&j_courant,sizeof(joueur),1,fp);
fclose(fp);
}

void ecran_stats_globale()
{
    statistiques st;
    int i;
    fenetre_stat_gb=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_modal(GTK_WINDOW(fenetre_stat_gb),TRUE);
    gtk_window_set_modal(GTK_WINDOW(fenetre_menu_jeu),FALSE);
    gtk_window_set_position(GTK_WINDOW(fenetre_stat_gb),GTK_WIN_POS_CENTER);
    fp=fopen("stats_global.bin","rb");
    fread(&st,sizeof(statistiques),1,fp);
    char exp[300];
    cadre_stat_gb=gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(fenetre_stat_gb),cadre_stat_gb);
    gtk_window_set_decorated(GTK_WINDOW(fenetre_stat_gb),FALSE);
    for(i=0;i<15;i++) label_nbr_p[i]=gtk_label_new("");
    butt_ok_s=gtk_button_new_with_label("  OK  ");
    for(i=0;i<6;i++)
    {
    if(strcmp(st.nom_meilleures_scores[i],"-------")!=0)
    {
    label_nom_m[i]=gtk_label_new("");
    label_score[i]=gtk_label_new("");
    if(st.utilise_50[i]==1) image_joker[0][i]=gtk_image_new_from_file("r_50.png");
    else image_joker[0][i]=gtk_image_new_from_file("non_r_50.png");
    if(st.utilise_25[i]==1) image_joker[1][i]=gtk_image_new_from_file("r_25.png");
    else image_joker[1][i]=gtk_image_new_from_file("non_r_25.png");
    if(st.utilise_s[i]==1) image_joker[2][i]=gtk_image_new_from_file("r_s.png");
    else image_joker[2][i]=gtk_image_new_from_file("non_r_s.png");
    }
    }
    for(i=0;i<6;i++)
    {
    if(strcmp(st.nom_meilleures_scores[i],"-------")!=0)
    {
    sprintf(exp,"%s%s%s",text_span_1,(st.nom_meilleures_scores)[i],text_span_2);
    gtk_label_set_text(GTK_LABEL(label_nom_m[i]),exp);
    gtk_label_set_use_markup(GTK_LABEL(label_nom_m[i]), TRUE);
    sprintf(exp,"%s%d DH%s",text_span_1,st.meilleure_scores[i],text_span_2);
    gtk_label_set_text(GTK_LABEL(label_score[i]),exp);
    gtk_label_set_use_markup(GTK_LABEL(label_score[i]), TRUE);
    }
    }
    image_st=gtk_image_new_from_file("stats_gb.png");
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),image_st,0,0);
    for(i=0;i<6;i++)
    {
    if(strcmp(st.nom_meilleures_scores[i],"-------")!=0)
    {
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),image_joker[0][i],252,53+55*i);
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),image_joker[1][i],303,53+55*i);
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),image_joker[2][i],356,53+55*i);
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),label_nom_m[i],130,62+55*i);
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),label_score[i],450,62+55*i);
    }
    }

    for(i=0;i<15;i++)
    {
    sprintf(exp,"<span foreground=\"white\">niveau %d DH : %d</span>",prix[i+1],(st.nbr_reponse_juste)[i]);
    gtk_label_set_text(GTK_LABEL(label_nbr_p[i]),exp);
    gtk_label_set_use_markup(GTK_LABEL(label_nbr_p[i]), TRUE);
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),label_nbr_p[i],18,420+i*15);
    }
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),butt_ok_s,600,660);
    g_signal_connect(G_OBJECT(butt_ok_s),"clicked", G_CALLBACK(quitter_ecran_stats_gb),NULL);
    fclose(fp);
    label_jr_e=gtk_label_new("");
    joueur jr;
    fp=fopen("joueurs.bin","rb");
    int cpt=0;
    while(fread(&jr,sizeof(joueur),1,fp)!=0) cpt++;
    sprintf(exp,"<span foreground=\"white\" font=\"Comic Sans MS\" size=\"xx-large\" > %d </span>",cpt);
    gtk_label_set_text(GTK_LABEL(label_jr_e),exp);
    gtk_label_set_use_markup(GTK_LABEL(label_jr_e), TRUE);
    gtk_fixed_put(GTK_FIXED(cadre_stat_gb),label_jr_e,300,657);
    gtk_widget_show_all(fenetre_stat_gb);
    fclose(fp);

}
void quitter_ecran_stats_gb()
{
    gtk_window_set_modal(GTK_WINDOW(fenetre_menu_jeu),TRUE);
    gtk_widget_destroy(fenetre_stat_gb);
}
void quitter_ecran_stats_j()
{
    gtk_window_set_modal(GTK_WINDOW(fenetre_menu_jeu),TRUE);
    gtk_widget_destroy(fenetre_stat_j);
}

void ecran_stats_joueur()
{
    char exp[300];
    bool ok=false;
    fenetre_stat_j=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    cadre_stat_j=gtk_fixed_new();
    butt_ok_sj=gtk_button_new_with_label("  OK  ");
    image_st_j=gtk_image_new_from_file("stats_jr.png");
    gtk_window_set_modal(GTK_WINDOW(fenetre_stat_j),TRUE);
    gtk_window_set_modal(GTK_WINDOW(fenetre_menu_jeu),FALSE);
    gtk_window_set_position(GTK_WINDOW(fenetre_stat_j),GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(GTK_WINDOW(fenetre_stat_j),FALSE);
    gtk_container_add(GTK_CONTAINER(fenetre_stat_j),cadre_stat_j);
    label_m_score=gtk_label_new("");
    label_nbr_pa=gtk_label_new("");
    label_somme_score=gtk_label_new("");
    sprintf(exp,"<span foreground=\"white\" font=\"Comic Sans MS\" size=\"xx-large\" > %d </span>",j_courant.nbr_partie_jouee);
    gtk_label_set_text(GTK_LABEL(label_nbr_pa),exp);
    gtk_label_set_use_markup(GTK_LABEL(label_nbr_pa), TRUE);
    sprintf(exp,"<span foreground=\"white\" font=\"Comic Sans MS\" size=\"xx-large\" > %d DH </span>",j_courant.meilleur_score);
    gtk_label_set_text(GTK_LABEL(label_m_score),exp);
    gtk_label_set_use_markup(GTK_LABEL(label_m_score), TRUE);
    sprintf(exp,"<span foreground=\"white\" font=\"Comic Sans MS\" size=\"xx-large\" > %d DH </span>",j_courant.somme_score);
    gtk_label_set_text(GTK_LABEL(label_somme_score),exp);
    gtk_label_set_use_markup(GTK_LABEL(label_somme_score), TRUE);
    gtk_fixed_put(GTK_FIXED(cadre_stat_j),image_st_j,0,0);
    gtk_fixed_put(GTK_FIXED(cadre_stat_j),label_nbr_pa,275,93);
    gtk_fixed_put(GTK_FIXED(cadre_stat_j),label_m_score,180,147);
    gtk_fixed_put(GTK_FIXED(cadre_stat_j),label_somme_score,390,196);
    gtk_fixed_put(GTK_FIXED(cadre_stat_j),butt_ok_sj,610,250);
    g_signal_connect(G_OBJECT(butt_ok_sj),"clicked", G_CALLBACK(quitter_ecran_stats_j),NULL);
    gtk_widget_show_all(fenetre_stat_j);

}
void ecran_champion()
{
    fenetre_champion=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    cadre_champion=gtk_fixed_new();
    butt_ok_c=gtk_button_new_with_label("Quitter");
    image_champion=gtk_image_new_from_file("champ.png");
    gtk_window_set_position(GTK_WINDOW(fenetre_champion),GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(GTK_WINDOW(fenetre_champion),FALSE);
    gtk_container_add(GTK_CONTAINER(fenetre_champion),cadre_champion);
    gtk_fixed_put(GTK_FIXED(cadre_champion),image_champion,0,0);
    gtk_fixed_put(GTK_FIXED(cadre_champion),butt_ok_c,800,600);
    g_signal_connect(G_OBJECT(butt_ok_c),"clicked", G_CALLBACK(quitter_ecran_champion),NULL);
    gtk_widget_show_all(fenetre_champion);
    //supprimer_sauvegarde();

}
void quitter_ecran_champion()
{
    gtk_widget_destroy(fenetre_champion);
}

void supprimer_compte()
{
  if(msgbox2(fenetre_menu_jeu,"Voulez vous vraiment supprimer votre compte ?")==1)
  {
  joueur jr;
  fp=fopen("joueurs.bin","rb");
  list_j=vider(list_j);
  LIST_JOUEURS *p;
  while(fread(&jr,sizeof(joueur),1,fp)!=0)
  {
      list_j=inserer_fin(jr,list_j);
  }
  fclose(fp);
  fp=fopen("joueurs.bin","wb");
  list_j=supprimer(j_courant.nom,list_j);
  p=list_j;
  while(p!=NULL)
    {
    fwrite(&(p->element),sizeof(joueur),1,fp);
    p=p->suivant;
    }
  fclose(fp);
  gtk_window_set_modal(GTK_WINDOW(fenetre_menu_jeu),FALSE);
  gtk_widget_destroy(fenetre_menu_jeu);
  }
}
void quitter_menu_jeu()
{
   if(msgbox2(fenetre_menu_jeu,"Voulez vous vraiment quitter le menu du jeu ?")==1)
   {
       gtk_widget_destroy(fenetre_menu_jeu);
   }
}

