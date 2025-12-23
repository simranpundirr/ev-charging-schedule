
# EV Charging Schedule Optimization using Genetic Algorithm (GA)

##  Overview
This project implements an **Electric Vehicle (EV) Charging Schedule Optimization system** using a **Genetic Algorithm (GA)** in C++.  
The objective is to efficiently schedule multiple EVs across limited charging ports while minimizing waiting time and prioritizing urgent vehicles.

The project models a **real-world EV charging station problem**, where EV arrivals, charging durations, urgency, and limited resources must be handled optimally.

---

##  Problem Description

An EV charging station has:
- A fixed number of **charging ports**
- Multiple **EVs arriving at different times**
- Each EV has:
  - Arrival time
  - Required charging duration
  - Urgency status (urgent / non-urgent)

### Constraints:
- Only one EV can charge at a port at a time
- EVs must wait if all ports are busy
- Urgent EVs should be prioritized (penalized more if delayed)

### Goal:
To determine an **optimal charging sequence** that:
- Minimizes total waiting time
- Reduces waiting for urgent EVs
- Efficiently utilizes charging ports

This is an **NP-hard scheduling problem**, making **Genetic Algorithms** an effective solution.

---

##  Solution Approach

A **Genetic Algorithm (GA)** is used to solve the scheduling problem.

### Chromosome Representation
- Each chromosome is a **permutation of EV indices**
- The order represents the sequence in which EVs are assigned to charging ports

### Decoding Strategy
- EVs are assigned to the **earliest available charging port**
- Charging starts at the maximum of:
  - EV arrival time
  - Port free time

---

##  Genetic Algorithm Components

### Population Initialization
- Random permutations of EV indices
- Population size: **120**

### Selection
- **Tournament Selection**
- Selects the fittest individual among randomly chosen candidates

### Crossover
- **Order Crossover (OX)**
- Preserves valid permutations
- Maintains relative order of EVs

### Mutation
- **Swap Mutation**
- Two EV positions are swapped randomly
- Mutation probability: **10%**

### Elitism
- Best chromosome of each generation is preserved
- Prevents loss of optimal solutions

### Generations
- Algorithm runs for **400 generations**

---

## Fitness Function

The fitness function is derived from a **penalty model**:

```
Penalty =
    Total Waiting Time
  + (Urgent EV Waiting Time × 6)
  + 0.01 × Makespan
```

Where:
- **Waiting Time** = Charging start time − Arrival time
- **Urgent EVs** incur higher penalty for delay
- **Makespan** = Time when the last EV finishes charging

Fitness is computed as:
```
Fitness = 1 / (1 + Penalty)
```

Higher fitness indicates a better schedule.

---

##  Input Format

```
N  → Number of EVs
P  → Number of charging ports

Then for each EV:
arrival_time  charging_duration  urgency_flag
```

### Example Input:
```
5
2
0  30  1
5  20  0
10 25  1
15 15  0
20 30  0
```

---

## Output Description

The program outputs:
- Best EV charging schedule found
- EV execution order
- Assigned charging port
- Start and end times
- Waiting time per EV
- Urgency status
- Total waiting time
- Final penalty value

---

##  Implementation Details

- Language: **C++**
- Paradigm: **Procedural Programming**
- Data Structures:
  - Arrays
  - Permutations
- Libraries Used:
  - `<iostream>`
  - `<algorithm>`
  - `<cstdlib>`
  - `<ctime>`

### Constraints:
- Maximum EVs: **60**
- Maximum charging ports: **20**

---

##  How to Compile and Run

### Compile:
```bash
g++ main.cpp -o ev_schedule
```

### Run:
```bash
./ev_schedule
```

---

##  Key Features

- Handles real-world scheduling constraints
- Urgency-aware optimization
- Stable and validated GA design
- Deterministic debugging using fixed random seed
- Console-based interactive input

---

##  Possible Enhancements

- Export schedules to CSV
- Visualization using graphs
- Multi-objective optimization
- Adaptive mutation rates
- Object-Oriented redesign
- Real-time EV simulation
- Integration with IoT / smart grid data

---

##  Applications

- Smart EV charging stations
- Energy management systems
- Load balancing problems
- Scheduling and optimization research
- Academic projects and coursework

---

##  Author

**Simran Pundir**  
Computer Science Student  


---

##  Usage Notes

- Suitable for academic demonstrations
- Can be extended for research experiments
- GA parameters can be tuned for performance

---

##  License

This project is intended for **educational and research purposes**.
