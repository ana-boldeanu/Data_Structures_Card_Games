// Boldeanu Ana-Maria
// 311 CD
// SD Tema 1

#include <stdlib.h>
#include <string.h>

#include "darby_structures.h"

// ====================================================
// ================= Functii Folosite =================
// ====================================================

// Functie ce intializeaza proprietatile unei carti de joc
TCard initiate_card(char name, char color, int value) {
    TCard new_card;
    new_card.name = name;
    new_card.color = color;
    new_card.value = value;

    // Pentru scarabei, determin si ce tip are
    if (value >= 12) {
        new_card.scarabei = 1;
    } else if (value == 8) {
        new_card.scarabei = 3;
    } else {
        new_card.scarabei = 2;
    }

    return new_card; 
}

// Functie ce aloca memoria pentru un TNode (carte)
TList create_node(TCard card) {
    TList new_card = (TList) calloc(1, sizeof(TNode));
    new_card->card = card;
    new_card->next = NULL;
    return new_card;
}

// Functie de insert TCard la sfarsitul pachetului
TList add_to_end(TList pack, TCard card) {
    TList new_card = create_node(card);

    // Daca pachetul e gol (pentru prima carte)
    if (pack == NULL) {
        return new_card;
    }

    // Altfel, adaug cartea la sfarsitul pachetului
    TList prev = pack;
    TList last = pack->next;
    while (last != NULL) {
        prev = last;
        last = last->next;
    }
    prev->next = new_card;

    return pack;
}

// Functie de insert TCard la inceputul pachetului
TList add_to_beginning(TList pack, TCard card) {
    TList new_card = create_node(card);

    // Daca pachetul e gol (pentru prima carte)
    if (pack == NULL) {
        return new_card;
    }
    // Altfel, adaug cartea la inceputul pachetului
    new_card->next = pack;

    return new_card;
}

// Functie pt printarea listei
void print_Tlist(TList list, FILE *file) {
    TList pack = list;
    while (pack != NULL) {
        fprintf(file, "%c%c ", pack->card.name, pack->card.color);
        pack = pack->next;
    }
    return;
}

// Functie pentru eliberarea memoriei
void free_Tlist(TList pack) {
    while (pack != NULL) {
        TList aux = pack;
        pack = pack->next;
        free(aux);
    }
    pack = NULL;
    return;
}

// Functii pentru min si max
int min(int x, int y) {
    if (x < y) {
        return x;
    }
    return y;
}
int max(int x, int y) {
    if (x > y) {
        return x;
    }
    return y;
}

// Functie care determina cate carti exista intr-un pachet
int size(TList pack) {
    TList aux = pack;
    int size = 0;
    while (aux != NULL) {
        size++;
        aux = aux->next;
    }
    return size;
}

// Functie care scoate prima carte din pachet si o returneaza prin argument
TList remove_first(TList pack, TCard *removed) {
    *removed = pack->card;
    TList aux = pack;
    pack = pack->next;
    free(aux);
    return pack;
}

// (pt Razboi) Functie care determina cate carti de o anumita valoare sunt intr-un pachet
int nr_ofXCards(TList pack, char type) {
    TList aux = pack;
    int nr = 0;
    while (aux != NULL) {
        if (aux->card.name == type) {
            nr++;
        }
        aux = aux->next;
    }
    return nr;
}

// (pentru Razboi) Functia de razboi la egalitate
void declaration_of_war(TList *pack_1, TList *pack_2, int nr_ofCards, TCard playedCard_1, TCard playedCard_2) {
    TList warCards_1 = NULL, warCards_2 = NULL; // Cartile jucate de fiecare la razboi
    TList player_1 = *pack_1, player_2 = *pack_2;  // Pachetele jucatorilor ca sa nu ma chinui cu dereferentieri
    TCard removed, lastCard_1, lastCard_2;  
    int nr_ofCards_copy = nr_ofCards;

    // Adaug intai cartea de razboi initiala (cea la care a fost egalitatea)
    warCards_1 = add_to_beginning(warCards_1, playedCard_1);
    warCards_2 = add_to_beginning(warCards_2, playedCard_2);

    // Determin ultima carte pentru fiecare jucator si actualizez pachetul de carti jucate (warCards)
    while (nr_ofCards) {
        player_1 = remove_first(player_1, &removed);
        warCards_1 = add_to_beginning(warCards_1, removed);
        nr_ofCards--;
    }
    lastCard_1 = removed;

    while (nr_ofCards_copy) {
        player_2 = remove_first(player_2, &removed);
        warCards_2 = add_to_beginning(warCards_2, removed);
        nr_ofCards_copy--;
    }
    lastCard_2 = removed;

    // Cine a castigat ia toate cartile jucate
    if (lastCard_1.value > lastCard_2.value) {
        TList aux = warCards_1;
        while (aux != NULL) {
            player_1 = add_to_end(player_1, aux->card);
            aux = aux->next;
        }

        aux = warCards_2;
        while (aux != NULL) {
            player_1 = add_to_end(player_1, aux->card);
            aux = aux->next;
        }

    } else {
        TList aux = warCards_2;
        while (aux != NULL) {
            player_2 = add_to_end(player_2, aux->card);
            aux = aux->next;
        }

        aux = warCards_1;
        while (aux != NULL) {
            player_2 = add_to_end(player_2, aux->card);
            aux = aux->next;
        }
    }
    // Actualizez pachetele jucatorilor
    *pack_1 = player_1;
    *pack_2 = player_2;

    free_Tlist(warCards_1);
    free_Tlist(warCards_2);
}

// (pt Razboi) Functie care determina cine castiga dupa 100 de runde
int war_timeLimit(TList player_1, TList player_2, char *winning_card) {
    int winner = 0;
    char card_names[14] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
    int playerScore_1[14], playerScore_2[14];   

    // Construiesc vectorii care retin cate carti de fiecare valoare au jucatorii
    for (int i = 0; i < 14; i++) {
        playerScore_1[i] = nr_ofXCards(player_1, card_names[i]);
        playerScore_2[i] = nr_ofXCards(player_2, card_names[i]);
    }

    // Determin castigatorul in functie de cate carti are
    for (int i = 0; i < 14; i++) {
        if (playerScore_1[i] > playerScore_2[i]) {
            winner = 1;
        } else if (playerScore_2[i] > playerScore_1[i]) {
            winner = 2;
        }
        if (winner) {
            *winning_card = card_names[i];
            break;
        }
    }
    return winner;
}

// (pt Rolling Stone) Functie care verifica daca un jucator are o carte de o anumita culoare
int contains(TList pack, char color) {
    TList aux = pack;
    while (aux != NULL) {
        if (aux->card.color == color) {
            return 1;
        } else {
            aux = aux->next;
        }
    }
    return 0;
}

// (pt Rolling Stone) Functie care scoate o carte de o anumita culoare din pachet 
// (returneaza noul pachet, iar cartea scoasa e retinuta prin adresa)
TList rolling_card(TList pack, char color, TCard *removed) {
    // Daca are doar o carte
    if (pack->next == NULL) {
        *removed = pack->card;
        free(pack);
        return NULL;
    }

    // Daca este la inceput
    if (pack->card.color == color) {
        TList aux = pack;
        *removed = pack->card;
        pack = pack->next;
        free(aux);
        return pack;

    } else {
        TList prev = pack;
        TList curr = pack->next;
        while (curr != NULL) {
            if (curr->card.color == color) {
                *removed = curr->card;
                prev->next = curr->next;
                free(curr);
                return pack;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    return pack;
}

// (pt Rolling Stone) Functie care determina cartea jucata la inceputul rundei (si retine ce culoare a fost)
TCard drop_card(TList *player, char *color, int *idx) {
    TCard *played_card = calloc(1, sizeof(TCard)); // Pointer la cartea jucata
    TCard card_toReturn = {0};  // Cartea jucata efectiv (ca sa fie TCard sa ii dau return)

    if (*player != NULL) {
        int new_idx = *idx;
        if (contains(*player, color[new_idx])) {
            *player = rolling_card(*player, color[new_idx], played_card);

            card_toReturn = *played_card;
            free(played_card);

            return card_toReturn;
        }
        while (1) {
            new_idx++;
            if (new_idx == 4) { // Ca sa cicleze vectorul de culori in caz ca a ajuns la sfarsit
                new_idx = 0;
            }
            if (contains(*player, color[new_idx])) {
                *player = rolling_card(*player, color[new_idx], played_card);

                // Actualizez si indicele curent pentru vectorul de culori
                *idx = new_idx;
                
                card_toReturn = *played_card;
                free(played_card);

                return card_toReturn;
            }
        }
    }
    card_toReturn = *played_card;
    free(played_card);
    
    return card_toReturn;
}

// (pt Scarabei) Functie ce creeaza lista dublu inlantuita cu cei 4 jucatori
CList create_playersList(void) {
    CList node_1 = calloc(1, sizeof(CNode));
    CList node_2 = calloc(1, sizeof(CNode));
    CList node_3 = calloc(1, sizeof(CNode));
    CList node_4 = calloc(1, sizeof(CNode));

    node_1->next = node_2;
    node_2->next = node_3;
    node_3->next = node_4;
    node_4->next = node_1;

    node_1->prev = node_4;
    node_4->prev = node_3;
    node_3->prev = node_2;
    node_2->prev = node_1;

    node_1->player = calloc(1, sizeof(ScNode));
    node_2->player = calloc(1, sizeof(ScNode));
    node_3->player = calloc(1, sizeof(ScNode));
    node_4->player = calloc(1, sizeof(ScNode));

    node_1->player->deck = NULL;
    node_2->player->deck = NULL;
    node_3->player->deck = NULL;
    node_4->player->deck = NULL;

    node_1->player->index = 1;
    node_2->player->index = 2;
    node_3->player->index = 3;
    node_4->player->index = 4;

    return node_1;
}

// (pt Scarabei) Functie de remove player din lista de jucatori
void remove_player(CList *players_list, ScPlayer player) {
    CList prev = (*players_list)->prev;
    CList curr = *players_list;
    do {
        if (curr->player == player) {
            CList aux = curr;

            prev->next = curr->next;
            curr->next->prev = prev;

            free_Tlist(aux->player->deck);
            free(aux->player);
            free(aux);

            return;
        }
        prev = curr;
        curr = curr->next;

    } while (curr != *players_list) ;

    return;
}