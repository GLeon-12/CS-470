#include <stdio.h>
#include <stdlib.h>

// Define the Process structure
typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_burst_time;
    int waiting_time;
    int turnaround_time;
} Process;


// Reset process data for a new algorithm
void resetProcessData(Process proc[], int n) {
    for (int i = 0; i < n; i++) {
        proc[i].remaining_burst_time = proc[i].burst_time;
        proc[i].waiting_time = 0;
        proc[i].turnaround_time = 0;
    }
}

// Preemptive Shortest Job First (SJF) Scheduling (Non-Preemptive)
void sjf(Process proc[], int n) {
    int completed = 0;
    int current_time = 0;
  
    printf("Order of execution (SJF): ");

    while (completed < n) {
        // Variables to track the process with the shortest remaining burst time
        int shortest = -1;
        int min_remaining = 1000000;

        // Find the process with the shortest remaining burst time at current_time
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival_time <= current_time && proc[i].remaining_burst_time > 0) {
                if (proc[i].remaining_burst_time < min_remaining) {
                    min_remaining = proc[i].remaining_burst_time;
                    shortest = i;
                }
            }
        }

        // If no process is ready, move to the next time unit
        if (shortest == -1) {
            current_time++;
            continue;
        }

        // Execute the process for 1 time unit 
        printf("P%d ", proc[shortest].process_id);
        proc[shortest].remaining_burst_time--;
        current_time++;

        // If process finished after this time unit, update completed count and calculate times
        if (proc[shortest].remaining_burst_time == 0) {
            completed++;
            proc[shortest].turnaround_time = current_time - proc[shortest].arrival_time;
            proc[shortest].waiting_time = proc[shortest].turnaround_time - proc[shortest].burst_time;
        }
    }

    printf("\n");
}

// Preemptive Round Robin (RR) Scheduling (Preemptive)
void rr(Process proc[], int n, int quantum) {
    int time = 0;
    int completed = 0;

    printf("Order of execution (RR): ");

    // Loop until all processes are completed
    while (completed < n) {
        int executed_in_cycle = 0;  

        // Loop through processes in order of arrival
        for (int i = 0; i < n; i++) {
            if (proc[i].remaining_burst_time > 0 && proc[i].arrival_time <= time) {
                
                // Execute for 1 quantum or remaining burst time, whichever is smaller
                int exec_time = (proc[i].remaining_burst_time > quantum) ? quantum : proc[i].remaining_burst_time;
                
                // Update remaining burst time and current time
                proc[i].remaining_burst_time -= exec_time;
                time += exec_time;

                // Print execution order
                printf("P%d ", proc[i].process_id);

                // If process completed after this execution, update completed count and calculate times
                if (proc[i].remaining_burst_time == 0) {
                    completed++;
                    proc[i].turnaround_time = time - proc[i].arrival_time;
                    proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
                }
                // Mark that we executed a process in this cycle to avoid unnecessary time increment
                executed_in_cycle = 1;
            }
        }

        // If no process could run, advance time
        if (!executed_in_cycle)
            time++;
    }

    printf("\n");
}

// Calculate average waiting time and turnaround time
void calculateAverages(Process proc[], int n) {
    int total_waiting_time = 0, total_turnaround_time = 0;

    // Sum up waiting times and turnaround times
    for (int i = 0; i < n; i++) {
        total_waiting_time += proc[i].waiting_time;
        total_turnaround_time += proc[i].turnaround_time;
    }

    // Print average waiting time and turnaround time
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
}

// Print the process table
void printProcessesTable(Process proc[], int n) {
    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", proc[i].process_id, proc[i].arrival_time, 
               proc[i].burst_time, proc[i].waiting_time, proc[i].turnaround_time);
    }
}

int main() {
    // Initialize processes (ID, Arrival Time, Burst Time)
    Process proc[] = {{1, 0, 8}, {2, 1, 4}, {3, 2, 9}, {4, 3, 5}};

    // Calculate the number of processes and define the time quantum for RR scheduling
    int n = sizeof(proc) / sizeof(proc[0]);
    int quantum = 4;

    // Run SJF scheduling
    resetProcessData(proc, n);
    printf("Preemptive Shortest Job First (SJF) Scheduling:\n");
    sjf(proc, n);
    printProcessesTable(proc, n);
    calculateAverages(proc, n);

    // Run RR scheduling
    resetProcessData(proc, n);
    printf("\nPreemptive Round Robin (RR) Scheduling:\n");
    rr(proc, n, quantum);
    printProcessesTable(proc, n);
    calculateAverages(proc, n);

    return 0;
}