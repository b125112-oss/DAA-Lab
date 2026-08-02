#include <stdio.h>

// Recursive function to simulate the Tower of Hanoi puzzle
// 'moves' is passed by reference to keep a running tally
void hanoi(int n, char from_rod, char to_rod, char aux_rod, long long *moves) {
    if (n == 0) {
        return;
    }
    
    // Step 1: Move top n-1 disks from origin to auxiliary rod
    hanoi(n - 1, from_rod, aux_rod, to_rod, moves);
    
    // Step 2: Move the nth disk (This represents a physical move)
    (*moves)++;
    
    // Step 3: Move the n-1 disks from auxiliary rod to destination rod
    hanoi(n - 1, aux_rod, to_rod, from_rod, moves);
}

void runHanoiAnalysis() {
    printf("Towers of Hanoi Complexity Analysis\n");
    printf("--------------------------------------------------------\n");
    printf("%-12s | %-18s | %-20s\n", "Discs (n)", "Simulated Moves", "Theoretical (2^n - 1)");
    printf("--------------------------------------------------------\n");

    // We simulate up to 25 disks. Going much higher will cause 
    // the simulation to take a noticeable amount of time due to O(2^n) time complexity.
    for (int n = 1; n <= 25; n++) {
        long long moves = 0;
        
        // Run the recursive simulation
        hanoi(n, 'A', 'C', 'B', &moves);
        
        // Calculate the theoretical expected moves using bitwise shift for powers of 2
        // 1LL << n is equivalent to 2^n
        long long theoretical = (1LL << n) - 1;

        printf("%-12d | %-18lld | %-20lld\n", n, moves, theoretical);
    }
    printf("--------------------------------------------------------\n");
}

int main() {
    runHanoiAnalysis();
    return 0;
}