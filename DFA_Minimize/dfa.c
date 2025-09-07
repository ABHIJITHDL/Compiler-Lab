#include <stdio.h>
#include <stdbool.h>

#define MAX_STATES 10
#define NUM_SYMBOLS 2

// Global DFA representation
int num_states;
int transitions[MAX_STATES][NUM_SYMBOLS];
bool is_final[MAX_STATES] = {false};

// Partitions: partition[i] stores the partition number for state i
int partition[MAX_STATES];
int num_partitions = 0;

// Finds the partition number for a given state
int get_partition_index(int state) {
    return partition[state];
}

// The core logic to refine partitions
void refine_partitions() {
    bool changed_in_pass = true;
    while (changed_in_pass) {
        changed_in_pass = false;

        for (int i = 0; i < num_states; i++) {
            for (int j = i + 1; j < num_states; j++) {
                // Only compare states if they are currently in the same partition
                if (get_partition_index(i) != get_partition_index(j)) {
                    continue;
                }

                // Check if their transitions go to different partitions
                for (int sym = 0; sym < NUM_SYMBOLS; sym++) {
                    int next_i = transitions[i][sym];
                    int next_j = transitions[j][sym];

                    if (get_partition_index(next_i) != get_partition_index(next_j)) {
                        // Not equivalent, so move state 'j' to a new partition
                        partition[j] = ++num_partitions;
                        changed_in_pass = true;
                        // Break symbol loop and re-check pairs with the new partitions
                        goto next_pair; 
                    }
                }
                next_pair:;
            }
        }
    }
}


int main() {
    // --- DFA Input ---
    printf("Enter the number of states in DFA: ");
    scanf("%d", &num_states);

    printf("Enter DFA transitions (symbol 'a' is 0, 'b' is 1):\n");
    printf("Format: For each state, enter its transition on 'a' then 'b'.\n");
    for (int i = 0; i < num_states; i++) {
        printf("State q%d: ", i);
        scanf("%d %d", &transitions[i][0], &transitions[i][1]);
    }

    int num_final_states;
    printf("Enter number of final states: ");
    scanf("%d", &num_final_states);
    printf("Enter final states: ");
    for (int i = 0; i < num_final_states; i++) {
        int final_state;
        scanf("%d", &final_state);
        is_final[final_state] = true;
    }

    // --- DFA Minimization ---

    // Step 1: Initial Partition (Final vs. Non-Final)
    // Non-final states are in partition 0.
    // Final states are in partition 1.
    for (int i = 0; i < num_states; i++) {
        if (is_final[i]) {
            partition[i] = 1;
        } else {
            partition[i] = 0;
        }
    }
    num_partitions = (num_final_states > 0 && num_final_states < num_states) ? 1 : 0;


    // Step 2: Iteratively refine the partitions
    refine_partitions();

    // --- Output Minimized DFA ---
    printf("\n--- Minimized DFA ---\n");
    printf("Total states in minimized DFA: %d\n", num_partitions + 1);

    // To make output cleaner, let's map partition numbers to new state names like M0, M1...
    int mapped_partitions[MAX_STATES];
    for(int i=0; i<MAX_STATES; i++) mapped_partitions[i] = -1;
    int new_state_count = 0;

    printf("\nNew States and their original state sets:\n");
    for (int i = 0; i < num_states; i++) {
        int p = get_partition_index(i);
        if (mapped_partitions[p] == -1) {
            mapped_partitions[p] = new_state_count++;
            printf("M%d = { ", mapped_partitions[p]);
            for(int j=0; j<num_states; j++){
                if(get_partition_index(j) == p){
                    printf("q%d ", j);
                }
            }
            printf("}\n");
        }
    }

    printf("\nNew Transition Table:\n");
    printf("State\t|\t a \t|\t b \n");
    printf("----------------------------------\n");
    for (int i = 0; i <= num_partitions; i++) {
        // Find a representative state for this partition
        int rep_state = -1;
        for(int j=0; j<num_states; j++){
            if(get_partition_index(j) == i){
                rep_state = j;
                break;
            }
        }
        if (rep_state != -1) {
            int trans_a = transitions[rep_state][0];
            int trans_b = transitions[rep_state][1];
            printf("M%d\t|\t M%d\t|\t M%d\n", mapped_partitions[i], mapped_partitions[get_partition_index(trans_a)], mapped_partitions[get_partition_index(trans_b)]);
        }
    }
    
    // Find and print new start and final states
    printf("\nNew Start State: M%d\n", mapped_partitions[get_partition_index(0)]);
    printf("New Final States: ");
    for(int i=0; i<=num_partitions; i++){
        int rep_state = -1;
        for(int j=0; j<num_states; j++){
            if(get_partition_index(j) == i){
                rep_state = j;
                break;
            }
        }
        if(rep_state != -1 && is_final[rep_state]){
            printf("M%d ", mapped_partitions[i]);
        }
    }
    printf("\n");

    return 0;
}