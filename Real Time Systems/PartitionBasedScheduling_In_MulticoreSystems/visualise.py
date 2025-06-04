import pandas as pd
import matplotlib.pyplot as plt

# Load CSV
df = pd.read_csv("allocation.csv")

# Setup plot
fig, ax = plt.subplots(figsize=(10, 6))
colors = plt.cm.tab10.colors  # Up to 10 unique colors

# Plot each task as a horizontal bar
for _, row in df.iterrows():
    ax.broken_barh(
        [(row['StartTime'], row['Duration'])],
        (row['CoreID'] * 10, 9),  # y position and bar height
        facecolors=colors[int(row['TaskID']) % len(colors)],
        label=f"Task {row['TaskID']}"
    )

# Customize y-axis
unique_cores = sorted(df['CoreID'].unique())
ax.set_yticks([core_id * 10 + 4.5 for core_id in unique_cores])
ax.set_yticklabels([f"Core {core_id}" for core_id in unique_cores])

ax.set_xlabel("Time")
ax.set_title("Task Allocation Gantt Chart")
ax.grid(True)

# Avoid duplicate legend entries
handles, labels = ax.get_legend_handles_labels()
by_label = dict(zip(labels, handles))
ax.legend(by_label.values(), by_label.keys(), loc='upper right')

plt.tight_layout()
plt.show()
