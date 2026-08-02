#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to simulate a perfectly fair coin toss (50/50 chance)
int flip_fair_coin() {
    return rand() % 2; 
}

// Function to simulate a coin toss with a specific weight/bias
int flip_weighted_coin(double probability_of_heads) {
    double random_val = (double)rand() / RAND_MAX;
    return (random_val < probability_of_heads) ? 1 : 0;
}

int main() {
    // Initialize random seed based on current system time
    srand((unsigned int)time(NULL));

    int total_flips = 5000; 
    double target_bias = 0.75; // 75% chance of getting Heads
    
    int fair_heads_total = 0;
    int weighted_heads_total = 0;

    // Arrays to log the running probability over time
    // Sized to total_flips + 1 to keep index matching the 1-based flip count
    double fair_prob_tracker[total_flips + 1];
    double weighted_prob_tracker[total_flips + 1];

    // Run the coin toss simulation
    for (int flip = 1; flip <= total_flips; flip++) {
        fair_heads_total += flip_fair_coin();
        weighted_heads_total += flip_weighted_coin(target_bias);
        
        // Track the running average (experimental probability)
        fair_prob_tracker[flip] = (double)fair_heads_total / flip;
        weighted_prob_tracker[flip] = (double)weighted_heads_total / flip;
    }

    // Print final analytical results to the console
    printf("--- Simulation Results (%d flips) ---\n", total_flips);
    printf("Theoretical Fair Coin Probability:  0.500000\n");
    printf("Experimental Fair Coin Probability: %f\n\n", fair_prob_tracker[total_flips]);
    
    printf("Theoretical Weighted Coin Prob:     %f\n", target_bias);
    printf("Experimental Weighted Coin Prob:    %f\n", weighted_prob_tracker[total_flips]);

    // --- Gnuplot Rendering ---
    FILE *plot_pipe = popen("gnuplot -persistent", "w");
    if (plot_pipe == NULL) {
        printf("Error: Failed to open Gnuplot pipe. Ensure Gnuplot is installed.\n");
        return 1;
    }

    // Configure graphics terminal, fonts, and output dimensions
    fprintf(plot_pipe, "set terminal pngcairo enhanced font 'Arial,11' size 800,600\n");
    fprintf(plot_pipe, "set output 'law_of_large_numbers_plot.png'\n");
    
    // Label axes and title
    fprintf(plot_pipe, "set title 'Law of Large Numbers: Fair vs Weighted Coin'\n");
    fprintf(plot_pipe, "set xlabel 'Total Tosses'\n");
    fprintf(plot_pipe, "set ylabel 'Running Probability of Heads'\n");
    fprintf(plot_pipe, "set yrange [0:1]\n");
    
    // Move legend to the bottom right and frame it with a box
    fprintf(plot_pipe, "set key right bottom box\n"); 
    
    // Draw theoretical reference lines for easy visual comparison
    fprintf(plot_pipe, "set arrow from 0,0.5 to %d,0.5 nohead lc rgb 'gray40' dashtype 2 lw 1.5\n", total_flips);
    fprintf(plot_pipe, "set arrow from 0,0.75 to %d,0.75 nohead lc rgb 'gray40' dashtype 2 lw 1.5\n", total_flips);
    
    // Pass plotting instructions with adjusted line widths (lw 2) for better visibility
    fprintf(plot_pipe, "plot '-' title 'Fair Coin (Target: 0.5)' with lines lc rgb 'royalblue' lw 2, \\\n");
    fprintf(plot_pipe, "     '-' title 'Weighted Coin (Target: 0.75)' with lines lc rgb 'crimson' lw 2\n");

    // Define a step interval to prevent over-plotting and optimize rendering speed
    int plot_step = 10; 

    // Stream the fair coin data points to Gnuplot
    for (int i = plot_step; i <= total_flips; i += plot_step) {
        fprintf(plot_pipe, "%d %f\n", i, fair_prob_tracker[i]);
    }
    fprintf(plot_pipe, "e\n");

    // Stream the weighted coin data points to Gnuplot
    for (int i = plot_step; i <= total_flips; i += plot_step) {
        fprintf(plot_pipe, "%d %f\n", i, weighted_prob_tracker[i]);
    }
    fprintf(plot_pipe, "e\n");

    pclose(plot_pipe);
    
    return 0;
}