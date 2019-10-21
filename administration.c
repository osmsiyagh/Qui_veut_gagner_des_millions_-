

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "administration.h"
#include "joueur_question.h"
#include "util.h"
#include "jeu.h"
LIST_JOUEURS *list_j=NULL;
LIST_JOUEURS *list_f=NULL;
LIST_QUESTIONS *list_q=NULL;
LIST_QUESTIONS *list_qf=NULL;
FILE *fp;
// les elements de la fennetre d'administration
GtkWidget * fenetre_admin=NULL;
GtkWidget * cadre_admin=NULL;
GtkWidget * butt_questions=NULL;
GtkWidget * butt_joueurs=NULL;
GtkWidget * butt_exit=NULL;
GtkWidget * supp_sta=NULL;
// le fenetre des éléments de la suppression des joueurs
GtkWidget * fenetre_s_joueurs=NULL,* cadre_s_joueurs=NULL;
GtkWidget *listview=NULL,*butt_supprimer=NULL,*butt_j_quitter=NULL,*butt_filtrer=NULL,*butt_supp_filtre;
GtkWidget *scrollbar=NULL;
GtkListStore *liststore=NULL;
GtkTreeViewColumn *column=NULL;
GtkCellRenderer *cellrenderer=NULL;
GtkTreeSelection *select=NULL;
// les elements de la fenetre de filtre des joueurs
GtkWidget * fenetre_filtre_joueurs=NULL,* cadre_filtre_joueurs=NULL;
GtkWidget * text_mot_cle=NULL,*label_f=NULL;
// les elements de gestion des questions
GtkWidget * fenetre_g_question=NULL,* cadre_g_question=NULL;
GtkWidget *butt_ajouter, *butt_modifier;
// les elements de l'ajout des quiz
GtkWidget * fenetre_a_question=NULL,* cadre_a_question=NULL;
GtkWidget *text_question,*text_reponse[3],*text_niveau,*text_reponse_proche,*text_reponse_correcte;
GtkWidget *label_question,*label_reponse,*label_niveau,*label_reponse_proche,*label_reponse_correcte;
GtkWidget *butt_ok;
GtkWidget *spin_niveau,*spin_reponse_correcte,*spin_reponse_proche;
void _ecran_admin()
{
    cadre_admin=gtk_fixed_new();
    fenetre_admin=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable(GTK_WINDOW(fenetre_admin), FALSE);
    butt_questions=gtk_button_new_with_label("Gestion des quiz");
    butt_joueurs=gtk_button_new_with_label("Suppression des comptes");
    supp_sta=gtk_button_new_with_label("Suppression des statistiques");
    butt_exit=gtk_button_new_with_label("Quitter");
    gtk_window_set_modal(GTK_WINDOW(fenetre_admin),TRUE);
    gtk_container_add(GTK_CONTAINER(fenetre_admin),cadre_admin);
    gtk_fixed_put(GTK_FIXED(cadre_admin),butt_questions,0,10);
    gtk_fixed_put(GTK_FIXED(cadre_admin),butt_joueurs,0,50);
    gtk_fixed_put(GTK_FIXED(cadre_admin),supp_sta,0,90);
    gtk_fixed_put(GTK_FIXED(cadre_admin),butt_exit,0,130);
    gtk_widget_set_size_request(butt_questions,417,27);
    gtk_widget_set_size_request(butt_joueurs,417,27);
    gtk_widget_set_size_request(supp_sta,417,27);
    gtk_widget_set_size_request(butt_exit,417,27);
    gtk_window_set_title(GTK_WINDOW(fenetre_admin),"Administration");
    gtk_window_set_position(GTK_WINDOW(fenetre_admin),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(fenetre_admin),50,50);
    g_signal_connect(G_OBJECT(butt_joueurs),"clicked", G_CALLBACK(_ecran_gestion_des_comptes),NULL);
    g_signal_connect(G_OBJECT(butt_questions),"clicked", G_CALLBACK(_ecran_gestion_des_quiz),NULL);
    g_signal_connect(G_OBJECT(supp_sta),"clicked", G_CALLBACK(supprimer_statistiques),NULL);
    g_signal_connect(G_OBJECT(butt_exit),"clicked", G_CALLBACK(quitter_admin),NULL);
    gtk_widget_show_all(fenetre_admin);
}

void _ajouter_quiz()
{
QUESTION q;
GtkTreeIter piter;
GtkWidget * pQuestion;
int i;
strcpy(q.question,gtk_entry_get_text(GTK_ENTRY(text_question)));
for(i=0;i<4;i++)
{
strcpy(q.reponses[i],gtk_entry_get_text(GTK_ENTRY(text_reponse[i])));
}
q.niveau=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_niveau));
q.reponse_proche=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_reponse_proche));
q.reponse_correcte=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_reponse_correcte));
gtk_list_store_move_before(liststore,&piter,NULL);
gtk_list_store_append(liststore, &piter);
gtk_list_store_set(liststore, &piter,0,q.question,1,q.reponses[0],2,q.reponses[1],3,q.reponses[2],4,q.reponses[3],5,q.reponse_correcte,6,q.niveau,7,q.reponse_proche,-1);
list_q=inserer_fin_q(q,list_q);
list_qf=inserer_fin_q(q,list_qf);
fp=fopen("questions.bin","ab");
fwrite(&q,sizeof(QUESTION),1,fp);
fclose(fp);
msgbox1(fenetre_a_question,"Le quiz est bien ajoute!");
gtk_widget_destroy(fenetre_a_question);
}
void _supprimer_quizz()
{
        GtkTreeIter iter;
        GtkTreeModel *model;
        GtkWidget * pQuestion;

        char ques[150];
        LIST_QUESTIONS *p;
        QUESTION q;
        gchar *question;
        if (gtk_tree_selection_get_selected (select,&model,&iter))
        {
                gtk_tree_model_get (model, &iter,0,&question,-1);
                gtk_list_store_remove(liststore,&iter);

                list_q=vider_q(list_q);
                strcpy(ques,question);
                fp=fopen("questions.bin","rb");
                while(fread(&q,sizeof(QUESTION),1,fp)!=0)
                {
                  if(strcmp(q.question,ques)!=0)
                  {
                     list_q=inserer_fin_q(q,list_q);
                  }
                }
                fclose(fp);
                msgbox1(fenetre_g_question,"Fait.");
                g_free(question);


        fp=fopen("questions.bin","wb");
        p=list_q;
        while(p!=NULL)
        {
        fwrite(&(p->element),sizeof(QUESTION),1,fp);
        p=p->suivant;
        }
        fclose(fp);
        }


}


void _ecran_gestion_des_quiz()
{
  QUESTION q;
  GtkTreeIter piter;
  list_q=vider_q(list_q);
  list_qf=vider_q(list_qf);
  char questions[55];
  cadre_g_question=gtk_fixed_new();
  fenetre_g_question=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_resizable(GTK_WINDOW(fenetre_g_question), FALSE);
  gtk_window_set_modal(GTK_WINDOW(fenetre_admin),FALSE);
  gtk_window_set_modal(GTK_WINDOW(fenetre_g_question),TRUE);
  liststore =gtk_list_store_new(8, G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_INT,G_TYPE_INT,G_TYPE_INT);
  gtk_list_store_clear(liststore);
  gtk_window_set_title(GTK_WINDOW(fenetre_g_question),"Gestion des quiz");
  gtk_list_store_move_after(liststore,&piter,NULL);
  butt_ajouter=gtk_button_new_with_label("Ajouter un quiz");
  butt_supprimer=gtk_button_new_with_label("Supprimer un quiz");
  butt_j_quitter=gtk_button_new_with_label("Quitter");
  fp=fopen("questions.bin","rb");
  while((fread(&q,sizeof(QUESTION),1,fp))!=0)
  {
    gtk_list_store_append(liststore, &piter);
    gtk_list_store_set(liststore, &piter,0,q.question,1,q.reponses[0],2,q.reponses[1],3,q.reponses[2],4,q.reponses[3],5,q.reponse_correcte,6,q.niveau,7,q.reponse_proche,-1);
    list_q=inserer_fin_q(q,list_q);
    list_qf=inserer_fin_q(q,list_qf);

  }
  fclose(fp);


    listview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
    cellrenderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Question",cellrenderer,"text",0,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Reponse 1",cellrenderer,"text",1,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Reponse 2",cellrenderer,"text",2,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Reponse 3",cellrenderer,"text",3,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Reponse 4",cellrenderer,"text",4,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Reponse correcte",cellrenderer,"text",5,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Niveau",cellrenderer,"text",6,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Reponse proche",cellrenderer,"text",7,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrollbar), listview);
    gtk_container_add(GTK_CONTAINER(fenetre_g_question),cadre_g_question);
    gtk_window_set_default_size(GTK_WINDOW(fenetre_g_question),1080,720);
    gtk_fixed_put(GTK_FIXED(cadre_g_question),butt_ajouter,900,10);
    gtk_fixed_put(GTK_FIXED(cadre_g_question),butt_supprimer,900,90);
    gtk_fixed_put(GTK_FIXED(cadre_g_question),scrollbar,0,0);
    gtk_widget_set_size_request(scrollbar,650,700);
    gtk_widget_set_size_request(butt_ajouter,417,27);
    gtk_widget_set_size_request(butt_supprimer,417,27);
    select = gtk_tree_view_get_selection (GTK_TREE_VIEW(listview));
    gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
    g_signal_connect(G_OBJECT(butt_ajouter),"clicked", G_CALLBACK(ecran_a_question),NULL);
    g_signal_connect(G_OBJECT(butt_supprimer),"clicked", G_CALLBACK(_supprimer_quizz),NULL);
    g_signal_connect(G_OBJECT(fenetre_g_question),"destroy", G_CALLBACK(quitter_gestion_question),NULL);
    gtk_window_set_position(GTK_WINDOW(fenetre_g_question),GTK_WIN_POS_CENTER);
    gtk_widget_show_all(fenetre_g_question);
}

void _ecran_gestion_des_comptes()
{
  joueur jr;
  GtkTreeIter piter;
  list_j=vider(list_j);
  list_f=vider(list_f);
  char nom[55];
  fp=fopen("joueurs.bin","rb");
  cadre_s_joueurs=gtk_fixed_new();
  fenetre_s_joueurs=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_resizable(GTK_WINDOW(fenetre_s_joueurs), FALSE);
  gtk_window_set_modal(GTK_WINDOW(fenetre_admin),FALSE);
  gtk_window_set_modal(GTK_WINDOW(fenetre_s_joueurs),TRUE);
  gtk_window_set_title(GTK_WINDOW(fenetre_s_joueurs),"Suppression des comptes");
  liststore = gtk_list_store_new(4, G_TYPE_STRING,G_TYPE_INT,G_TYPE_INT,G_TYPE_INT);
  gtk_list_store_clear(liststore);
  gtk_list_store_move_after(liststore,&piter,NULL);
  butt_supprimer=gtk_button_new_with_label("Supprimer le compte");
  butt_j_quitter=gtk_button_new_with_label("Quitter ");
  while((fread(&jr,sizeof(joueur),1,fp))!=0)
  {
    gtk_list_store_append(liststore, &piter);
    gtk_list_store_set(liststore, &piter,0,jr.nom,1,jr.meilleur_score,2,jr.somme_score,3,jr.nbr_partie_jouee,-1);
    list_j=inserer_fin(jr,list_j);
    list_f=inserer_fin(jr,list_f);

  }
  fclose(fp);

    listview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
    cellrenderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom d'utilisateur",cellrenderer,"text",0,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Meilleur Score",cellrenderer,"text",1,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Score",cellrenderer,"text",2,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    column = gtk_tree_view_column_new_with_attributes("Nombre de parties jouees",cellrenderer,"text",3,NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(listview), column);
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrollbar), listview);
    gtk_window_set_default_size(GTK_WINDOW(fenetre_s_joueurs),600,400);
    gtk_fixed_put(GTK_FIXED(cadre_s_joueurs),butt_supprimer,400,90);
    gtk_fixed_put(GTK_FIXED(cadre_s_joueurs),scrollbar,0,0);
    gtk_widget_set_size_request(butt_supprimer,417,27);
    gtk_widget_set_size_request(scrollbar,350,300);
    select = gtk_tree_view_get_selection (GTK_TREE_VIEW(listview));
    gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
    g_signal_connect(G_OBJECT(butt_supprimer),"clicked", G_CALLBACK(supprimer_joueur),NULL);
    g_signal_connect(G_OBJECT(fenetre_s_joueurs),"destroy", G_CALLBACK(quitter_gestion_joueur),NULL);
    gtk_container_add(GTK_CONTAINER(fenetre_s_joueurs),cadre_s_joueurs);
    gtk_widget_show_all(fenetre_s_joueurs);

}
void ecran_a_question()
{
int i;
cadre_a_question=gtk_fixed_new();
fenetre_a_question=gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_resizable(GTK_WINDOW(fenetre_a_question), FALSE);
gtk_window_set_modal(GTK_WINDOW(fenetre_a_question),TRUE);
gtk_window_set_modal(GTK_WINDOW(fenetre_g_question),FALSE);
text_question=gtk_entry_new();
spin_niveau=gtk_spin_button_new_with_range(1,15,1);

for(i=0;i<4;i++)
{
text_reponse[i]=gtk_entry_new();
}
text_niveau=gtk_entry_new();
text_reponse_proche=gtk_entry_new();
text_reponse_correcte=gtk_entry_new();
spin_reponse_correcte=gtk_spin_button_new_with_range(0,3,1);
spin_reponse_proche=gtk_spin_button_new_with_range(0,3,1);
label_question=gtk_label_new("La question :");
label_reponse=gtk_label_new("Les reponses :");
label_reponse_proche=gtk_label_new("La reponse proche :");
label_reponse_correcte=gtk_label_new("La reponse correcte :");
label_niveau=gtk_label_new("Le niveau associe : ");
butt_ok=gtk_button_new_with_label("OK");
gtk_container_add(GTK_CONTAINER(fenetre_a_question),cadre_a_question);
gtk_fixed_put(GTK_FIXED(cadre_a_question),label_question,7,8);
gtk_widget_set_size_request(label_question,100,20);
gtk_fixed_put(GTK_FIXED(cadre_a_question),text_question,94,10);
gtk_widget_set_size_request(text_question,602,22);
gtk_fixed_put(GTK_FIXED(cadre_a_question),label_reponse,7,52);
gtk_widget_set_size_request(label_reponse,100,20);
gtk_fixed_put(GTK_FIXED(cadre_a_question),text_reponse[0],94,54);
gtk_fixed_put(GTK_FIXED(cadre_a_question),text_reponse[1],264,54);
gtk_fixed_put(GTK_FIXED(cadre_a_question),text_reponse[2],94,95);
gtk_fixed_put(GTK_FIXED(cadre_a_question),text_reponse[3],264,95);
gtk_fixed_put(GTK_FIXED(cadre_a_question),label_niveau,12,211);
gtk_widget_set_size_request(label_niveau,100,20);
gtk_fixed_put(GTK_FIXED(cadre_a_question),spin_niveau,214,208);
gtk_fixed_put(GTK_FIXED(cadre_a_question),label_reponse_proche,12,172);
gtk_widget_set_size_request(label_reponse_proche,100,20);
gtk_fixed_put(GTK_FIXED(cadre_a_question),spin_reponse_proche,214,171);
gtk_fixed_put(GTK_FIXED(cadre_a_question),label_reponse_correcte,12,133);
gtk_widget_set_size_request(label_reponse_correcte,100,20);
gtk_fixed_put(GTK_FIXED(cadre_a_question),spin_reponse_correcte,214,132);
gtk_fixed_put(GTK_FIXED(cadre_a_question),butt_ok,300,262);
gtk_window_set_title(GTK_WINDOW(fenetre_a_question),"Ajout d'un nouveau quiz ");
g_signal_connect(G_OBJECT(butt_ok),"clicked", G_CALLBACK(_ajouter_quiz),NULL);
g_signal_connect(G_OBJECT(fenetre_a_question),"destroy", G_CALLBACK(fermer_ecran_ajout_question),NULL);
gtk_window_set_position(GTK_WINDOW(fenetre_a_question),GTK_WIN_POS_CENTER);
gtk_window_set_default_size(GTK_WINDOW(fenetre_a_question),700,300);
gtk_widget_show_all(fenetre_a_question);
}
void supprimer_joueur()
{
        GtkTreeIter iter;
        GtkTreeModel *model;
        GtkWidget * pQuestion;
        gchar *nom;
        LIST_JOUEURS *p;
        if (gtk_tree_selection_get_selected (select, &model, &iter))
        {
                gtk_tree_model_get (model, &iter,0,&nom, -1);
                gtk_list_store_remove(liststore,&iter);
                list_j=supprimer(nom,list_j);
                list_f=supprimer(nom,list_f);
                msgbox1(fenetre_s_joueurs,"le compte est supprimé");
                g_free(nom);

        fp=fopen("joueurs.bin","wb");
        p=list_j;
        while(p!=NULL)
        {
        fwrite(&(p->element),sizeof(joueur),1,fp);
        p=p->suivant;
        }
        fclose(fp);
        }
}

void supprimer_statistiques()
{
    if(msgbox2(fenetre_admin,"Voulez vous vraiment rénitialiser les statistiques  ")==1)
    {
    statistiques st;
    list_j=vider(list_j);
    LIST_JOUEURS *p;
    int i;
    joueur jr;
    for(i=0;i<6;i++)
    {
    strcpy((st.nom_meilleures_scores)[i],"-------");
    (st.utilise_50)[i]=1;
    (st.utilise_25)[i]=1;
    (st.utilise_s)[i]=1;
    (st.meilleure_scores)[i]=0;
    }
    for(i=0;i<15;i++) (st.nbr_reponse_juste)[i]=0;
    fp=fopen("joueurs.bin","rb");
    while((fread(&jr,sizeof(joueur),1,fp)!=0))
    {
        jr.meilleur_score=0;
        jr.nbr_partie_jouee=0;
        jr.somme_score=0;
        list_j=inserer_fin(jr,list_j);
    }
    fclose(fp);
    fp=fopen("joueurs.bin","wb");
    p=list_j;
    while(p!=NULL)
    {
        fwrite(&(p->element),sizeof(joueur),1,fp);
        p=p->suivant;
    }
    fclose(fp);
    fp=fopen("stats_global.bin","wb");
    fwrite(&st,1,sizeof(statistiques),fp);
    fclose(fp);
    }
}
void quitter_admin()
{
  gtk_widget_destroy(fenetre_admin);
}
void quitter_gestion_question()
{
    gtk_window_set_modal(GTK_WINDOW(fenetre_admin),TRUE);
}
void fermer_ecran_ajout_question()
{
    gtk_window_set_modal(GTK_WINDOW(fenetre_g_question),TRUE);
}
void quitter_gestion_joueur()
{
    gtk_window_set_modal(GTK_WINDOW(fenetre_admin),TRUE);
}
