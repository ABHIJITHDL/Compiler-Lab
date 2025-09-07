#include <stdio.h>

#define MAX_STATES 10
#define NUM_SYMBOLS 2 // 'a' is at index 0, 'b' is at index 1

// Structure to represent a set of states
struct state_set {
    int states[MAX_STATES];
    int count;
};

// Global transition tables
struct state_set nfa_transitions[MAX_STATES][NUM_SYMBOLS + 1]; // +1 for epsilon
struct state_set result_nfa[MAX_STATES][NUM_SYMBOLS];
struct state_set epsilon_closures[MAX_STATES];
int num_states;

// Adds a state to a set if it's not already present
void add_state(struct state_set *set, int state) {
    for (int i = 0; i < set->count; i++) {
        if (set->states[i] == state) return; // Already in the set
    }
    set->states[set->count++] = state;
}

// Recursively computes the epsilon closure for a given state
void compute_epsilon_closure(int state, struct state_set *closure) {
    add_state(closure, state);
    
    // The last column (index NUM_SYMBOLS) holds epsilon transitions
    struct state_set epsilon_moves = nfa_transitions[state][NUM_SYMBOLS];

    for (int i = 0; i < epsilon_moves.count; i++) {
        int next_state = epsilon_moves.states[i];
        int found = 0;
        // Check if we've already processed this state to avoid infinite loops
        for(int j = 0; j < closure->count; j++) {
            if (closure->states[j] == next_state) {
                found = 1;
                break;
            }
        }
        if (!found) {
            compute_epsilon_closure(next_state, closure);
        }
    }
}

int main() {
    printf("Enter the number of states: ");
    scanf("%d", &num_states);

    // Input transitions
    printf("Enter transitions (use 'e' for epsilon):\n");
    printf("Format: <from_state> <symbol> <to_state>\n");
    printf("Enter -1 x -1 to stop.\n");

    int from, to;
    char symbol;
    while (1) {
        scanf("%d %c %d", &from, &symbol, &to);
        if (from == -1) break;

        if (symbol == 'a')      add_state(&nfa_transitions[from][0], to);
        else if (symbol == 'b') add_state(&nfa_transitions[from][1], to);
        else if (symbol == 'e') add_state(&nfa_transitions[from][NUM_SYMBOLS], to);
    }

    // 1. Compute E-Closures for all states
    for (int i = 0; i < num_states; i++) {
        compute_epsilon_closure(i, &epsilon_closures[i]);
    }

    // 2. Compute the new transitions
    for (int i = 0; i < num_states; i++) { // For each state i
        for (int j = 0; j < NUM_SYMBOLS; j++) { // For each symbol 'a' and 'b'
            struct state_set temp_set;
            temp_set.count = 0;

            // Find all possible moves on symbol j from E-Closure(i)
            for (int k = 0; k < epsilon_closures[i].count; k++) {
                int s = epsilon_closures[i].states[k]; // A state in E-closure(i)
                struct state_set moves = nfa_transitions[s][j];
                for (int m = 0; m < moves.count; m++) {
                    add_state(&temp_set, moves.states[m]);
                }
            }
            
            // The result is the E-closure of the temporary set
            for (int k = 0; k < temp_set.count; k++) {
                int s = temp_set.states[k];
                for (int m = 0; m < epsilon_closures[s].count; m++) {
                    add_state(&result_nfa[i][j], epsilon_closures[s].states[m]);
                }
            }
        }
    }

    // 3. Print the final NFA without epsilon
    printf("\n--- NFA without Epsilon Transitions ---\n");
    printf("State\t   a\t\t   b\n");
    printf("------------------------------------------\n");

    for (int i = 0; i < num_states; i++) {
        printf("q%d\t", i);
        // Print transitions for 'a'
        printf("{");
        for (int k = 0; k < result_nfa[i][0].count; k++) {
            printf("q%d%s", result_nfa[i][0].states[k], (k == result_nfa[i][0].count - 1) ? "" : ",");
        }
        printf("}\t\t");
        
        // Print transitions for 'b'
        printf("{");
        for (int k = 0; k < result_nfa[i][1].count; k++) {
            printf("q%d%s", result_nfa[i][1].states[k], (k == result_nfa[i][1].count - 1) ? "" : ",");
        }
        printf("}\n");
    }

    return 0;
}