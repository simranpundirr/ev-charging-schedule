#include <bits/stdc++.h>
using namespace std;

#define MAX_EV 60
#define MAX_PORTS 20
#define POP_SIZE 120
#define MAX_GEN 400

int N, P;
int arrival[MAX_EV];
int duration_min[MAX_EV];
int urgency_flag[MAX_EV];

int population[POP_SIZE][MAX_EV];
double fitness_arr[POP_SIZE];

void random_permutation(int arr[]) {
    for (int i = 0; i < N; i++) arr[i] = i;
    for (int i = 0; i < N; i++) {
        int j = rand() % N;
        swap(arr[i], arr[j]);
    }
}

double decodePenalty_and_record(
    int chrom[],
    int out_ev[] = nullptr,
    int out_port[] = nullptr,
    int out_start[] = nullptr,
    int out_end[] = nullptr
) {
    int port_free_time[MAX_PORTS] = {0};
    double total_wait = 0, urgent_penalty = 0;

    for (int i = 0; i < N; i++) {
        int ev = chrom[i];

        int chosen_port = 0;
        for (int j = 1; j < P; j++) {
            if (port_free_time[j] < port_free_time[chosen_port])
                chosen_port = j;
        }

        int start = max(arrival[ev], port_free_time[chosen_port]);
        int end = start + duration_min[ev];
        int wait = start - arrival[ev];

        port_free_time[chosen_port] = end;
        total_wait += wait;
        if (urgency_flag[ev]) urgent_penalty += wait * 6;

        if (out_ev) {
            out_ev[i] = ev;
            out_port[i] = chosen_port;
            out_start[i] = start;
            out_end[i] = end;
        }
    }

    int makespan = *max_element(port_free_time, port_free_time + P);
    return total_wait + urgent_penalty + 0.01 * makespan;
}

void initPopulation() {
    for (int i = 0; i < POP_SIZE; i++)
        random_permutation(population[i]);
}

void evaluatePopulation() {
    for (int i = 0; i < POP_SIZE; i++) {
        double penalty = decodePenalty_and_record(population[i]);
        fitness_arr[i] = 1.0 / (1.0 + penalty);
    }
}

int tournament_select(int k = 3) {
    int best = rand() % POP_SIZE;
    for (int i = 1; i < k; i++) {
        int r = rand() % POP_SIZE;
        if (fitness_arr[r] > fitness_arr[best])
            best = r;
    }
    return best;
}

// Correct Order Crossover (OX)
void order_crossover(int pA[], int pB[], int cA[], int cB[]) {
    int l = rand() % N;
    int r = rand() % N;
    if (l > r) swap(l, r);

    bool usedA[MAX_EV] = {0}, usedB[MAX_EV] = {0};

    for (int i = l; i <= r; i++) {
        cA[i] = pA[i];
        cB[i] = pB[i];
        usedA[pA[i]] = true;
        usedB[pB[i]] = true;
    }

    int idxA = (r + 1) % N;
    for (int i = 0; i < N; i++) {
        int x = pB[i];
        if (!usedA[x]) {
            cA[idxA] = x;
            idxA = (idxA + 1) % N;
        }
    }

    int idxB = (r + 1) % N;
    for (int i = 0; i < N; i++) {
        int x = pA[i];
        if (!usedB[x]) {
            cB[idxB] = x;
            idxB = (idxB + 1) % N;
        }
    }
}

void swap_mutation(int chrom[], double prob) {
    if ((double)rand() / RAND_MAX < prob) {
        int i = rand() % N;
        int j = rand() % N;
        swap(chrom[i], chrom[j]);
    }
}

void runGA_and_print_best() {
    evaluatePopulation();

    double best_fitness = fitness_arr[0];
    int best_index = 0;
    for (int i = 1; i < POP_SIZE; i++) {
        if (fitness_arr[i] > best_fitness) {
            best_fitness = fitness_arr[i];
            best_index = i;
        }
    }

    for (int gen = 0; gen < MAX_GEN; gen++) {
        int newpop[POP_SIZE][MAX_EV];

        // Elitism
        for (int k = 0; k < N; k++)
            newpop[0][k] = population[best_index][k];

        for (int i = 1; i < POP_SIZE; i += 2) {
            int p1 = tournament_select();
            int p2 = tournament_select();

            int cA[MAX_EV], cB[MAX_EV];
            order_crossover(population[p1], population[p2], cA, cB);
            swap_mutation(cA, 0.10);
            swap_mutation(cB, 0.10);

            for (int k = 0; k < N; k++) {
                newpop[i][k] = cA[k];
                if (i + 1 < POP_SIZE)
                    newpop[i + 1][k] = cB[k];
            }
        }

        memcpy(population, newpop, sizeof(population));
        evaluatePopulation();

        for (int i = 0; i < POP_SIZE; i++) {
            if (fitness_arr[i] > best_fitness) {
                best_fitness = fitness_arr[i];
                best_index = i;
            }
        }

        if (gen % 50 == 0)
            cout << "[Gen " << gen << "] best fitness = " << best_fitness << endl;
    }

    int ev_order[MAX_EV], port[MAX_EV], st[MAX_EV], en[MAX_EV];
    double penalty = decodePenalty_and_record(
        population[best_index], ev_order, port, st, en
    );

    cout << "\n=== BEST SCHEDULE (Penalty = " << penalty << ") ===\n";
    cout << "Idx EV Port Start End Wait Urgent\n";
    double total_wait = 0;
    for (int i = 0; i < N; i++) {
        int ev = ev_order[i];
        int wait = st[i] - arrival[ev];
        total_wait += wait;
        cout << i << "   " << ev << "   " << port[i]
             << "   " << st[i] << "   " << en[i]
             << "   " << wait << "   " << urgency_flag[ev] << "\n";
    }
    cout << "Total waiting time = " << total_wait << "\n";
}

int main() {
    srand(42);  // fixed seed for debugging

    cout << "EV Charging Scheduling using GA\n";
    cout << "Enter number of EVs: ";
    cin >> N;
    cout << "Enter number of ports: ";
    cin >> P;

    if (N > MAX_EV || P > MAX_PORTS) {
        cout << "Input exceeds limits.\n";
        return 1;
    }

    cout << "Enter EV data (arrival duration urgency):\n";
    for (int i = 0; i < N; i++)
        cin >> arrival[i] >> duration_min[i] >> urgency_flag[i];

    initPopulation();
    runGA_and_print_best();
    return 0;
}
