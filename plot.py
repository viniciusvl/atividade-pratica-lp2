import matplotlib.pyplot as plt

threads = [1, 2, 4, 8]

datasets = [
    {
        "label":   "10.000 palavras",
        "t_seq":   0.000407,
        "t_par":   [0.000799, 0.000616, 0.000661, 0.001715],
        "speedup": [0.51, 0.66, 0.61, 0.24],
        "color_time": "steelblue",
        "color_spd":  "tomato",
    },
    {
        "label":   "100.000 palavras",
        "t_seq":   0.003545,
        "t_par":   [0.003895, 0.001982, 0.001882, 0.002241],
        "speedup": [0.91, 1.78, 1.88, 1.57],
        "color_time": "steelblue",
        "color_spd":  "tomato",
    },
    {
        "label":   "5.000.000 palavras",
        "t_seq":   0.134023,
        "t_par":   [0.123537, 0.074752, 0.042994, 0.021848],
        "speedup": [1.08, 1.79, 3.12, 6.13],
        "color_time": "steelblue",
        "color_spd":  "tomato",
    },
]

fig, axes = plt.subplots(1, 3, figsize=(16, 5))
fig.suptitle("Contagem de Palavras — Tempo e Speedup por Número de Threads", fontsize=13)

for ax, ds in zip(axes, datasets):
    ax2 = ax.twinx()

    line_time, = ax.plot(threads, ds["t_par"], marker="o", color=ds["color_time"],
                         linewidth=2, label="T_par (s)")
    ax.axhline(ds["t_seq"], color=ds["color_time"], linestyle="--", linewidth=1.2,
               label=f"T_seq ({ds['t_seq']:.6f} s)", alpha=0.5)

    line_spd, = ax2.plot(threads, ds["speedup"], marker="s", color=ds["color_spd"],
                          linewidth=2, label="Speedup")

    ax.set_title(ds["label"])
    ax.set_xlabel("Número de Threads")
    ax.set_ylabel("Tempo médio (s)", color=ds["color_time"])
    ax2.set_ylabel("Speedup", color=ds["color_spd"])
    ax.set_xticks(threads)
    ax.tick_params(axis="y", labelcolor=ds["color_time"])
    ax2.tick_params(axis="y", labelcolor=ds["color_spd"])
    ax.grid(True, linestyle="--", alpha=0.4)

    lines = [line_time, line_spd]
    labels = [l.get_label() for l in lines]
    ax.legend(lines, labels, loc="upper right", fontsize=8)

plt.tight_layout()
plt.savefig("speedup.png", dpi=150)
plt.show()
print("Grafico salvo em speedup.png")
