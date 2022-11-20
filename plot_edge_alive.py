import pandas as pd
import sys
import matplotlib.pyplot as plt


if len(sys.argv) != 3:
    print("give the input file, output file")


df = pd.read_csv(sys.argv[1], skipinitialspace=True)
df["100"] = df["window of 100"] / df["total edges"]
df["1000"] = df["window of 1000"] / df["total edges"]
df["10000"] = df["window of 10000"] / df["total edges"]
df["100000"] = df["window of 100000"] / df["total edges"]
df["1000000"] = df["window of 1000000"] / df["total edges"]
df["10000000"] = df["window of 10000000"] / df["total edges"]
df["100000000"] = df["window of 100000000"] / df["total edges"]
df["1000000000"] = df["window of 1000000000"] / df["total edges"]

output_filename = sys.argv[2]


df.plot(x="timestamp", y=["100", "1000", "10000", "100000", "1000000", "10000000", "100000000", "1000000000"], legend=True)
# plt.ylim(ymin=0)
plt.title(output_filename.split("/")[-1]+ "percent edges alive")
# plt.yscale('log')
plt.savefig(output_filename, bbox_inches="tight")
