


#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "login_creation.h"
#include "joueur_question.h"
#include "administration.h"
#include "jeu.h"
#include "util.h"
extern joueur j_courant;
extern administrateur a_courant;
extern GtkWidget *fenetre_accueil;
bool mode_admin;
// les �lements de la fenetre login
GtkWidget * fenetre_login=NULL;
GtkWidget * cadre_login=NULL;
GtkWidget * label_nom=NULL,*label_mot_passe=NULL,*label_info;
GtkWidget * text_nom=NULL,*text_mot_passe=NULL;
GtkWidget * butt_ok=NULL;
// les elements de la fenetre creation d'un joueur
GtkWidget * fenetre_creation=NULL;
GtkWidget * cadre_creation=NULL,*text_mot_passe_2=NULL,*label_mot_passe_2=NULL;
void verif_login()
{
     char nom[55],mot_passe_tapee[15],exp[150];
     bool valide;
     GtkWidget *pQuestion;
     FILE *fp_j,*fp_a;
     fp_a=fopen("admins.bin","rb");
     fp_j=fopen("joueurs.bin","rb");
     strcpy(nom,(char *)gtk_entry_get_text(GTK_ENTRY(text_nom)));
     strcpy(mot_passe_tapee,(char *)gtk_entry_get_text(GTK_ENTRY(text_mot_passe)));
     valide=false;
     while((valide==false)&&((fread(&j_courant,sizeof(joueur),1,fp_j))!=0))
       {
           if(((strcmp(j_courant.nom,nom))==0)&&((strcmp(j_courant.mot_passe,mot_passe_tapee))==0))
           {
               mode_admin=false;
               valide=true;
           }
       }
       while(((fread(&a_courant,sizeof(administrateur),1,fp_a))!=0)&&(valide==false))
       {
           if(((strcmp(a_courant.nom,nom))==0)&&((strcmp(a_courant.mot_passe,mot_passe_tapee))==0))
           {
               mode_admin=true;
               valide=true;

           }
       }
       if(valide==false)
          {
        pQuestion = gtk_message_dialog_new (GTK_WINDOW(fenetre_login),GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER, GTK_BUTTONS_YES_NO,
        "Erreur Nom inexistant ou mot de passe errone voulez vous refaire une autre tentative ");
        /* Affichage et attente d une reponse */
              switch(gtk_dialog_run(GTK_DIALOG(pQuestion)))
              {
              case GTK_RESPONSE_YES:  gtk_widget_destroy(pQuestion);
                                      gtk_entry_set_text(GTK_ENTRY(text_nom),"");
                                      gtk_entry_set_text(GTK_ENTRY(text_mot_passe),"");
                                      break;
              case GTK_RESPONSE_NO:   gtk_widget_destroy(pQuestion);
                                      gtk_widget_destroy(fenetre_login);
               break;
              }
          }
       if(valide==true)
          {
            if(mode_admin==true)
            {
             msgbox1(fenetre_login,"Je suis a votre service Admin !");
             gtk_widget_destroy(fenetre_login);
             _ecran_admin();
            }
            else
            {
                 sprintf(exp,"Salut! %s",j_courant.nom);
                 msgbox1(fenetre_login,exp);
                 gtk_widget_destroy(fenetre_login);
                 ecran_menu_jeu();
            }


          }

       fclose(fp_j);
       fclose(fp_a);
}


void creer_joueur()
{
     char nom[55],mot_passe_tapee[15],mot_passe_tapee_2[15];
     bool valide,ok_nom,ok_passe;
     joueur jr;
     administrateur admin;
     GtkWidget *pQuestion;
     FILE *fp_j,*fp_a;
     fp_a=fopen("admins.bin","rb");
     fp_j=fopen("joueurs.bin","rb");
     strcpy(nom,(char *)gtk_entry_get_text(GTK_ENTRY(text_nom)));
     strcpy(mot_passe_tapee,(char *)gtk_entry_get_text(GTK_ENTRY(text_mot_passe)));
     strcpy(mot_passe_tapee_2,(char *)gtk_entry_get_text(GTK_ENTRY(text_mot_passe_2)));
     ok_nom=true;
     while(((fread(&jr,sizeof(joueur),1,fp_j))!=0)&&(ok_nom==true))
       {
           if((strcmp(jr.nom,nom))==0)
           {
               ok_nom=false;
           }
       }
       while(((fread(&admin,sizeof(administrateur),1,fp_a))!=0)&&(ok_nom==true))
       {
           if((strcmp(admin.nom,nom))==0)
           {
               ok_nom=false;
           }
       }

    if(ok_nom==false)
    {
          msgbox1(fenetre_creation,"Veuillez ressayer à nouveau, ce nom d'utilisateur existe déja");
          fclose(fp_j);
          fclose(fp_a);
          gtk_entry_set_text(GTK_ENTRY(text_nom),"");
          gtk_entry_set_text(GTK_ENTRY(text_mot_passe),"");
          gtk_entry_set_text(GTK_ENTRY(text_mot_passe_2),"");

          return ;
    }
    if(strcmp(mot_passe_tapee,mot_passe_tapee_2)!=0)
     {
          msgbox1(fenetre_creation,"Veuillez ressayer à nouveau");
          gtk_entry_set_text(GTK_ENTRY(text_mot_passe),"");
          gtk_entry_set_text(GTK_ENTRY(text_mot_passe_2),"");
          fclose(fp_j);
          fclose(fp_a);
          return ;
     }
      fp_j=fopen("joueurs.bin","ab");
      jr=affecter_joueur(nom,mot_passe_tapee,0,0,0);
      fwrite(&jr,sizeof(joueur),1,fp_j);
      fclose(fp_j);
      msgbox1(fenetre_creation,"Validé.");
          gtk_widget_destroy(fenetre_creation);
}

void ecran_creation()
{
     label_info=gtk_label_new("Saisir le nom d'utilisateur et le mot de passe: ");
     label_nom=gtk_label_new("Nom \nd'utilisateur: ");
     label_mot_passe=gtk_label_new("Mot de passe: ");
     label_mot_passe_2=gtk_label_new("Confirmer le\n mot de passe: ");
     text_nom=gtk_entry_new();
     text_mot_passe=gtk_entry_new();
     text_mot_passe_2=gtk_entry_new();
     fenetre_creation=gtk_window_new(GTK_WINDOW_TOPLEVEL);
     cadre_creation=gtk_fixed_new();
     butt_ok=gtk_button_new_with_label("Inscription");
     gtk_container_add(GTK_CONTAINER(fenetre_creation),cadre_creation);
     gtk_fixed_put(GTK_FIXED(cadre_creation),label_info,15,0);
     gtk_fixed_put(GTK_FIXED(cadre_creation),label_nom,15,45);
     gtk_fixed_put(GTK_FIXED(cadre_creation),text_mot_passe,90,102);
     gtk_fixed_put(GTK_FIXED(cadre_creation),text_mot_passe_2,90,168);
     gtk_fixed_put(GTK_FIXED(cadre_creation),text_nom,90,42);
     gtk_fixed_put(GTK_FIXED(cadre_creation),label_mot_passe,15,105);
     gtk_fixed_put(GTK_FIXED(cadre_creation),label_mot_passe_2,15,165);
     gtk_entry_set_max_length (GTK_ENTRY(text_nom),15);
     gtk_entry_set_max_length (GTK_ENTRY(text_mot_passe),14);
     gtk_entry_set_max_length (GTK_ENTRY(text_mot_passe_2),14);
     gtk_fixed_put(GTK_FIXED(cadre_creation),butt_ok,130,210);
     gtk_window_set_title(GTK_WINDOW(fenetre_creation),"Creation d'un nouveau compte");
     gtk_window_set_position(GTK_WINDOW(fenetre_creation),GTK_WIN_POS_CENTER);
     gtk_window_set_default_size(GTK_WINDOW(fenetre_creation),300,250);
     gtk_entry_set_visibility(GTK_ENTRY(text_mot_passe),FALSE);
     gtk_entry_set_visibility(GTK_ENTRY(text_mot_passe_2),FALSE);
     gtk_widget_set_parent(fenetre_creation,fenetre_accueil);
     gtk_window_set_modal(GTK_WINDOW(fenetre_creation),TRUE);
     g_signal_connect(G_OBJECT(butt_ok),"clicked", G_CALLBACK(creer_joueur),NULL);
     gtk_widget_set_parent(fenetre_creation,fenetre_accueil);
     gtk_window_set_modal(GTK_WINDOW(fenetre_creation),TRUE);
     gtk_widget_show_all(fenetre_creation);
}

void ecran_login()
{

     label_info=gtk_label_new("Saisir le nom d'utilisateur et le mot de passe:");
     label_nom=gtk_label_new("Nom \nd'utilisateur:");
     label_mot_passe=gtk_label_new("Mot de passe:");
     text_nom=gtk_entry_new();
     text_mot_passe=gtk_entry_new();
     fenetre_login=gtk_window_new(GTK_WINDOW_TOPLEVEL);
     cadre_login=gtk_fixed_new();
     butt_ok=gtk_button_new_with_label("Commencer");
     gtk_container_add(GTK_CONTAINER(fenetre_login),cadre_login);
     gtk_fixed_put(GTK_FIXED(cadre_login),label_info,0,0);
     gtk_fixed_put(GTK_FIXED(cadre_login),label_nom,15,45);
     gtk_fixed_put(GTK_FIXED(cadre_login),text_mot_passe,90,102);
     gtk_fixed_put(GTK_FIXED(cadre_login),text_nom,90,42);
     gtk_fixed_put(GTK_FIXED(cadre_login),label_mot_passe,15,105);
     gtk_fixed_put(GTK_FIXED(cadre_login),butt_ok,100,150);
     gtk_entry_set_max_length (GTK_ENTRY(text_nom),15);
     gtk_entry_set_max_length (GTK_ENTRY(text_mot_passe),14);
     gtk_window_set_title(GTK_WINDOW(fenetre_login), "Se loger");
     gtk_window_set_position(GTK_WINDOW(fenetre_login),GTK_WIN_POS_CENTER);
     gtk_window_set_default_size(GTK_WINDOW(fenetre_login),300,200);
     gtk_entry_set_visibility(GTK_ENTRY(text_mot_passe),FALSE);
     gtk_widget_set_parent(fenetre_login,fenetre_accueil);
     gtk_window_set_modal(GTK_WINDOW(fenetre_login),TRUE);
     g_signal_connect(G_OBJECT(butt_ok),"clicked", G_CALLBACK(verif_login),NULL);
     gtk_widget_show_all(fenetre_login);

}
