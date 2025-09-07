#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define NUM_SYMBOLS 2

// Represents a set of NFA states (e.g., {q0, q1})
struct StateSet {
    int states[MAX_STATES];
    int count;
};

// Global variables
struct StateSet nfa_transitions[MAX_STATES][NUM_SYMBOLS];
struct StateSet dfa_states[MAX_STATES];
int dfa_transitions[MAX_STATES][NUM_SYMBOLS];
int num_nfa_states;
int num_dfa_states = 0;

// Helper function to sort states within a set for easy comparison
int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void sort_state_set(struct StateSet *set) {
    qsort(set->states, set->count, sizeof(int), compare_ints);
}

// Helper function to add a state to a set if it's not already present
void add_state_to_set(struct StateSet *set, int state) {
    for (int i = 0; i < set->count; i++) {
        if (set->states[i] == state) return; // Already exists
    }
    set->states[set->count++] = state;
}

// Prints a DFA state (e.g., S0, S1) and its corresponding NFA states
void print_dfa_state(int state_index) {
    printf("S%d = {", state_index);
    for (int i = 0; i < dfa_states[state_index].count; i++) {
        printf("q%d%s", dfa_states[state_index].states[i], (i == dfa_states[state_index].count - 1) ? "" : ",");
    }
    printf("}");
}

int main() {
    // --- NFA Input ---
    printf("Enter the number of states in NFA: ");
    scanf("%d", &num_nfa_states);

    // Initialize NFA transition table
    for (int i = 0; i < num_nfa_states; i++) {
        for (int j = 0; j < NUM_SYMBOLS; j++) {
            nfa_transitions[i][j].count = 0;
        }
    }

    printf("Enter NFA transitions (symbol 'a' is 0, 'b' is 1):\n");
    printf("Format: <from_state> <symbol_index> <to_state>\n");
    printf("Enter -1 -1 -1 to stop.\n");
    int from, symbol_idx, to;
    while (1) {
        scanf("%d %d %d", &from, &symbol_idx, &to);
        if (from == -1) break;
        add_state_to_set(&nfa_transitions[from][symbol_idx], to);
    }

    int nfa_start_state;
    printf("Enter NFA start state: ");
    scanf("%d", &nfa_start_state);

    int num_nfa_final_states;
    int nfa_final_states[MAX_STATES];
    printf("Enter number of NFA final states: ");
    scanf("%d", &num_nfa_final_states);
    printf("Enter NFA final states: ");
    for(int i = 0; i < num_nfa_final_states; i++) {
        scanf("%d", &nfa_final_states[i]);
    }

    // --- NFA to DFA Conversion ---
    for(int i=0; i<MAX_STATES; i++) dfa_states[i].count = 0;

    // Create the first DFA state from the NFA start state
    dfa_states[0].states[0] = nfa_start_state;
    dfa_states[0].count = 1;
    num_dfa_states = 1;

    // Process each DFA state
    for (int i = 0; i < num_dfa_states; i++) {
        for (int j = 0; j < NUM_SYMBOLS; j++) { // For symbols 'a' and 'b'
            struct StateSet current_move_set;
            current_move_set.count = 0;

            // Find the union of transitions for the current DFA state's NFA states
            for (int k = 0; k < dfa_states[i].count; k++) {
                int current_nfa_state = dfa_states[i].states[k];
                struct StateSet temp_set = nfa_transitions[current_nfa_state][j];
                for(int m = 0; m < temp_set.count; m++) {
                    add_state_to_set(&current_move_set, temp_set.states[m]);
                }
            }

            if (current_move_set.count == 0) {
                dfa_transitions[i][j] = -1; // No transition (dead state)
                continue;
            }

            sort_state_set(&current_move_set);

            // Check if this new set of states is already a DFA state
            int found_state = -1;
            for (int k = 0; k < num_dfa_states; k++) {
                if (dfa_states[k].count == current_move_set.count &&
                    memcmp(dfa_states[k].states, current_move_set.states, current_move_set.count * sizeof(int)) == 0) {
                    found_state = k;
                    break;
                }
            }
            
            if (found_state != -1) {
                dfa_transitions[i][j] = found_state;
            } else { // It's a new DFA state
                dfa_states[num_dfa_states] = current_move_set;
                dfa_transitions[i][j] = num_dfa_states;
                num_dfa_states++;
            }
        }
    }
    
    // --- DFA Output ---
    printf("\n--- Equivalent DFA ---\n");
    printf("DFA States and their corresponding NFA states:\n");
    for(int i=0; i<num_dfa_states; i++) {
        print_dfa_state(i);
        printf("\n");
    }

    printf("\nDFA Transition Table (S_ is state index, -1 is dead state):\n");
    printf("State\t|\t a \t|\t b \n");
    printf("----------------------------------\n");
    for (int i = 0; i < num_dfa_states; i++) {
        printf("S%d\t|\t S%d\t|\t S%d\n", i, dfa_transitions[i][0], dfa_transitions[i][1]);
    }

    printf("\nDFA Start State: S0\n");
    printf("DFA Final States: ");
    for (int i = 0; i < num_dfa_states; i++) {
        for (int j = 0; j < dfa_states[i].count; j++) {
            for(int k=0; k < num_nfa_final_states; k++) {
                if(dfa_states[i].states[j] == nfa_final_states[k]){
                    printf("S%d ", i);
                    goto next_dfa_state;
                }
            }
        }
        next_dfa_state:;
    }
    printf("\n");

    return 0;
}