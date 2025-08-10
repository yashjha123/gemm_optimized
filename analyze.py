# %%

import json
import pandas as pd
import seaborn as sns

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import matplotlib as mpl
import numpy as np
import matplotlib.cm as cm
import matplotlib.colors as mcolors
import matplotlib.patches as mpatches
import matplotlib.lines as mlines
import matplotlib.gridspec as gridspec
import matplotlib.font_manager as fm
import matplotlib.colors as mcolors

import re

import sys

# %%
def analyze_benchmarks(data):


# %%

if len(sys.argv) < 2:
    print("Usage: analyze.py <bench.json> > <merge.csv>")

# %%


bench_file = sys.argv[1]
# %%

bench_file = "./bench_f.json"
un_vectorized_bench_file = "./bench_no_vector_f.json"
# %%

with open(bench_file, 'r') as f:
    data = json.load(f)

with open(un_vectorized_bench_file, 'r') as f:
    no_vector_data = json.load(f)
    
# %%
    
# for b in data.get("benchmarks",[]):
#     # print(b)
#     name = b.get("name","")
#     # match BM_OptimizedMM/1024/... style
#     m = re.match(r'^(BM_[A-Za-z0-9_]+)/(\d+)', name)
#     if not m: 
#         print(f"Skipping {name}")
#         continue

benchmarks = data["benchmarks"]
un_vectorized_benchmarks = no_vector_data["benchmarks"]


iter_rows = []
agg_rows = []

for b in benchmarks:
    name = b["name"]
    run_type = b["run_type"]

    # Match BM_<Kind>/<N>/real_time
    m = re.match(r"^(BM_[A-Za-z0-9_]+)/(\d+)/real_time", name)
    if not m:
        continue
    kind, N = m.group(1), int(m.group(2))

    if run_type == "iteration":
        iter_rows.append({
            "kind": kind,
            "N": N,
            "real_time_ns": b["real_time"],
            "GFLOPS": b.get("GFLOPS", None)
        })
    elif run_type == "aggregate" and b.get("aggregate_name") == "mean":
        agg_rows.append({
            "kind": kind,
            "N": N,
            "real_time_ns": b["real_time"],
            "GFLOPS": b.get("GFLOPS", None)
        })

for b in un_vectorized_benchmarks:
    name = b["name"]
    run_type = b["run_type"]

    # Match BM_<Kind>/<N>/real_time
    m = re.match(r"^(BM_[A-Za-z0-9_]+)/(\d+)/real_time", name)
    if not m:
        continue
    kind, N = m.group(1), int(m.group(2))

    if run_type == "iteration":
        iter_rows.append({
            "kind": kind+" (unvectorized)",
            "N": N,
            "real_time_ns": b["real_time"],
            "GFLOPS": b.get("GFLOPS", None)
        })
    elif run_type == "aggregate" and b.get("aggregate_name") == "mean":
        agg_rows.append({
            "kind": kind+" (unvectorized)",
            "N": N,
            "real_time_ns": b["real_time"],
            "GFLOPS": b.get("GFLOPS", None)
        })

# ---------------- Convert to DataFrames ----------------
iter_df = pd.DataFrame(iter_rows)
agg_df = pd.DataFrame(agg_rows)

print(f"Loaded {len(iter_df)} iteration rows and {len(agg_df)} mean aggregates.")

# ---------------- Box & Whisker Plot ----------------
sns.set(style="whitegrid")
plt.figure(figsize=(10,6))
# sns.boxplot(data=iter_df, x="N", y="real_time_ns", hue="kind")
sns.lineplot(data=agg_df, x="N", y="real_time_ns", hue="kind", estimator='mean', ci=None, marker='o', legend=True, color='black', linewidth=1.5)
plt.yscale("log")  # time in ns spans large range
plt.title("Execution Time vs N (Mean of Runs)")
plt.xlabel("Matrix Size N")
plt.ylabel("Real Time (ns, log scale)")
plt.legend(title="Implementation")
plt.tight_layout()
plt.savefig("lineplot_time.png", dpi=150)
print("Saved boxplot_time.png")

# ---------------- Line Chart: GFLOPS vs N ----------------
plt.figure(figsize=(10,6))
sns.lineplot(data=agg_df, x="N", y="GFLOPS", hue="kind", marker="o")
plt.title("GFLOPS vs Matrix Size N (Mean of Runs)")
plt.xlabel("Matrix Size N")
plt.ylabel("GFLOPS")
plt.tight_layout()
plt.savefig("lineplot_gflops.png", dpi=150)
print("Saved lineplot_gflops.png")


# %%
g = sns.catplot(
    data=iter_df, x="N", y="real_time_ns", col="kind",
    kind="box", sharey=True, height=4, aspect=1.2,
    
)
g.set_axis_labels("Matrix Size N", "Real Time (ns)")
g.set_titles("{col_name}")
plt.tight_layout()
plt.savefig("boxplot_facet.png", dpi=150)