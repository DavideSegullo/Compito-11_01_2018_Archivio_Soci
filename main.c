#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 21

typedef enum{false,true} bool;

typedef struct {
    char Cognome[N];
    char Nome[N];
    char Codice_Fiscale[17];
    char Email[N];
} Dati_socio;

typedef struct vettura {
    char Marca[10];
    char Modello[N];
    int Anno;
    struct vettura *next;
} Vettura;

typedef struct socio {
    Dati_socio dati;
    Vettura *elenco_vetture;
    struct socio *next;
} Socio;

bool crea_lista_vettura(Vettura **vetture, Vettura *p){

    Vettura *q = (Vettura *)malloc(sizeof(Vettura));

    if (q == NULL)
        return false;

    q->Anno = p->Anno;
    strcpy(q->Marca,p->Marca);
    strcpy(q->Modello,p->Modello);
    q->next = NULL;
    *vetture = q;

    return true;
}

bool inserimento_lista_vettura(Vettura **vetture, Vettura *p){

    if (*vetture == NULL)
        return crea_lista_vettura(vetture,p);

    Vettura *q = (Vettura *)malloc(sizeof(Vettura));
    q->Anno = p->Anno;
    strcpy(q->Marca,p->Marca);
    strcpy(q->Modello,p->Modello);

    q->next = *vetture;
    *vetture = q;

    return true;
}

bool crea_lista_socio(Socio **archivio, Socio *p){

    Socio *q = (Socio *)malloc(sizeof(Socio));

    if (q == NULL)
        return false;

    q->dati = p->dati;
    q->elenco_vetture = p->elenco_vetture;
    q->next = NULL;
    *archivio = q;

    return true;
}

bool inserimento_lista_socio(Socio **archivio, Socio *p){

    if (*archivio == NULL)
        return crea_lista_socio(archivio,p);

    Socio *current = NULL;
    Socio *successive = *archivio;

    while(successive!=NULL && strcmp(successive->dati.Cognome,p->dati.Cognome)<0){

        if (strcmp(successive->dati.Cognome,p->dati.Cognome)==0)
            if (strcmp(successive->dati.Nome,p->dati.Nome)>0)
                break;

        current = successive;
        successive = successive->next;
    }

    Socio *q = (Socio *)malloc(sizeof(Socio));
    q->dati=p->dati;
    q->elenco_vetture = p->elenco_vetture;
    q->next = NULL;

    if (current == NULL){
        q->next = *archivio;
        *archivio = q;
        return true;
    }

    if (successive == NULL){
        current->next = q;
        return true;
    }

    q->next = successive;
    current->next = q;

    return true;
}

void aggiungi_socio(Socio **archivio){

    printf("Inserimento di un Socio\n");

    Socio *p = (Socio *)malloc(sizeof(Socio));
    p->elenco_vetture = NULL;
    p->next = NULL;

    printf("Cognome: ");
    scanf("%s",p->dati.Cognome);
    printf("Nome: ");
    scanf("%s",p->dati.Nome);
    printf("Codice Fiscale: ");
    scanf("%s",p->dati.Codice_Fiscale);
    printf("Indirizzo e-mail: ");
    scanf("%s",p->dati.Email);

    int n=0;
    printf("Quante vetture vuoi inserire per %s %s?\n",p->dati.Nome,p->dati.Cognome);
    scanf("%d",&n);

    for(int i=0; i<n; i++){
        Vettura *q = (Vettura *)malloc(sizeof(Vettura));
        q->next = NULL;

        printf("Inserimento vettura n.%d\n",(i+1));
        printf("Marca: ");
        scanf("%s",q->Marca);
        printf("Modello: ");
        scanf("%s",q->Modello);
        printf("Anno di produzione: ");
        scanf("%d",&(q->Anno));

        inserimento_lista_vettura(&(p->elenco_vetture),q);
    }

    inserimento_lista_socio(archivio, p);
}

void ricerca_email(Socio *archivio){

    if (archivio==NULL)
        return;

    int anno_inizio, anno_fine;
    char modello[N];

    printf("Inserire la fascia dell'anno nel formato AAAA-AAAA\n");
    scanf("%d-%d",&anno_inizio,&anno_fine);
    printf("Modello: ");
    scanf("%s",modello);

    for (Socio *p=archivio; p!=NULL; p=p->next)
        for (Vettura *q=p->elenco_vetture; q!=NULL; q=q->next){
            if (strcmp(q->Modello,modello)==0 && ((q->Anno>=anno_inizio) && (q->Anno<=anno_fine))){
                printf("Email: %s\n",p->dati.Email);
                break;
            }
        }

}

bool salva(Socio *socio, char file_name[]){

    if (socio == NULL)
        return false;

    FILE *f = NULL;

    f = fopen(file_name,"w");

    if (f==NULL)
        return false;

    for(Socio *p=socio; p!=NULL; p=p->next){
        fprintf(f,"%s\t%s\t%s\t%s\n",p->dati.Cognome,p->dati.Nome,p->dati.Codice_Fiscale,p->dati.Email);
        for(Vettura *q=socio->elenco_vetture; q!=NULL; q=q->next){
            fprintf(f,"\t- %s\t%s\t%d\n",q->Marca,q->Modello,q->Anno);
        }
    }

    fclose(f);
    return true;
}

void salve_socio(Socio *archivio){

    char CF[N];

    printf("Salva socio\nCodice Fiscale: ");
    scanf("%s",CF);

    Socio *p;

    for(p=archivio; p!=NULL && strcmp(p->dati.Codice_Fiscale,CF)!=0; p=p->next);

    if (p==NULL){
        printf("Nessun socio registrato con il CF %s",CF);
        return;
    } else {
        p->next = NULL;
        salva(p,"club.txt");
    }

}

void aggiungi_vettura_socio(Socio **archivio){

    char CF[N];

    printf("Aggiungi Autovettura\nCodice Fiscale: ");
    scanf("%s",CF);

    Socio *p;

    for(p=*archivio; p!=NULL && strcmp(p->dati.Codice_Fiscale,CF)!=0; p=p->next);

    if (p==NULL){
        printf("Nessun socio registrato con il CF %s",CF);
        return;
    } else {
        Vettura *q = (Vettura *)malloc(sizeof(Vettura));
        q->next = NULL;

        printf("Marca: ");
        scanf("%s",q->Marca);
        printf("Modello: ");
        scanf("%s",q->Modello);
        printf("Anno di produzione: ");
        scanf("%d",&(q->Anno));

        inserimento_lista_vettura(&(p->elenco_vetture),q);
    }
}

void stampa(Socio *archivio){

    for(Socio *p=archivio; p!=NULL; p=p->next){
        printf("DATI SOCIO\n%s %s\n",p->dati.Cognome,p->dati.Nome);
        for(Vettura *q=p->elenco_vetture; q!=NULL; q=q->next)
            printf("DATI VETTURA\n%s %s %d\n",q->Marca,q->Modello,q->Anno);
    }

}

int main()
{
    Socio *archivio = NULL;

    int scelta=0;

    do{
        printf("0.Aggiungi Socio\n1.Ricerca\n2.Salva Socio\n3.Aggiungi Vettura\n4.Esci\n: ");
        scanf("%d",&scelta);
       switch (scelta) {
       case 0:
           aggiungi_socio(&archivio);
           break;
       case 1:
           ricerca_email(archivio);
           break;
       case 2:
           salve_socio(archivio);
           break;
       case 3:
           aggiungi_vettura_socio(&archivio);
           break;
       default:
           printf("Azione non valida!");
           break;
       }
    } while (scelta!=4);

    return 0;
}
