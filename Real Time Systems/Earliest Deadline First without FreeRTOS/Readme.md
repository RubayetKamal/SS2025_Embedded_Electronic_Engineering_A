# Earliest Deadline First (EDF) Scheduling Simulation

## 1) Definition of EDF

**Earliest Deadline First (EDF)** is a dynamic scheduling algorithm used in real-time systems to assign priorities to tasks. The task with the earliest deadline is given the highest priority. EDF is optimal for uniprocessor systems, meaning it can always find a feasible schedule if one exists.



### Formula of EDF Schedulability

The EDF schedulability criterion is as follows:

Sum (C_i / T_i) for i = 1 to n <= 1

Where:
- \(C_i\) is the Worst Case Execution Time (WCET) of task \(i\),
- \(T_i\) is the period of task \(i\),
- \(n\) is the number of tasks.

This formula ensures that the tasks can be scheduled without missing their deadlines.

### EDF Hyper Period

The **Hyperperiod** is the least common multiple (LCM) of all task periods. It defines the length of the scheduling cycle, after which the schedule repeats.

---

## 2) Download the File

To get started, download the necessary files for the EDF scheduling simulation.

In the terminal, go to the directory where the file is located:

```bash
cd ./Downloads/Earliest Deadline First without FreeRTOS
```

3) Build the Files
Now, compile the source code to create the executable. Use the following command:

```bash
gcc main.c func_def.c -o main
```
Why?
This command compiles the source files (main.c and func_def.c) and links them to create an executable file named main.

4) Run the File
To run the simulation, execute the following command:

```bash
./main
```

This will run the simulation using the default parameters. Ensure everything is working as expected.

5) Visualize the Results
Once the initial run works, execute the following command to visualize the task scheduling:

```bash
./main | python3 visualize.py
```

You will be prompted to enter the following task details:

Number of Tasks

Arrival of Task 1

Execution time of Task 1

Deadline of Task 1

Period of Task 1

Arrival of Task 2

And so on, for each task in the set.

6) Gantt Diagram
After running the command, a Gantt chart will be generated based on the EDF scheduling of your tasks.

Example:

Task Set: {(2,5), (4,7)}

<img width="1402" alt="image" src="https://github.com/user-attachments/assets/512b59cd-6fdd-4b8a-befd-930110b73b61" />


To Close the Diagram:
To close the Gantt chart, simply press Control + C (on Mac) in the terminal to stop the script.







