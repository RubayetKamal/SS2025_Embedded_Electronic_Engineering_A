# Traffic Light Simulation with FreeRTOS

This project demonstrates the **FreeRTOS Trafic Light Simulation** in a simulated environment. This guide walks you through downloading the required repository, building the project, running the simulation, and visualizing the output.



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

### 2. Add the Traffic Light Simulation Project Folder

Download this project folder (`Traffic Light Simulator with FreeRTOS`) and place it inside the `Demo` directory so that the structure looks like this:

```
FreeRTOS/
└── FreeRTOS/
    └── Demo/
        └── Traffic Light Simulator with FreeRTOS/
```

---

## ⚙️ Build and Run the Traffic Light Simulation

### 3. Navigate to the Project Directory

Change into the Traffic Light Simulation project folder:

```bash
cd "Traffic Light Simulator with FreeRTOS"
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
./posix_demo
```

**What Happens?**  
This executes the simulation. And Ctrl + C to stop it. 

---

## 📊 Visualize Output with a PyQt5

To better understand the task execution , you can generate a GUI based on the simulation log.

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
pip install PyQt5
```

These libraries are used to parse the log file and plot the Gantt chart.

### 9. Run the Gantt Chart Script

```bash
./posix_demo & python3 traffic_light_gui.py
```

**What Happens?**  
The script reads from output and visualizes the traffic lights, making it easier to analyze how tasks were scheduled and when they were executed.

Run the following to finish the program : 1) Control + C, 2) kill %1

---

## ✅ That's All
