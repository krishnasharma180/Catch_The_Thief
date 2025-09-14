#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
  #include <windows.h>
  static void sleep_ms(int ms) { Sleep(ms); }
#else
  #include <unistd.h>
  static void sleep_ms(int ms) { usleep(ms * 1000); }
#endif

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

// helper: check if a given node is an escape node (n1, n2, n3)
static int isEscapeNode(Node *x) {
    return (x == n1 || x == n2 || x == n3);
}

// helper: check if a node is adjacent to thief (useful when police blocks)
static int isAdjacentToThief(Node *x) {
    if (!x || !posT) return 0;
    for (int i = 0; i < 8; ++i) {
        if (posT->adj[i] && posT->adj[i] == x) return 1;
    }
    return 0;
}

// dialogue helpers
static void typeOutLine(const char *s, int msPerChar) {
    for (const char *p = s; *p; ++p) {
        putchar(*p);
        fflush(stdout);
        sleep_ms(msPerChar);
    }
    putchar('\n');
}

// Thief taunt when actually taking escape this move
static void thiefTaunt() {
    sleep_ms(80);
    typeOutLine("Thief: ...", 20);
    sleep_ms(120);
    typeOutLine("Thief: You are cooked.", 30);
    sleep_ms(80);
}

// Thief reaction when police blocks a free escape that was adjacent to thief
static void thiefBlockedReaction() {
    sleep_ms(80);
    typeOutLine("Thief: Argh! You blocked my way!", 20);
    sleep_ms(60);
    typeOutLine("Thief: That's cheap... I'll find another route.", 18);
    sleep_ms(80);
}

// Police conversation (three police roast the thief)
static void policeConversation() {
    const char *p1[] = {
        "Officer A: Did you *see* that escape attempt?",
        "Officer A: That was the most obvious move I've ever seen."
    };
    const char *p2[] = {
        "Officer B: He stood one tile away and still panicked. Incredible.",
        "Officer B: I think he tripped on his own ego."
    };
    const char *p3[] = {
        "Officer C: We could have taken a coffee break and still caught him.",
        "Officer C: This is textbook 'how not to thief'."
    };
    int idx1 = rand() % (sizeof(p1) / sizeof(p1[0]));
    int idx2 = rand() % (sizeof(p2) / sizeof(p2[0]));
    int idx3 = rand() % (sizeof(p3) / sizeof(p3[0]));

    sleep_ms(160);
    typeOutLine(p1[idx1], 6);
    sleep_ms(300);
    typeOutLine(p2[idx2], 6);
    sleep_ms(260);
    typeOutLine(p3[idx3], 6);
    sleep_ms(160);
    typeOutLine("Officer A: Maybe next time he'll read the map BEFORE trying to improvise.", 5);
    sleep_ms(120);
}

// Move thief with preference for escape nodes (if any)
void move_thief() {
    Node *options[8];
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (posT->adj[i] && posT->adj[i]->data == ' ') {
            options[count++] = posT->adj[i];
        }
    }
    if (count == 0) {
        win = 1;
        return;
    }

    // Collect escape options among legal moves
    Node *escapeOptions[8];
    int escapeCount = 0;
    for (int i = 0; i < count; ++i) {
        if (isEscapeNode(options[i])) {
            escapeOptions[escapeCount++] = options[i];
        }
    }

    // If any escape is available, prefer it (pick randomly among escapes).
    Node *chosen = NULL;
    if (escapeCount > 0) {
        chosen = escapeOptions[rand() % escapeCount];
        // Thief will take escape -> taunt then move and end game detection happens in main loop
        thiefTaunt();
        posT->data = ' ';
        posT = chosen;
        posT->data = 'T';
        return;
    }

    // No direct escape available — pick any legal move randomly
    chosen = options[rand() % count];

    // Move thief
    posT->data = ' ';
    posT = chosen;
    posT->data = 'T';

    // If there were escape options but thief didn't have one (shouldn't happen because we prefer escapes),
    // or if none existed, do nothing (no taunt). Police roast only applies when thief ignores a real escape.
    // Since we always take escape if available, police roasting happens only when an escape existed earlier
    // but got blocked by police before thief moved; such blocking triggers a thief reaction in move_police().
}

// Move police
void move_police(Node **p, int dir) {
    Node *target = (*p)->adj[dir];
    // Pre-move checks: was target a free escape node AND adjacent to thief? If yes, police is stealing the escape.
    int blockedEscape = 0;
    if (target && target->data == ' ' && isEscapeNode(target) && isAdjacentToThief(target)) {
        blockedEscape = 1;
    }

    if (target && target->data == ' ') {
        char symbol = (*p)->data;
        (*p)->data = ' ';
        *p = target;
        (*p)->data = symbol;

        // If police just moved into an escape node that was a real escape for thief, thief reacts.
        if (blockedEscape) {
            thiefBlockedReaction();
        } else {
            // normal flow: after police move, thief moves automatically
            move_thief();
        }
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
    
    if (choice == 1) {
        move_police(&posA, dir);
    } else if (choice == 2) {
        move_police(&posB, dir);
    } else if (choice == 3) {
        move_police(&posC, dir);
    } else {
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
    srand((unsigned)time(NULL));
    printf("Do you want to play(y/n) : ");
    scanf(" %c",&ask);
    if(ask=='y'){
        setup_board();
        while (1) {
            print_board();
            if (posT == n1 || posT == n2 || posT == n3) {
                printf("Thief Escaped! You Lose!\n");
                break;
            }
            if (win) {
                printf("Thief trapped! You win!\n");
                break;
            }

            int choice;
            char key;
            printf("Choose police (1=A, 2=B, 3=C): ");
            if (scanf("%d", &choice) != 1) { // simple input guard
                printf("Bad input. Exiting.\n");
                break;
            }
            printf("Direction (a,w,e,r,f,c,x,z): ");
            scanf(" %c", &key);
            handle_choice(choice, key);
        }
    }
    return 0;
}
