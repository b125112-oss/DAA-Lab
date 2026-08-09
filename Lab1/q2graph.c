#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Helper function for a Fair Coin toss
int tossFair() {
    return rand() % 2; 
}

// Helper function for a Biased Coin toss
int tossBiased(double bias) {
    double r = (double)rand() / RAND_MAX;
    return (r < bias) ? 1 : 0;
}

int main() {
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    int max_tosses = 5000; // Total number of tosses for the simulation
    double bias_prob = 0.75; // 75% chance of Heads for the biased coin
    
    int fair_heads = 0;
    int biased_heads = 0;

    // Arrays to store cumulative probability values for plotting
    // Sized to max_tosses + 1 to avoid out-of-bounds errors
    double fair_probs[max_tosses + 1];
    double biased_probs[max_tosses + 1];

    // Simulation loop
    for (int i = 1; i <= max_tosses; i++) {
        fair_heads += tossFair();
        biased_heads += tossBiased(bias_prob);
        
        // Calculate the cumulative probability of Heads up to the i-th toss
        fair_probs[i] = (double)fair_heads / i;
        biased_probs[i] = (double)biased_heads / i;
    }

    // Output final results to console to explicitly show the probabilities
    printf("--- Simulation Complete (%d tosses) ---\n", max_tosses);
    printf("Expected Fair Coin Probability:   0.500000\n");
    printf("Calculated Fair Coin Probability: %f\n\n", fair_probs[max_tosses]);
    printf("Expected Biased Coin Probability: %f\n", bias_prob);
    printf("Calculated Biased Coin Prob:      %f\n", biased_probs[max_tosses]);

    // --- Visualization using Gnuplot ---
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        printf("Error: Could not open Gnuplot.\n");
        return 1;
    }

    // Graph setup
    fprintf(gnuplot, "set terminal pngcairo enhanced font 'arial,10' size 800,600\n");
    fprintf(gnuplot, "set output 'coin_toss.png'\n");
    fprintf(gnuplot, "set title 'Law of Large Numbers: Fair vs Biased Coin'\n");
    fprintf(gnuplot, "set xlabel 'Number of Tosses'\n");
    fprintf(gnuplot, "set ylabel 'Cumulative Probability of Heads'\n");
    fprintf(gnuplot, "set yrange [0:1]\n");
    fprintf(gnuplot, "set key right bottom\n"); // Move legend so it doesn't block data
    
    // Add horizontal reference lines at Expected Probabilities (0.5 and 0.75)
    fprintf(gnuplot, "set arrow from 0,0.5 to %d,0.5 nohead lc rgb 'black' dashtype 2\n", max_tosses);
    fprintf(gnuplot, "set arrow from 0,0.75 to %d,0.75 nohead lc rgb 'black' dashtype 2\n", max_tosses);
    
    // Plotting as lines to show convergence
    fprintf(gnuplot, "plot '-' title 'Fair Coin (Exp: 0.5)' with lines lc rgb 'blue', \\\n");
    fprintf(gnuplot, "     '-' title 'Biased Coin (Exp: 0.75)' with lines lc rgb 'red'\n");

    // We plot every 10th step to make the graph render cleanly
    int step = 10; 

    // Send Fair Coin Data
    for (int i = step; i <= max_tosses; i += step) {
        fprintf(gnuplot, "%d %f\n", i, fair_probs[i]);
    }
    fprintf(gnuplot, "e\n");

    // Send Biased Coin Data
    for (int i = step; i <= max_tosses; i += step) {
        fprintf(gnuplot, "%d %f\n", i, biased_probs[i]);
    }
    fprintf(gnuplot, "e\n");

    pclose(gnuplot);
    return 0;
}