#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    char data;
    struct Node *adj[8];
} Node;

void instructions() {
    printf("==============================================\n");
    printf("         Instructions for the Game!!!\n");
    printf("      Console-Based 'Catch the Thief' Game\n");
    printf("==============================================\n\n");

    printf("Characters:\n");
    printf("  A -> Police 1\n");
    printf("  B -> Police 2\n");
    printf("  C -> Police 3\n");
    printf("  T -> Thief\n\n");

    printf("How to Play:\n");
    printf("  - Select which police you want to move by entering its number (1, 2, or 3).\n");
    printf("  - Then enter the direction key to move that police.\n\n");

    printf("Direction Controls:\n");
    printf("  a -> Move left\n");
    printf("  w -> Move top-left\n");
    printf("  e -> Move top\n");
    printf("  r -> Move top-right\n");
    printf("  f -> Move right\n");
    printf("  z -> Move bottom-left\n");
    printf("  x -> Move bottom\n");
    printf("  c -> Move bottom-right\n\n");

    printf("Rules of the Game:\n");
    printf("  1. Don't let the thief occupy the starting position of any police.\n");
    printf("  2. You win if all police corner the thief so they can't move.\n");
    printf("  3. The thief moves automatically after your turn.\n");
    printf("  4. Invalid moves will be ignored.\n");
    printf("==============================================\n\n");
}


Node* makeNode(char c) {
    Node* n = malloc(sizeof(Node));
    n->data = c;
    for (int i = 0; i < 8; i++) n->adj[i] = NULL;
    return n;
}

// Global nodes
Node *n1, *n2, *n3, *n4, *n5, *n6, *n7, *n8, *n9, *n10, *n11;
Node *posA, *posB, *posC, *posT;
int win = 0;

char dirKeys[8] = { 'a','w','e','r','f','c','x','z' };

void setup_board() {
    n1 = makeNode(' '); n2 = makeNode(' '); n3 = makeNode(' ');
    n4 = makeNode(' '); n5 = makeNode(' '); n6 = makeNode(' ');
    n7 = makeNode(' '); n8 = makeNode(' '); n9 = makeNode(' ');
    n10 = makeNode(' '); n11 = makeNode(' ');

n1->adj[1] = n10; n1->adj[2] = n4;n1->adj[3] = n5;n1->adj[4] = n2;
n2->adj[0] = n1;n2->adj[2] = n5;n2->adj[4] = n3;
n3->adj[0] = n2;n3->adj[1] = n5;n3->adj[2] = n6;n3->adj[3] = n11;
n4->adj[0] = n10; n4->adj[2] = n7;n4->adj[4] = n5; n4->adj[6] = n1;
n5->adj[0] = n4;n5->adj[1] = n7;n5->adj[2] = n8;n5->adj[3] = n9;
n5->adj[4] = n6;n5->adj[5] = n3;n5->adj[6] = n2;n5->adj[7] = n1;
n6->adj[0] = n5;n6->adj[2] = n9;n6->adj[4] = n11;n6->adj[6] = n3;
n7->adj[4] = n8;n7->adj[5] = n5;n7->adj[6] = n4;n7->adj[7] = n10;
n8->adj[0] = n7;n8->adj[4] = n9;n8->adj[6] =n5;
n9->adj[0] = n8;n9->adj[5] = n11;n9->adj[6] = n6;n9->adj[7] = n5;
n10->adj[3] = n7;n10->adj[4] = n4; n10->adj[5] = n1;
n11->adj[0] = n6;n11->adj[1] = n9; n11->adj[7] = n3;

    posA = n1; posA->data = 'A';
    posB = n2; posB->data = 'B';
    posC = n3; posC->data = 'C';
    posT = n7; posT->data = 'T';
}

// Move thief randomly
void move_thief() {
    Node *options[8];
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (posT->adj[i] && posT->adj[i]->data == ' ') {
            options[count++] = posT->adj[i];
        }
    }
    if (count == 0) { 
        win = 1; return; 
    }
    posT->data = ' ';
    posT = options[rand() % count];
    posT->data = 'T';
}

// Move police
void move_police(Node **p, int dir) {
    if ((*p)->adj[dir] && (*p)->adj[dir]->data == ' ') {
        char symbol = (*p)->data;
        (*p)->data = ' ';
        *p = (*p)->adj[dir];
        (*p)->data = symbol;
        move_thief();
    } else {
        printf("Invalid move!\n");
    }
}

// Handle key
void handle_choice(int choice, char key) {
    int dir = -1;
    for (int i = 0; i < 8; i++) {
        if (dirKeys[i] == key) { dir = i; break; }
    }
    if (dir == -1) {
        printf("Invalid character\n");
        return;
    }
    
    if (choice == 1) move_police(&posA, dir);
    if (choice == 2) move_police(&posB, dir);
    if (choice == 3) move_police(&posC, dir);
    else {
        printf("Invalid choice\n");
       return;
        
    }
}

// Print board exactly like your ASCII
void print_board() {
    printf("       [%c]-----[%c]-----[%c]\n", n7->data, n8->data, n9->data);
    printf("      / | \\     |     / | \\\n");
    printf("     /  |  \\    |    /  |  \\\n");
    printf("    /   |   \\   |   /   |   \\\n");
    printf("   /    |    \\  |  /    |    \\\n");
    printf("  /     |     \\ | /     |     \\\n");
    printf("[%c]----[%c]-----[%c]-----[%c]----[%c]\n", n10->data, n4->data, n5->data, n6->data, n11->data);
    printf("  \\     |     / | \\     |     /\n");
    printf("   \\    |    /  |  \\    |    /\n");
    printf("    \\   |   /   |   \\   |   /\n");
    printf("     \\  |  /    |    \\  |  /\n");
    printf("      \\ | /     |     \\ | /\n");
    printf("       [%c]-----[%c]-----[%c]\n", n1->data, n2->data, n3->data);
}

int main() {
    char ask;
    instructions();
    srand(time(NULL));
    printf("Do you want to play(y/n) : ");
    scanf("%c",&ask);
    if(ask=='y'){
    setup_board();
    while (1) {
        print_board();
        if (posT == n1 || posT == n2 || posT == n3) {
            printf("Thief Escsped! You Loose!\n");
            break;
        }
        if (win) {
            printf("Thief trapped! You win!\n");
            break;
        }

        int choice;
        char key;
        printf("Choose police (1=A, 2=B, 3=C): ");
        scanf("%d", &choice);
        printf("Direction (a,w,e,r,f,c,x,z): ");
        scanf(" %c", &key);
        handle_choice(choice, key);
    }
    }
    return 0;
}