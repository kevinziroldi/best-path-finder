// librerie e costanti

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VEICOLI 512
#define INPUT_LEN 20


// strutture dati

typedef struct{
    unsigned int veicoli[MAX_VEICOLI];
    int ultimo_veicolo; // ultima posizione PIENA dell'array
}parco_veicoli_t;

typedef struct STAZIONE{
    unsigned int key; // key = distanza dall'inizio dell'autostrada
    parco_veicoli_t parco_veicoli;
    struct STAZIONE *left, *right, *p;
}stazione_t; // nodo di un albero RB

typedef struct NODO{
    int key;
    struct NODO *next;
    struct NODO *prev;
}nodo_lista_t;


// funzioni

int input(stazione_t**);     // 0 = input finito, 1 = input non finito

// funzioni bst
stazione_t* tree_search(stazione_t*, int);  // ritorna nodo della stazione cercata
stazione_t* tree_successor(stazione_t*);    // ritorna nodo successore
stazione_t* tree_predecessor(stazione_t*);  // ritorna nodo predecessore
stazione_t* tree_minimum(stazione_t*);  // ritorna nodo minimo
stazione_t* tree_maximum(stazione_t*);  // ritorna nodo massimo
stazione_t* elimina_albero(stazione_t*);
stazione_t* tree_insert(stazione_t*, stazione_t*);
stazione_t* tree_delete(stazione_t**, stazione_t*);     // ritorna nodo eliminato

// funzioni liste
nodo_lista_t* inserisci_in_testa(nodo_lista_t*, int);
void stampa_lista_indietro(nodo_lista_t*);
nodo_lista_t* distruggi_lista(nodo_lista_t*);

// funzioni specifiche
stazione_t* aggiungi_stazione(stazione_t*);    // ritorna albero aggiornato
stazione_t* demolisci_stazione(stazione_t*);    // ritorna albero aggiornato
stazione_t* aggiungi_auto(stazione_t*);     // ritorna albero aggiornato
stazione_t* rottama_auto(stazione_t*);      // ritorna albero aggiornato
void pianifica_percorso(stazione_t*);
int max_autonomia(stazione_t*);
stazione_t* prossima_stazione_avanti(stazione_t*, stazione_t*);
stazione_t* prossima_stazione_indietro(stazione_t*, stazione_t*);
nodo_lista_t* gestisci_pari_p_min(stazione_t*, nodo_lista_t*, int*);     // gestisce il caso di sequenze di pari lunghezza, caso partenza < arrivo
nodo_lista_t* gestisci_pari_a_min(stazione_t*, nodo_lista_t*, int*);     // gestisce il caso di sequenze di pari lunghezza, caso arrivo < partanza 


// main

int main(){
    int flag;
    stazione_t* s; // bst.root 
    s = NULL;
    
    flag = 1;
    while(flag){
        flag = input(&s);
    }
    
    s = elimina_albero(s);

    return 0;
}


// definizione funzioni

// input legge una riga di input e chiama la funzione richiesta, non prende alcun argomento
int input(stazione_t** s){

    char comando_input[INPUT_LEN+1];
    
    if(scanf(" %s", comando_input) != EOF){
        if(!strcmp(comando_input, "aggiungi-stazione"))
            *s = aggiungi_stazione(*s);
        else if(!strcmp(comando_input, "demolisci-stazione"))
            *s = demolisci_stazione(*s);
        else if(!strcmp(comando_input, "aggiungi-auto"))
            *s = aggiungi_auto(*s);
        else if(!strcmp(comando_input, "rottama-auto"))
            *s = rottama_auto(*s);
        else if(!strcmp(comando_input, "pianifica-percorso"))
            pianifica_percorso(*s);
        return 1;
    }

    return 0;
}

// funzioni bst

stazione_t* tree_search(stazione_t* s, int k){    
    if(s == NULL || s->key == k)
        return s;
    if(k < s->key)
        return tree_search(s->left, k);
    else
        return tree_search(s->right, k);
}

stazione_t* tree_successor(stazione_t *x){
    stazione_t* y;
    y = NULL;
    if(x->right != NULL)
        return tree_minimum(x->right);
    
    y = x->p;
    while(y != NULL && x == y->right){
        x = y;
        y = y->p;
    }
    return y;
}

stazione_t* tree_predecessor(stazione_t *x){
    stazione_t* y;
    y = NULL;
    if (x->left != NULL)
        return tree_maximum(x->left);
    y = x->p;
    while(y != NULL && x == y->left){
        x = y;
        y = y->p;
    }
    return y;
}

stazione_t* tree_minimum(stazione_t *x){
    while(x->left != NULL)
        x = x->left;
    return x;
}

stazione_t* tree_maximum(stazione_t* x){
    while(x->right != NULL)
        x = x->right;
    return x;
}

stazione_t* elimina_albero(stazione_t* x){
    if(x != NULL){
        elimina_albero(x->left);
        elimina_albero(x->right);
        free(x);
        x = NULL;
    }
    return NULL;
}

stazione_t* tree_insert(stazione_t*t, stazione_t*z){
    stazione_t *x, *y;
    y = NULL;
    x = t;
    while(x != NULL){
        y = x;
        if(z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->p = y;
    if(y == NULL)
        t = z;
    else if(z->key < y->key)
        y->left = z;
    else
        y->right = z;
    
    return t;
}

stazione_t* tree_delete(stazione_t**p, stazione_t*z){
    
    stazione_t *x, *y;
    
    x = NULL;
    y = NULL;

    if(z->left == NULL || z->right == NULL)
       y = z;
    else
        y = tree_successor(z);
    
    if(y->left != NULL)
        x = y->left;
    else
        x = y->right;
    
    if(x != NULL)
        x->p = y->p;
    if(y->p == NULL)
        *p = x;
    else if(y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;
    
    if(y != z){
        z->key = y->key;
        z->parco_veicoli = y->parco_veicoli;
    }
    
    return y;
}

// funzioni liste

nodo_lista_t* inserisci_in_testa(nodo_lista_t *l, int dist){
    nodo_lista_t *x;
    x = NULL;
    x = malloc(sizeof(nodo_lista_t));
    if(x){
        x->key = dist;
        x->prev = NULL;
        if(l!=NULL){
            x->next = l;
            l->prev = x;
            l = x;
        }
        else{
            x->next = NULL;
            l = x;
        }
    }
    return l;
}

void stampa_lista_indietro(nodo_lista_t *l){
    while(l->next != NULL)
        l = l->next;
    while(l->prev != NULL){
        printf("%d ", l->key);
        l = l->prev;
    }
    printf("%d\n", l->key);
}

nodo_lista_t* distruggi_lista(nodo_lista_t* l){
    nodo_lista_t* x;
    while(l != NULL){
        x = l;
        l = l->next;
        free(x);
    }
    return NULL;
}

// funzioni specifiche

// crea un nodo e lo inserisce nel BST
stazione_t* aggiungi_stazione(stazione_t* s){

    int distanza, numero_auto, i, autonomia;
    stazione_t* new_s;
    //stazione_t *x;
    new_s = NULL;
    
    //x = NULL;

    if(scanf("%d", &distanza)){

        if(tree_search(s, distanza) == NULL){   // il nodo non è presente
            new_s = malloc(sizeof(stazione_t));
            if (new_s){
                // inserisco i dati
                new_s->key = distanza;
                if(scanf("%d", &numero_auto)){
                    new_s->parco_veicoli.ultimo_veicolo = numero_auto - 1;
                    for(i = 0; i < numero_auto; i++)
                        if(scanf("%d", &autonomia))
                            new_s->parco_veicoli.veicoli[i] = autonomia;
                }
                new_s->p = NULL;
                new_s->left = NULL;
                new_s->right = NULL;
                s = tree_insert(s, new_s);
                printf("aggiunta\n");
            }
            else
                printf("non aggiunta\n");
        }
        else{
            // parte aggiunta da qui...
            if(scanf("%d", &numero_auto)){
                for(i = 0; i < numero_auto; i++)
                    if(scanf("%d", &autonomia));
            }
            // ... a qui
            printf("non aggiunta\n");
        }
    }
    return s;
}

stazione_t* demolisci_stazione(stazione_t* s){
    int distanza;
    stazione_t* x;
    stazione_t* y;

    x = NULL;
    y = NULL;

    if(scanf("%d", &distanza)){
        x = tree_search(s, distanza);
        if (x != NULL){
            y = tree_delete(&s, x);
            free(y);
            y = NULL;
            printf("demolita\n");
        }
        else
            printf("non demolita\n");
    }
    return s;
}

// prende in input la stazione a cui inserire l'auto
// aggiunge l'auto se ci sono meno di 512 auto, incremento "ultimo_veicolo"
stazione_t* aggiungi_auto(stazione_t* s){    
    int distanza, autonomia;
    stazione_t* x;
    
    x = NULL;

    if(scanf("%d", &distanza)){
        if(scanf("%d", &autonomia)){
            x = tree_search(s, distanza);
            if(x != NULL){     // la stazione esiste
                if(x->parco_veicoli.ultimo_veicolo < MAX_VEICOLI-1){    // 0 - 511
                    // -1 perchè devo avere lo spazio per inserire
                    x->parco_veicoli.ultimo_veicolo = x->parco_veicoli.ultimo_veicolo + 1;
                    x->parco_veicoli.veicoli[x->parco_veicoli.ultimo_veicolo] = autonomia;
                    printf("aggiunta\n");
                }
                else
                    printf("non aggiunta\n");
            }
            else
                printf("non aggiunta\n");
        }
    }
    return s;
}

stazione_t* rottama_auto(stazione_t* s){
    // dato che si tratta di un array, non devo effettivamente deallocare memoria
    // cerco l'indice dell'elemento da eliminare 
    // se tale elemento è presente, copio i km di ultimo_veicolo nella pos di quello da eliminare
    // decremento ultimo_veicolo
    // così tengo array compatto

    int distanza, autonomia, i, found;
    stazione_t* x;

    if (scanf("%d", &distanza)){
        if(scanf("%d", &autonomia)){
            
            x = tree_search(s, distanza);
            if(x != NULL && x->parco_veicoli.ultimo_veicolo >= 0){    // stazione esistente
                
                // cerco la macchina 
                found = 0;
                i = 0;
                while(i <= x->parco_veicoli.ultimo_veicolo && !found){
                    if(x->parco_veicoli.veicoli[i] == autonomia)
                        found = 1;
                    else
                        i++;
                }
                if(found){  // ho trovato la macchina ed è in posizione i (non incremento i se metto found=1)
                    x->parco_veicoli.veicoli[i] = x->parco_veicoli.veicoli[x->parco_veicoli.ultimo_veicolo];
                    x->parco_veicoli.ultimo_veicolo = x->parco_veicoli.ultimo_veicolo - 1;                    
                    printf("rottamata\n");
                }
                else{
                    printf("non rottamata\n");
                }
            }
            else{
                printf("non rottamata\n");
            }
        }
    }
    return s;
}

void pianifica_percorso(stazione_t* s){

    int dist_partenza, dist_arrivo, found, flag;
    stazione_t* ultima;
    nodo_lista_t *l;

    l = NULL;
    found = 0;
    ultima = NULL; // rappresenta l'ultima stazione dove ho fatto sosta
    flag = 1;

    if(scanf("%d", &dist_partenza) && scanf("%d", &dist_arrivo)){

        if(dist_partenza == dist_arrivo){
            printf("%d\n", dist_partenza);
        }
        else if(dist_partenza < dist_arrivo){

            ultima = tree_search(s, dist_partenza);     // la prima stazione è quella di partenza
            l = inserisci_in_testa(l, ultima->key);     // inserisco stazione partenza

            while(!found && ultima != NULL){
                if(max_autonomia(ultima) >= dist_arrivo - ultima->key){
                    found = 1;
                }
                else{
                    ultima = prossima_stazione_avanti(s, ultima);
                    if (ultima != NULL){
                        l = inserisci_in_testa(l, ultima->key);
                        //l = Insintesta(l, ultima->key);
                    }
                }
            }

            // stampo risultato
            if(found){
                l = inserisci_in_testa(l, dist_arrivo);
                flag = 1;
                while(flag)
                    l = gestisci_pari_p_min(s, l, &flag);
                stampa_lista_indietro(l);
            }
            else
            {
                printf("nessun percorso\n");
            }
        }
        else{   // dist_partenza > dist_arrivo

            ultima = tree_search(s, dist_partenza);
            l = inserisci_in_testa(l , dist_partenza);

            while(!found && ultima != NULL){
                if(max_autonomia(ultima) >= ultima->key - dist_arrivo)
                    found = 1;
                else{
                    ultima = prossima_stazione_indietro(s, ultima);
                    if (ultima != NULL){
                        l = inserisci_in_testa(l, ultima->key);
                        //l = Insintesta(l, ultima->key);
                    }
                }
            }

            // stampo risultato
            if(found){
                l = inserisci_in_testa(l, dist_arrivo);
                flag = 1;
                while(flag)
                {
                    l = gestisci_pari_a_min(s, l, &flag);
                }
                stampa_lista_indietro(l);
            }
            else{
                printf("nessun percorso\n");
            }
        }
        
        l = distruggi_lista(l);
    }
}

// trova la macchina con massima autonomia in una certa stazione
int max_autonomia(stazione_t *x){
    int i, max;
    max = 0;
    for(i = 0; i <= x->parco_veicoli.ultimo_veicolo; i++){
        if (x->parco_veicoli.veicoli[i] > max)
            max = x->parco_veicoli.veicoli[i];
    }
    return max;
}

// trova la stazione successiva a cui fermarsi, nel caso dist_partenza < dist_arrivo
stazione_t* prossima_stazione_avanti(stazione_t *s, stazione_t *ultima){
    
    int tot, max_tot, aut;
    stazione_t *x, *x_max;
    
    aut = max_autonomia(ultima);
    max_tot = aut;
    x_max = ultima;
    x = tree_successor(ultima);

    while(x != NULL && aut >= x->key - ultima->key){     // fichè x è raggiungibile
        tot = x->key - ultima->key + max_autonomia(x);
        if(tot > max_tot){
            x_max = x;
            max_tot = tot;
        }
        x = tree_successor(x);
    }

    if (x_max != ultima){
        return x_max;
    }
    else{
        return NULL;
    }

}

// trova la stazione successiva a cui fermarsi, nel caso dist_partenza > dist_arrivo
stazione_t* prossima_stazione_indietro(stazione_t *s, stazione_t *ultima){
    int tot, max_tot, aut;
    stazione_t *x, *x_max;
    
    aut = max_autonomia(ultima);
    max_tot = aut;
    x_max = ultima;
    x = tree_predecessor(ultima);

    while(x != NULL && aut >= ultima->key - x->key){     // fichè x è raggiungibile
        tot = ultima->key - x->key + max_autonomia(x);
        if(tot > max_tot){
            x_max = x;
            max_tot = tot;
        }
        x = tree_predecessor(x);
    }

    if (x_max != ultima)
    {
        return x_max;
    }
    else
    {
        return NULL;
    }
}

// es 500 - 400 - 300 -180 -150 - 100, dist_arrivo = 500
// ogni volta mi sposto di uno a dx e verifco se esiste un predecessore che permette di arriva in quello a sx dei due
// nodo a sx x, nodo a dx y
nodo_lista_t* gestisci_pari_p_min(stazione_t*s, nodo_lista_t*l, int* flag){
    // flag inizializzato a 0, se effettuo almeno una modifica allora lo imposto a 1
    nodo_lista_t *x, *y;
    stazione_t* pred;
    
    x = l;
    y = l->next;
    pred = NULL;
    *flag = 0;

    // la lista ha almeno 2 nodi
    while(y->next != NULL){

        // verifico se posso cambiare y
        pred = tree_predecessor(tree_search(s, y->key));
        while(pred != NULL && pred->key != y->next->key){
            if (max_autonomia(pred) >= x->key - pred->key){
                y->key = pred->key;
                *flag = 1;
            }
            pred = tree_predecessor(pred);
        }   

        x = y;
        y = y->next;
    }
    return l;
}

// es 120 - 140 - 200 - 300 - 560, dist_arrivo = 120
nodo_lista_t* gestisci_pari_a_min(stazione_t*s, nodo_lista_t*l, int *flag){
    
    // non si può fare come sopra
    // qui ogni volta cerco di avvicinarmi di più alla fine, non per forza se ci arrivavo con il precedente, allora ci arrivo con un predecessor
    
    nodo_lista_t *x, *y;
    stazione_t *pred;
    
    x = l;
    y = l->next;
    pred = NULL;
    *flag = 0;

    while(y->next != NULL){
        pred = tree_predecessor(tree_search(s, y->key));

        while(pred != NULL && pred->key != x->key){
            if(max_autonomia(pred) >= pred->key - x->key && max_autonomia(tree_search(s, y->next->key)) >= y->next->key - pred->key)
            {
                y->key = pred->key;
                *flag = 1;
            }
            pred = tree_predecessor(pred);
        }

        x = y;
        y = y->next;
    }
    return l;
}
