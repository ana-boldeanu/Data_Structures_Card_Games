// Boldeanu Ana-Maria
// 311 CD
// SD Tema 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darby_functions.h"

#define STRING_MAX 50
#define MAX_CARDS 52

// Functie pentru generarea pachetului initial
TList create_initial_pack(void) {
    // Vectori cu proprietatile cartilor (de ex. numele si valoarea pentru As se gasesc la card_names[0] si card_values[0])
    char card_names[14] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};
    int card_values[14] = {15, 14, 13, 12, 10, 9, 8, 7, 6, 5, 4, 3, 2};
    char card_colors[4] = {'s', 'h', 'c', 'd'};

    // Generez pachetul initial (neamestecat)
    TList initial_pack = NULL;

    for (int n = 0; n < 13; n++) {
        for (int c = 0; c < 4; c++) {
            TCard new_card = initiate_card(card_names[n], card_colors[c], card_values[n]); // Aloc cate o carte
            if (initial_pack == NULL) {
                initial_pack = create_node(new_card);
            } else {
                initial_pack = add_to_end(initial_pack, new_card);
            }
        }
    }
    return initial_pack;
}

// (pt Amestecare) Inversare
TList inversare(TList pack) {
    TList aux = pack, new_pack = NULL; // Construiesc un pachet nou cu ordinea cartilor inversa

    while (aux != NULL) {
        new_pack = add_to_beginning(new_pack, aux->card);
        aux = aux->next;
    }
    free_Tlist(pack);
    
    return new_pack;
}

// (pt Amestecare) Npiles
TList npiles(TList pack, int nr_ofPacks, int *order) {
    TList aux = pack, new_pack = NULL; // Construiesc pachetul nou in new_pack

    int nr_ofCardsPerPack = MAX_CARDS/nr_ofPacks;
    int added = 0, idx_ofPack = 1;
    TList subpack[MAX_CARDS] = {0};

    // Creez subpachetele
    while (aux != NULL) {
        if (added != nr_ofCardsPerPack) {
            subpack[idx_ofPack] = add_to_end(subpack[idx_ofPack], aux->card);
            added++;
        } else {
            idx_ofPack++;   // idx_ofPack creste daca s-a ajuns la numarul de carti cerut pt subpachetul curent
            subpack[idx_ofPack] = add_to_end(subpack[idx_ofPack], aux->card);
            added = 1;
        }
        aux = aux->next;
    }
    free_Tlist(pack);

    // Pun pachetele inapoi in ordinea indecsilor ceruta
    for (int i = 0; i < idx_ofPack; i++) {
        TList tmp = subpack[order[i]];
        while (tmp != NULL) {
            new_pack = add_to_end(new_pack, tmp->card);
            tmp = tmp->next;
        }
        free_Tlist(subpack[order[i]]);
    }

    return new_pack;
}

// (pt Amestecare) Intercalare
TList intercalare(TList pack) {
    TList aux = pack, new_pack = NULL;  // In new_pack construiesc pachetul final, amestecat
    TList subpack1 = NULL, subpack2 = NULL;

    // Construiesc subpachetele
    for (int i = 0; i < MAX_CARDS/2; i++) { 
        subpack1 = add_to_end(subpack1, aux->card);
        aux = aux->next;
    }
    while (aux != NULL) { 
        subpack2 = add_to_end(subpack2, aux->card);
        aux = aux->next;
    }
    free_Tlist(pack);
    
    // Adaug alternativ cartile din subpachete
    TList aux1 = subpack1, aux2 = subpack2;
    while (aux1 != NULL && aux2 != NULL) { 
        new_pack = add_to_end(new_pack, aux1->card);
        new_pack = add_to_end(new_pack, aux2->card);
        aux1 = aux1->next;
        aux2 = aux2->next;
    }
    free_Tlist(subpack1);
    free_Tlist(subpack2);

    return new_pack;
}

// (pt Amestecare) Mongean
TList mongean(TList pack) {
    TList aux = pack, new_pack = NULL;  // In new_pack construiesc pachetul final
    
    // Alternant, adaug o carte la sfarsit si una la inceput
    while (aux != NULL) {
        new_pack = add_to_end(new_pack, aux->card);
        aux = aux->next;
        if (aux != NULL) {
            new_pack = add_to_beginning(new_pack, aux->card);
            aux = aux->next;
        }
    }
    free_Tlist(pack);

    return new_pack;
}

// Functie pentru Task 1 - Amestecarea Cartilor
TList shuffle(FILE *in, FILE *out, TList pack) {
    int nr_amestecari;
    char *tip_amestecare = calloc(STRING_MAX, sizeof(char));

    fscanf(in, "%d\n", &nr_amestecari);
    while (nr_amestecari) {
        fscanf(in, "%s\n", tip_amestecare);
        nr_amestecari--;

        if (!strcmp(tip_amestecare, "inversare")) {
            pack = inversare(pack);

        } else if (!strcmp(tip_amestecare, "intercalare")) {
            pack = intercalare(pack);

        } else if (!strcmp(tip_amestecare, "mongean")) {
            pack = mongean(pack);

        } else if (strstr(tip_amestecare, "npiles")) {
            int nr_ofPacks, order[STRING_MAX];

            fscanf(in, "%d ", &nr_ofPacks);
            for (int i = 0; i < nr_ofPacks; i++) {
                fscanf(in, "%d ", &(order[i]));
            }

            pack = npiles(pack, nr_ofPacks, order);
        }
    }
    free(tip_amestecare);

    return pack;
}

// Functie pentru Task 2 - Razboi
void razboi(TList pack, FILE *out) {
    TList aux = pack;
    TList player_1 = NULL, player_2 = NULL; // Pachetele celor 2 jucatori
    char winning_card = 0;
    
    // Impart cartile celor 2 jucatori
    int nr_ofCards = MAX_CARDS/2;
    int added = 0;

    while (aux != NULL) {
        if (added != nr_ofCards) {
            player_1 = add_to_end(player_1, aux->card);
            added++;
        } else {
            player_2 = add_to_end(player_2, aux->card);
        }
        aux = aux->next;
    }
    free_Tlist(pack);

    // Desfasurarea jocului
    TCard playedCard_1, playedCard_2 = {0};   // Cartea intoarsa in tura curenta
    int winner = 0;
    int nr_ofTurns = 0; // Numarul de runde jucate

    while (winner == 0) {
        // Verific daca vreun jucator a ramas fara carti
        if (player_1 == NULL) {
            winner = 2;
            break;
        }
        if (player_2 == NULL) {
            winner = 1;
            break;
        }
        // Fiecare jucator intoarce prima carte din pachetul sau
        playedCard_1 = player_1->card;
        aux = player_1;
        player_1 = player_1->next;
        free(aux);

        playedCard_2 = player_2->card;
        aux = player_2;
        player_2 = player_2->next;
        free(aux);

        // Cartea cu valoarea mai mare castiga
        if (playedCard_1.value > playedCard_2.value) {
            player_1 = add_to_end(player_1, playedCard_1);
            player_1 = add_to_end(player_1, playedCard_2);

        } else if (playedCard_2.value > playedCard_1.value) {
            player_2 = add_to_end(player_2, playedCard_2);
            player_2 = add_to_end(player_2, playedCard_1);

        } else {
            // La egalitate determin cate carti se vor juca la razboi (nr_warCards)
            int nr_warCards, nrCards_1, nrCards_2;
            TCard warCard = playedCard_1;   // Cartea de razboi
            nrCards_1 = size(player_1); // Cate carti are jucatorul 1
            nrCards_2 = size(player_2);

            nr_warCards = min(min(nrCards_1, warCard.value), min(nrCards_2, warCard.value));
            declaration_of_war(&player_1, &player_2, nr_warCards, playedCard_1, playedCard_2);
        }
        nr_ofTurns++;

        // Daca s-au jucat 100 de maini
        if (nr_ofTurns == 100) {
            winner = war_timeLimit(player_1, player_2, &winning_card);
        }
    }
    // Intorc rezultatul
    fprintf(out, "%d", winner);
    if (nr_ofTurns == 100) {
        fprintf(out, " %c", winning_card);
    }

    free_Tlist(player_1);
    free_Tlist(player_2);
    return;
}

// Functie pentru Task 3 - Rolling Stone
int rollingstone(TList pack) {
    TList aux = pack;
    TList player[4] = {0};  // Vector cu pachetele celor 4 jucatori

    // Impart cartile jucatorilor
    int nr_ofCards = MAX_CARDS/4;
    int added = 0, idx_ofPlayer = 0;
    
    while (aux != NULL) {
        if (added != nr_ofCards) {
            player[idx_ofPlayer] = add_to_end(player[idx_ofPlayer], aux->card);
            added++;

        } else {
            idx_ofPlayer++;
            player[idx_ofPlayer] = add_to_end(player[idx_ofPlayer], aux->card);
            added = 1;
        }
        aux = aux->next;
    }
    free_Tlist(pack);

    // Desfasurarea jocului
    TCard *played_card = calloc(1, sizeof(TCard)); // Cartea pusa pe masa
    TList discard_pile = NULL;  // Pachetul de carti care se afla pe masa la runda curenta
    char color[4] = {'s', 'h', 'c', 'd'}; // Pentru ordinea culorilor
    char played_color;  // Culoarea jucata la runda curenta
    int idx_ofColor = 0, winner = 0; idx_ofPlayer = 0;

    while (winner == 0) {

        // Verificare daca a castigat cineva
        for (int i = 0; i < 4; i++) {
            if (player[i] == NULL) {
                winner = i + 1;
                break;
            }
        }

        // Daca sunt la inceputul unei runde
        if (discard_pile == NULL) {
            // Actualizez cartea jucata
            *played_card = drop_card(&(player[idx_ofPlayer]), color, &idx_ofColor);
            played_color = (*played_card).color;
            discard_pile = add_to_end(discard_pile, *played_card);

            // Trec la urmatorul jucator
            idx_ofPlayer++;
            if (idx_ofPlayer == 4) { // Ca sa cicleze
                idx_ofPlayer = 0;
            }
        }

        // Parcursul rundei: Daca are cartea, o joaca
        if (contains(player[idx_ofPlayer], played_color)) {
            player[idx_ofPlayer] = rolling_card(player[idx_ofPlayer], played_color, played_card);
            discard_pile = add_to_end(discard_pile, *played_card);

            // Verific daca a castigat jucatorul curent
            if (player[idx_ofPlayer] == NULL) {
                winner = idx_ofPlayer + 1;
                break;
            }
            // Trec la urmatorul jucator
            idx_ofPlayer++;
            if (idx_ofPlayer == 4) {    // Ca sa cicleze
                idx_ofPlayer = 0;
            }

        // Altfel, le ia pe cele jucate pana acum si se trece la urmatoarea culoare
        } else {
            aux = discard_pile;
            while (aux != NULL) {
                player[idx_ofPlayer] = add_to_end(player[idx_ofPlayer], aux->card);
                aux = aux->next;
            }
            free_Tlist(discard_pile);
            discard_pile = NULL;

            // Determin urmatoarea culoare
            idx_ofColor++;
            if (idx_ofColor == 4) { // Ca sa cicleze
                idx_ofColor = 0;
            }
        }
    }
    free_Tlist(discard_pile);
    for (int i = 0; i < 4; i++) {
        free_Tlist(player[i]);
    }
    free(played_card);

    return winner;
}

// Functie pentru Task 4 - Scarabeii Egipteni
int scarabei(TList pack, FILE *out) {
    // Creez lista de jucatori
    CList playlist = create_playersList();

    // Impart cartile celor 4 jucatori
    TList aux_pack = pack;
    int nr_ofCards = MAX_CARDS/4;
    int added = 0;  // Numarul de carti impartite

    while (aux_pack != NULL) {
        if (added != nr_ofCards) {
            playlist->player->deck = add_to_end(playlist->player->deck, aux_pack->card);
            added++;

        } else { 
            playlist = playlist->next; // Daca s-au impartit cele 13 carti, trec la urmatorul
            playlist->player->deck = add_to_end(playlist->player->deck, aux_pack->card);
            added = 1;
        }
        aux_pack = aux_pack->next;
    }
    free_Tlist(pack);
    playlist = playlist->next; // Ca sa fie din nou la primul jucator
   
    // Desfasurarea jocului
    int winner = 0;
    int direction = 1; // Directia curenta a jocului
    int drawn_scarab = 0; // Retine daca s-a intors un scarabeu sau nu inca
    TCard curr_card = {0};  // Cartea jucata la runda curenta
    TList table_deck = NULL;  // Pachetul de carti care s-au intors pana acum
    ScPlayer *ptr_toLastScarab; // Adresa jucatorului care a intors ultimul scarabeu

    while (winner == 0) {
        // Jucatorul la rand intoarce cartea de deasupra pachetului sau si o pune pe masa
        playlist->player->deck = remove_first(playlist->player->deck, &curr_card);
        table_deck = add_to_beginning(table_deck, curr_card);

        // Daca inaintea lui nu s-a jucat nimic sau nu s-a jucat inca un scarabeu
        if (size(table_deck) == 1 || drawn_scarab == 0) {

            // Daca intoarce o carte speciala, se inverseaza sensul jocului
            if (curr_card.scarabei == 3) {
                if (direction == 1) {
                    direction = -1;
                } else {
                    direction = 1;
                }
            }

            // Daca intoarce un scarabeu, retin ca a fost ultimul care a intors scarabeu
            if (curr_card.scarabei == 1) {
                ptr_toLastScarab = &(playlist->player);
                drawn_scarab = 1;
            }

            // Se trece la urmatorul jucator
            if (direction == 1) {
                playlist = playlist->next;
            } else {
                playlist = playlist->prev;
            }

            // Daca jucatorul anterior a intors ultima lui carte si era 8 sau un numar, acum il elimin din joc
            if (curr_card.scarabei != 1 && direction == 1 && size(playlist->prev->player->deck) == 0) {
                remove_player(&playlist, playlist->prev->player);
            }
            if (curr_card.scarabei != 1 && direction == -1 && size(playlist->next->player->deck) == 0) {
                remove_player(&playlist, playlist->next->player);
            }

        // Altfel, daca inaintea lui s-a jucat un scarabeu
        } else if (drawn_scarab == 1) {

            // Daca intoarce un scarabeu
            if (curr_card.scarabei == 1) {
                // E ultimul care a intors scarabeu
                ptr_toLastScarab = &(playlist->player);

                // Daca jucatorul din urma a tras ultima lui carte tura trecuta, acum a pierdut
                if (direction == 1 && size(playlist->prev->player->deck) == 0) {
                    remove_player(&playlist, playlist->prev->player);
                } else if (direction == -1 && size(playlist->next->player->deck) == 0) {
                    remove_player(&playlist, playlist->next->player);
                }

                // Se trece la urmatorul jucator
                if (direction == 1) {
                    playlist = playlist->next;
                } else {
                    playlist = playlist->prev;
                }
            }

            // Daca intoarce un 8
            if (curr_card.scarabei == 3) {

                // Se trece la urmatorul jucator
                if (direction == 1) {
                    playlist = playlist->next;
                } else {
                    playlist = playlist->prev;
                }

                // Daca cel care a intors 8 a tras ultima lui carte, a pierdut
                if (direction == 1 && size(playlist->prev->player->deck) == 0) {
                    remove_player(&playlist, playlist->prev->player);
                }
                if (direction == -1 && size(playlist->next->player->deck) == 0) {
                    remove_player(&playlist, playlist->next->player);
                }
            }

            // Daca intoarce un numar
            if (curr_card.scarabei == 2) {
                ScPlayer *prev_player = &(playlist->player); // Retin adresa jucatorului curent in caz ca a pierdut runda asta

                // Trec la jucatorul urmator (cel care va castiga cartile / care a tras ultimul scarabeu)
                while (&(playlist->player) != ptr_toLastScarab) {
                    if (direction == 1) {
                        playlist = playlist->prev;
                    } else {
                        playlist = playlist->next;
                    }
                }

                // Acesta ia toate cartile de jos si se trece la urmatoarea runda
                TList aux = table_deck;
                while (aux != NULL) {
                    playlist->player->deck = add_to_end(playlist->player->deck, aux->card);
                    aux = aux->next;
                }
                free_Tlist(table_deck); table_deck = NULL;
                drawn_scarab = 0;

                // Daca cel care a intors un numar a tras ultima lui carte, a pierdut
                if (size((*prev_player)->deck) == 0) {
                    remove_player(&playlist, *prev_player);
                }
            }
            // Verific daca a castigat cineva (daca are 52 de carti)
            CList curr_player = playlist;
            do {
                if (size(curr_player->player->deck) == MAX_CARDS) {
                    winner = curr_player->player->index;

                    free_Tlist(curr_player->player->deck);
                    free(curr_player->player);
                    free(curr_player);
                    break;
                }
                curr_player = curr_player->next;

            } while (curr_player != playlist) ;          
        }
    }

    return winner; // Ma mir ca a reusit
}


// Main
int main() {
    // Generez pachetul initial
    TList initial_pack = create_initial_pack();
    TList pack = NULL;

    // Date pentru testari
    FILE *in, *out;
    in = fopen("in", "r");
    out = fopen("out", "w");

    char *task = calloc(STRING_MAX, sizeof(char));

    fscanf(in, "%s\n", task);

    if (!strcmp(task, "amestecare")) {
        pack = shuffle(in, out, initial_pack);
        print_Tlist(pack, out);
        free_Tlist(pack);

    } else if (!strcmp(task, "rollingstone")) {
        pack = shuffle(in, out, initial_pack);
        int winner = rollingstone(pack);
        fprintf(out, "%d", winner);

    } else if (!strcmp(task, "razboi")) {
        pack = shuffle(in, out, initial_pack);
        razboi(pack, out);

    } else if (!strcmp(task, "scarabei")) {
        pack = shuffle(in, out, initial_pack); 
        int winner = scarabei(pack, out);
        fprintf(out, "%d", winner);
    }

    fclose(in);
    fclose(out);
    free(task);
    
    return 0;
}