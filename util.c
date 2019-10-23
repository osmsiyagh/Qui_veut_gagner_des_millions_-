#include <gtk/gtk.h>
#include "util.h"

GtkWidget *messagebox;
gchar *TexteConverti;

void msgbox1(GtkWidget *f,gchar *msg)
{
    TexteConverti=g_locale_to_utf8(msg,-1,NULL,NULL,NULL);
    messagebox=gtk_message_dialog_new (GTK_WINDOW(f),GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,TexteConverti);
    gtk_dialog_run(GTK_DIALOG(messagebox));
    gtk_widget_destroy(messagebox);

}
void msgbox(GtkWidget *f,gchar *msg)
{
    messagebox=gtk_message_dialog_new (GTK_WINDOW(f),GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,msg);
    gtk_dialog_run(GTK_DIALOG(messagebox));
    gtk_widget_destroy(messagebox);

}
int msgbox2(GtkWidget *f,gchar *msg)
{
   TexteConverti=g_locale_to_utf8(msg,-1,NULL,NULL,NULL);
   messagebox = gtk_message_dialog_new (GTK_WINDOW(f),GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER, GTK_BUTTONS_YES_NO,TexteConverti);
      switch(gtk_dialog_run(GTK_DIALOG(messagebox)))
              {
              case GTK_RESPONSE_YES:  gtk_widget_destroy(messagebox);return 1;break;
              case GTK_RESPONSE_NO:   gtk_widget_destroy(messagebox);return 0;break;
              }
}

void set_l(GtkWidget *l,char *txt)
{
    gchar *TexteConverti = g_locale_to_utf8(txt,-1,NULL,NULL,NULL);
    gtk_label_set_text(GTK_LABEL(l),TexteConverti);
    g_free(TexteConverti);
}
