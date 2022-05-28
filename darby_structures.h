// Boldeanu Ana-Maria
// 311 CD
// SD Tema 1

#include <stdlib.h>
#include <string.h>

// ====================================================
// ================ Structuri Folosite ================
// ====================================================

// O carte de joc. Struct cu proprietatile ei (pt scarabei, 1 = scarabeu, 2 = numar, 3 = speciala)
typedef struct {
    char name;
    char color;
    int value;
    int scarabei; 
} TCard;

// Lista simplu inlantuita. TNode = O carte de joc + pointer la urmatoarea
typedef struct node { 
    TCard card;
    struct node* next;
} TNode, *TList;

// (pt Scarabei) struct pentru un jucator: ScPlayer = TList (pachet de carti) + index jucator
typedef struct player { 
    TList deck;
    int index; // Al catalea jucator este
} ScNode, *ScPlayer;

// (pt Scarabei) Lista dublu inlantuita circulara (pentru ordonarea jucatorilor)
typedef struct cNode { 
    ScPlayer player;
    struct cNode* next;
    struct cNode* prev;
} CNode, *CList;