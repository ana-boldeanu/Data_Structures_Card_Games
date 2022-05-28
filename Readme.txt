// Boldeanu Ana-Maria
// 311 CD
// SD Tema 1

======================== Reguli Makefile =======================

make (evident, face build and run)

make mem-test (pentru testarea memoriei cu valgrind, dar merge doar daca exista 
	un fisier "in" in folderul in care se afla)


=========================== Continut ===========================

Nota: Sunt comentarii si in cod cu ce face fiecare functie si ce reprezinta
      variabilele folosite. Aici mai mult o sa povestesc, cred


"darby_structures.h" - Header in care am definit structurile folosite:

   -> TCard (struct) - O carte de joc, cu campuri pentru nume, culoare, valoare
	si tipul de carte pentru jocul Scarabeii Egipteni.

   -> TList (pointer la struct de tip TNode) - Lista simplu inlantuita. Un TNode
	contine o carte de joc TCard si un pointer la urmatorul nod din lista.

   -> ScPlayer (pointer la struct de tip ScNode) - Structura folosita doar la 
	jocul Scarabeii Egipteni. ScPlayer reprezinta un jucator, contine un 
	TList (pachetul jucatorului) si indexul jucatorului.

   -> CList (pointer la struct de tip CNode) - Structura folosita doar la jocul
	Scarabeii Egipteni. E lista dublu inlantuita circulara, folosita pentru 
	stocarea jucatorilor. Contine un ScPlayer si pointeri la jucatorul anterior
	si cel urmator.

"darby_functions.h" - Header cu toate functiile "auxiliare" folosite. 
	Doar main(), create_initial_pack(), shuffle(), razboi(), rollingstone() si 
	scarabei() se afla in "darby.c". Restul sunt in header.

"darby.c" - Sursa principala. Mai jos sunt functiile in ordinea folosirii.

   -> create_initial_pack() - Genereaza pachetul initial de carti (TList).
	Pentru aceasta, am folosit functii auxiliare din "darby_functions.h":
	initiate_card(), create_node() si add_to_end().

   -> main() - Aici creez pachetul de carti initial si citesc datele de intrare.
	In functie taskul dat de fisierul "in", folosesc urmatoarele functii:

=============== Task 1 - Amestecare ===============

   -> shuffle() - Functia principala pentru Amestecarea cartilor.
	In functie de tipul de amestecare cerut, chem functiile auxiliare
	inversare(), npiles(), intercalare() si mongean().

   -> TList inversare(TList pack);
	- Primeste pachetul de carti pack si genereaza pachetul inversat new_pack
	prin adaugarea la inceput add_to_beginning() a fiecarei carti din pack, in
	ordinea in care apar (de la inceput la sfarsit).

   -> TList npiles(TList pack, int nr_ofPacks, int *order);
	- Am un vector de subpachete TList subpack[], in el construiesc fiecare
	subpachet, apoi folosesc subpack[order[i]] ca sa adaug pachetele intr-unul
	nou in ordinea ceruta.

   -> TList intercalare(TList pack);
	- Creez intai cele 2 subpachete, apoi adaug cartile in new_pack in mod
	alternant.

   -> TList mongean(TList pack);
	- Tot in mod alternant, ma folosesc de cele 2 functii add_to_end() si 
	add_to_beginning() ca sa adaug cartile in ordinea ceruta.

================= Task 2 - Razboi =================

   -> void razboi(TList pack, FILE *out) - Functia principala pentru Razboi.
	- Foloseste urmatoarele functii auxiliare din "darby_functions.h":

   -> void declaration_of_war(TList *player1, TList *player2, int nr, TCard warCard);
	- Pachetele jucatorilor sunt date prin adresa ca sa le actualizeze automat.
	
	-> TList remove_first(TList pack, TCard *removed); auxiliara
	- Scoate prima carte din pachetul primit si in plus folosesc pointer la
	TCard removed ca sa retin si ce carte a fost scoasa.

	- Folosesc add_to_beginning() ca sa simulez cartile puse pe masa. In plus,
	la final cand adaug cartile celui care le-a castigat, e de ajuns sa le iau
	in ordine de la prima carte si sa folosesc add_to_end().

   -> int war_timeLimit(TList player_1, TList player_2, &winning_card);
	- Functie care determina cine castiga dupa 100 de runde si care a fost
	cartea castigatoare, foloseste nr_ofXCards() ca sa determine cate carti
	de fiecare tip are un jucator

============== Task 3 - Rolling Stone ==============

   -> int rollingstone(TList pack); - Functia principala pentru Rolling Stone.
	- Pentru cei 4 jucatori am folosit un vector de TList.
	- Pentru ordinea culorilor am folosit vectorul char color[4].
	- TCard *played_card e cartea jucata la tura curenta, aveam nevoie sa fie
	pointer pentru o functie ce urmeaza.

	-> TCard drop_card(TList *player, char *color, int *idx_ofColor);
	- Cauta in pachetul jucatorului player cartea cu idx_ofColor cerut si o
	returneaza. Practic determina cartea jucata in runda curenta.
	- *player e pointer ca sa actualizez pachetul jucatorlui
	- Ii dau vectorul color ca sa treaca la urmatoarea culoare in caz ca nu o
	are pe cea ceruta, si *idx_ofColor e pointer ca sa se actualizeze culoarea
	jucata in cazul acesta.

	-> Foloseste rolling_card() ca sa elimine efectiv cartea cu culoarea ceruta,
	daca jucatorul o are. Aceasta verificare se face cu contains().


============ Task 3 - Scarabeii Egipteni ============

   - Majoritatea explicatiilor pentru logica jocului sunt in comentarii pe cod.

   - Pentru cele 3 tipuri de carti, exista campul int scarabei; in structura TCard.
	curr_card.scarabei = 1 inseamna scarabeu
	curr_card.scarabei = 2 inseamna numar
	curr_card.scarabei = 3 inseamna carte speciala

   - Am folosit lista circulara dublu inlantuita pentru cei 4 jucatori. La randul
	lor, jucatorii au o structura speciala ce contine un TList (pachetul) si
	indexul lor (asta ca sa pot sa determin cine a castigat).

   - Lista de jucatori am creat-o direct in functia create_playersList(void)
	- Am alocat cele 4 noduri, am stabilit legaturile, am alocat fiecare
	node->player (adica structurile pentru jucatori).
	- Am setat node->player->deck la NULL ca sa se considere gol pachetul si
	tot aici am stabilit si indexul pentru fiecare node->player.

   - Am folosit o variabila int direction = 1 ca sa retin sensul jocului. De fiecare
	data cand trebuie sa trec la urmatorul jucator sau sa vad care jucator
	a avut scarabeu, folosesc playlist->prev sau playlist->next in functie de
	valoarea directiei.

   - In fiecare runda, am variabila drawn_scarab care dicteaza cum se vor comporta
	jucatorii (este 0 daca inca nu s-a intors niciun scarabeu).

   - Daca un jucator intoarce un scarabeu, ii retin adresa in ptr_toLastScarab si
	actualizez variabila drawn_scarab.

   -> void remove_player(CList *players_list, ScPlayer player);
	- Face exaact ce spune numele. + Free
	- Cazurile si modul de folosire sunt prin comentariile din cod


===================== Victory!! =====================
