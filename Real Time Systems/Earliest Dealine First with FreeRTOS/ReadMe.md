# Earliest Deadline First with FreeRTOS

This project demonstrates the **Earliest Deadline First (EDF)** scheduling algorithm implemented using FreeRTOS in a simulated environment. This guide walks you through downloading the required repository, building the project, running the simulation, and visualizing the output using a Gantt chart.

---

## 📥 Setup Instructions

### 1. Download the FreeRTOS Repository

Download or clone the official FreeRTOS repository from GitHub:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git
```

Navigate to the Demo directory:

```bash
cd ./FreeRTOS/FreeRTOS/Demo
```

### 2. Add the EDF Project Folder

Download this project folder (`Earliest Deadline First with FreeRTOS`) and place it inside the `Demo` directory so that the structure looks like this:

```
FreeRTOS/
└── FreeRTOS/
    └── Demo/
        └── Earliest Deadline First with FreeRTOS/
```

---

## ⚙️ Build and Run the EDF Simulation

### 3. Navigate to the Project Directory

Change into the EDF project folder:

```bash
cd "Earliest Deadline First with FreeRTOS"
```

### 4. Clean Previous Builds

```bash
make clean
```

**Why?**  
This command removes any previously compiled object files and binaries. It ensures that the build starts fresh, preventing issues caused by outdated or leftover files.

### 5. Compile the Project

```bash
make
```

**Why?**  
This command compiles all the source files and links them into a single executable named `edf_demo`, as defined in the Makefile. This prepares the program for execution.

### 6. Run the EDF Simulation

```bash
./edf_demo
```

**What Happens?**  
This executes the simulation. All output from the simulation (such as task execution order and timing information) is saved automatically in a file called `edf_log.txt`, located in the current working directory.

---

## 📊 Visualize Output with a Gantt Chart

To better understand the task execution timeline, you can generate a Gantt chart based on the simulation log.

### 7. Create a Python Virtual Environment

```bash
python3 -m venv myenv
source myenv/bin/activate
```

**Why?**  
Creating a virtual environment keeps all Python dependencies contained and avoids potential conflicts with global packages.

### 8. Install Required Python Libraries

You may need to install the following libraries:

```bash
pip install matplotlib pandas
```

These libraries are used to parse the log file and plot the Gantt chart.

### 9. Run the Gantt Chart Script

```bash
python3 edf_gannt.py
```

**What Happens?**  
The script reads from `edf_log.txt` and visualizes the task schedule as a Gantt chart, making it easier to analyze how tasks were scheduled and when they were executed under the EDF algorithm.

---

## ✅ You're All Set!

You’ve successfully built, run, and visualized an EDF scheduling simulation using FreeRTOS.  
Feel free to explore different task configurations and deadlines to see how EDF behaves in various scenarios.
