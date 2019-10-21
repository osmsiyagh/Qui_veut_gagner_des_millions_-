#include "login_creation.h"
#include "accueil.h"
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

// les elements de la fenetre d'acceuik
GtkWidget * fenetre_accueil=NULL, *cadre_accueil=NULL;
GtkWidget * butt_l=NULL,*butt_c=NULL,*butt_q=NULL, *butt_admin=NULL, *butt_credit=NULL;
GtkWidget * image1=NULL;


void ecran_accueil()
{
    cadre_accueil=gtk_fixed_new();
    fenetre_accueil=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    butt_l=gtk_button_new_with_label("Acceder a votre compte");
    butt_c=gtk_button_new_with_label("Creer un nouveau compte");
    butt_admin=gtk_button_new_with_label("Administration");
    butt_credit=gtk_button_new_with_label("Credits");
    butt_q=gtk_button_new_with_label("Quitter");
    image1=gtk_image_new_from_file("background.jpg");
    gtk_window_set_resizable(GTK_WINDOW(fenetre_accueil), FALSE);
    gtk_container_add(GTK_CONTAINER(fenetre_accueil),cadre_accueil);
    gtk_fixed_put(GTK_FIXED(cadre_accueil),image1,0,0);
    gtk_fixed_put(GTK_FIXED(cadre_accueil),butt_l,30,15);
    gtk_fixed_put(GTK_FIXED(cadre_accueil),butt_c,30,75);
    gtk_fixed_put(GTK_FIXED(cadre_accueil),butt_credit,30,135);
    gtk_fixed_put(GTK_FIXED(cadre_accueil),butt_q,30,195);
    gtk_fixed_put(GTK_FIXED(cadre_accueil),butt_admin,550,15);
    g_signal_connect(G_OBJECT(fenetre_accueil),"destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(butt_l),"clicked", G_CALLBACK(ecran_login),NULL);
    //////
    g_signal_connect(G_OBJECT(butt_c),"clicked", G_CALLBACK(ecran_creation),NULL);
    g_signal_connect(G_OBJECT(butt_credit),"clicked", G_CALLBACK(credit),NULL);
    g_signal_connect(G_OBJECT(butt_q),"clicked", G_CALLBACK(fermer),NULL);
    g_signal_connect(G_OBJECT(butt_admin),"clicked", G_CALLBACK(ecran_login),NULL);
    gtk_window_set_title(GTK_WINDOW(fenetre_accueil),"Qui Veut Gagner des millions ? ");
    gtk_window_set_position(GTK_WINDOW(fenetre_accueil),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(fenetre_accueil),800,600);
    gtk_widget_show_all(fenetre_accueil);
}

void credit()
{
    GtkWidget * _credit=NULL, *cadre=NULL, *fenetre_creation=NULL;

    _credit=gtk_label_new("Version 1.0.2 projet C \n ENSIAS\n SIYAGH OUSSAMA\n GHITA EL AMLAQUI");
    fenetre_creation=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    cadre=gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(cadre),_credit,150,100);
    gtk_container_add(GTK_CONTAINER(fenetre_creation),cadre);
    gtk_window_set_title(GTK_WINDOW(fenetre_creation), "Credits");
    gtk_window_set_position(GTK_WINDOW(fenetre_creation),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(fenetre_creation),400,300);
    gtk_widget_show_all(fenetre_creation);
}



void fermer()
{
     GtkWidget * pQuestion=NULL;
     pQuestion = gtk_message_dialog_new (GTK_WINDOW(fenetre_accueil),GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER, GTK_BUTTONS_YES_NO,
     "t'es sur(e) ?");
      switch(gtk_dialog_run(GTK_DIALOG(pQuestion)))
              {
              case GTK_RESPONSE_YES:  gtk_main_quit();break;
              case GTK_RESPONSE_NO:   gtk_widget_destroy(pQuestion);break;
              }
}

