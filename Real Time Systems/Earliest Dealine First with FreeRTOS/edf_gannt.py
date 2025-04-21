import matplotlib.pyplot as plt
import pandas as pd

# Read and parse log file
log_file = 'edf_log.txt'
data = []

with open(log_file, 'r') as file:
    for line in file:
        parts = line.strip().split()
        if len(parts) >= 2:
            time = int(parts[0])
            task_id = parts[1]
            data.append((task_id, time))

# Group time intervals for each task instance
gantt_data = []
if data:
    current_task = data[0][0]
    start_time = data[0][1]

    for i in range(1, len(data)):
        task, time = data[i]
        if task != current_task:
            gantt_data.append((current_task, start_time, data[i - 1][1] + 1))
            current_task = task
            start_time = time
    gantt_data.append((current_task, start_time, data[-1][1] + 1))

# Assign colors
colors = {
    "Task1": "skyblue",
    "Task2": "lightgreen",
    "Task3": "salmon",
    "Task4": "orange",
    "Task5": "plum"
}

# Create Gantt chart
fig, ax = plt.subplots(figsize=(10, 4))
yticks = []
yticklabels = []
for i, (task, start, end) in enumerate(gantt_data):
    ax.barh(y=i, width=end - start, left=start, height=0.5,
            color=colors.get(task, 'gray'), edgecolor='black')
    yticks.append(i)
    yticklabels.append(task)

ax.set_yticks(yticks)
ax.set_yticklabels(yticklabels)
ax.set_xlabel("Time")
ax.set_title("EDF Gantt Chart")
ax.grid(True, axis='x', linestyle='--', alpha=0.5)
ax.set_xlim(0, 14)

plt.tight_layout()
plt.show()
