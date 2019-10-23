#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <gtk/gtk.h>
#include "joueur_question.h"
#include "login_creation.h"
#include "accueil.h"

joueur  j_courant;
administrateur  a_courant;
int main(int argc, char **argv)
{
    /* Initialisation de GTK+ */
    gtk_init(&argc, &argv);
    ecran_accueil();
    gtk_main();
    return EXIT_SUCCESS;
}
