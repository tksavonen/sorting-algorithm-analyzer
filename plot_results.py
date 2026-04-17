# final/plot_results.py

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

colors = {
    "Insertion Sort": "blue",
    "Bubble Sort": "red",
    "Merge Sort": "green"
}

df = pd.read_csv("results.csv")
df = df.groupby(["Population", "Algorithm"], as_index=False)["Time(ms)"].mean()

plt.figure(figsize=(9, 5))
for algo in df["Algorithm"].unique():
    subset = df[df["Algorithm"] == algo].sort_values("Population")
    
    plt.plot(
        subset["Population"],
        subset["Time(ms)"],
        marker='o',
        linewidth=2,
        label=algo,
        color=colors.get(algo, None))

plt.yscale("log")

plt.xlabel("Population (n)")
plt.ylabel("Time (ms, log scale)")
plt.title("Sorting Algorithm Performance")

plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()

plt.tight_layout()

# theoretical reference curves
x = np.array(sorted(df["Population"].unique()))

n2 = x**2
nlogn = x * np.log2(x)

bubble = df[df["Algorithm"] == "Bubble Sort"]
scale_n2 = bubble["Time(ms)"].iloc[-1] / (bubble["Population"].iloc[-1]**2)

merge = df[df["Algorithm"] == "Merge Sort"]
scale_nlogn = merge["Time(ms)"].iloc[-1] / (
    merge["Population"].iloc[-1] * np.log2(merge["Population"].iloc[-1])
)

n2_scaled = x**2 * scale_n2
nlogn_scaled = x * np.log2(x) * scale_nlogn

plt.plot(x, n2_scaled, '--', label='O(n²) reference', color='gray')
plt.plot(x, nlogn_scaled, '--', label='O(n log n) reference', color='black')

plt.show()