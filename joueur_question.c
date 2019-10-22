#include "joueur_question.h"
#include <stdlib.h>
#include <string.h>
int rechercher(char *chaine,char *mot_cle)
{
    int l=strlen(chaine)-strlen(mot_cle)+1;
    if( l<0) return 0;
    int i ,k,j,ok;
    for(i=0;i<=l;i++)
    {
        k=0;
        if(chaine[i]==mot_cle[0])
        {
           ok=1;
           for(j=1;mot_cle[j]!='\0';j++)
           {
             if(chaine[i+j]!=mot_cle[j]) ok=0;
           }
           if(ok==1) return 1;
        }
    }
return 0;
}
joueur affecter_joueur(char * nom,char * mot_passe,int meilleur_score,int somme_score,int nbr_partie_jouee)
  {
      joueur jr;
      strcpy(jr.nom,nom);
      strcpy(jr.mot_passe,mot_passe);
      jr.meilleur_score=meilleur_score;
      jr.somme_score=somme_score;
      jr.nbr_partie_jouee=nbr_partie_jouee;
      return jr;
  }
QUESTION affecter_question(char *question,char *reponse1,char *reponse2,char *reponse3,char *reponse4,int reponse_correcte,int niveau,int reponse_proche)
{
      QUESTION q;
      int i;
      strcpy(q.question,question);
      strcpy(q.reponses[0],reponse1);
      strcpy(q.reponses[1],reponse2);
      strcpy(q.reponses[2],reponse3);
      strcpy(q.reponses[3],reponse4);

      q.reponse_correcte=reponse_correcte;
      q.niveau=niveau;
      q.reponse_proche=reponse_proche;
      return q;
}
QUESTION copier_question(QUESTION q1)
{
QUESTION q;
      int i;
      strcpy(q.question,q1.question);
      strcpy(q.reponses[0],q1.reponses[0]);
      strcpy(q.reponses[1],q1.reponses[1]);
      strcpy(q.reponses[2],q1.reponses[2]);
      strcpy(q.reponses[3],q1.reponses[3]);

      q.reponse_correcte=q1.reponse_correcte;
      q.niveau=q1.niveau;
      q.reponse_proche=q1.reponse_proche;
      return q;
}
LIST_JOUEURS * inserer_fin(joueur element,LIST_JOUEURS *l)
{
  LIST_JOUEURS *r,*p;
  if(l==NULL)
  {
  l=(LIST_JOUEURS *)malloc(sizeof(LIST_JOUEURS));
  l->element=affecter_joueur(element.nom,element.mot_passe,element.meilleur_score,element.somme_score,element.nbr_partie_jouee);
  l->suivant=NULL;
  }
  else
  {
  r=l;
  while(r->suivant!=NULL)
  {
  r=r->suivant;
  }
  p=(LIST_JOUEURS *)malloc(sizeof(LIST_JOUEURS));
  p->element=affecter_joueur(element.nom,element.mot_passe,element.meilleur_score,element.somme_score,element.nbr_partie_jouee);
  r->suivant=p;
  p->suivant=NULL;
  }
  return l;
}

LIST_JOUEURS * supprimer(char *nom,LIST_JOUEURS *l)
{
     LIST_JOUEURS *p,*q;
     if(l!=NULL)
     {
       p=l;
       if(strcmp(l->element.nom,nom)==0)
       {
       q=l;
       l=l->suivant;
       free(q);
       }
       else
       {
       q=p;
       while(strcmp(p->element.nom,nom)!=0)
       {
       q=p;
       p=p->suivant;
       }
       q->suivant=p->suivant;
       free(p);
       }
     }
return l;
}
LIST_JOUEURS * vider(LIST_JOUEURS *l)
{
    LIST_JOUEURS *q;
    while(l!=NULL)
    {
     q=l;
     l=l->suivant;
     free(q);
   }
   return l;
}
LIST_QUESTIONS * inserer_fin_q(QUESTION element,LIST_QUESTIONS *l)
{
  LIST_QUESTIONS *r,*p;
  if(l==NULL)
  {
  l=(LIST_QUESTIONS *)malloc(sizeof(LIST_QUESTIONS));
  l->element=affecter_question(element.question,element.reponses[0],element.reponses[1],element.reponses[2],element.reponses[3],element.reponse_correcte,element.niveau,element.reponse_proche);
  l->suivant=NULL;
  }
  else
  {
  r=l;
  while(r->suivant!=NULL)
  {
  r=r->suivant;
  }
  p=(LIST_QUESTIONS *)malloc(sizeof(LIST_QUESTIONS));
  p->element=affecter_question(element.question,element.reponses[0],element.reponses[1],element.reponses[2],element.reponses[3],element.reponse_correcte,element.niveau,element.reponse_proche);
  r->suivant=p;
  p->suivant=NULL;
  }
  return l;
}
LIST_QUESTIONS * supprimer_q(char *e,LIST_QUESTIONS *l)
{
     LIST_QUESTIONS *p,*q;
     if(l!=NULL)
     {
       p=l;
       if(strcmp((l->element).question,e)==0)
       {
       q=l;
       l=l->suivant;
       free(q);
       }
       else
       {
       q=p;
       while(strcmp((p->element).question,e)!=0)
       {
       q=p;
       p=p->suivant;
       }
       q->suivant=p->suivant;
       free(p);
       }
     }
return l;
}
LIST_QUESTIONS * vider_q(LIST_QUESTIONS *l)
{
    LIST_QUESTIONS *q;
    while(l!=NULL)
    {
     q=l;
     l=l->suivant;
     free(q);
   }
   return l;
}
