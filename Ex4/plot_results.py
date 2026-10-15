# Exercise 4: Time vs Processors and Speedup graphs
# Usage: python3 plot_results.py   (needs results_sum.csv and results_pi.csv)
import csv
import matplotlib.pyplot as plt

for name, title in [("sum", "Sum 1..10,000,000"), ("pi", "Monte Carlo Pi (10,000,000 samples)")]:
    procs, times = [], []
    with open(f"results_{name}.csv") as f:
        for row in csv.DictReader(f):
            procs.append(int(row["procs"]))
            times.append(float(row["time"]))

    speedup = [times[0] / t for t in times]   # S(p) = T(1) / T(p)

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4))
    ax1.plot(procs, times, "o-")
    ax1.set_xlabel("Number of processors")
    ax1.set_ylabel("Time (s)")
    ax1.set_title(f"{title}: Time vs Processors")
    ax1.set_xticks(procs)
    ax1.grid(True)

    ax2.plot(procs, speedup, "o-", label="Measured speedup")
    ax2.plot(procs, procs, "--", label="Ideal (linear)")
    ax2.set_xlabel("Number of processors")
    ax2.set_ylabel("Speedup  T(1)/T(p)")
    ax2.set_title(f"{title}: Speedup")
    ax2.set_xticks(procs)
    ax2.legend()
    ax2.grid(True)

    fig.tight_layout()
    fig.savefig(f"graph_{name}.png", dpi=150)
    print(f"saved graph_{name}.png")
