#include <stdio.h>
#include <string.h>

#define MAX_STATES 20

int n_states;
// Adjacency matrix for epsilon transitions: epsilon_transitions[i][j] = 1 means i -> j on epsilon
int epsilon_transitions[MAX_STATES][MAX_STATES];
// Matrix to store the epsilon closure for each state
int epsilon_closure[MAX_STATES][MAX_STATES];

// Recursive function (DFS) to find all states reachable from 'state' via epsilon transitions
void find_closure(int state, int closure_set[]) {
    closure_set[state] = 1; // The state is in its own closure

    // Find all states reachable from the current state on epsilon
    for (int i = 0; i < n_states; i++) {
        // If there is an epsilon transition and the destination state is not yet in the closure set
        if (epsilon_transitions[state][i] == 1 && closure_set[i] == 0) {
            find_closure(i, closure_set);
        }
    }
}

int main() {
    printf("Enter the number of states: ");
    scanf("%d", &n_states);

    // Initialize transition and closure matrices to 0
    memset(epsilon_transitions, 0, sizeof(epsilon_transitions));
    memset(epsilon_closure, 0, sizeof(epsilon_closure));

    printf("Enter the epsilon transitions (enter -1 to end for a state):\n");
    for (int i = 0; i < n_states; i++) {
        printf("Epsilon transitions for state q%d: ", i);
        int next_state;
        while (1) {
            scanf("%d", &next_state);
            if (next_state == -1) {
                break;
            }
            if (next_state >= 0 && next_state < n_states) {
                epsilon_transitions[i][next_state] = 1;
            } else {
                printf("Invalid state. Please enter a state between 0 and %d.\n", n_states - 1);
            }
        }
    }

    // Calculate epsilon closure for each state
    for (int i = 0; i < n_states; i++) {
        // For each state i, find its closure and store it in epsilon_closure[i]
        find_closure(i, epsilon_closure[i]);
    }

    printf("\nEpsilon Closures for all states:\n");
    for (int i = 0; i < n_states; i++) {
        printf("Epsilon-Closure(q%d) = { ", i);
        int first = 1;
        for (int j = 0; j < n_states; j++) {
            if (epsilon_closure[i][j] == 1) {
                if (!first) {
                    printf(", ");
                }
                printf("q%d", j);
                first = 0;
            }
        }
        printf(" }\n");
    }

    return 0;
}
