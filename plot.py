import pandas as pd
import sys
import matplotlib.pyplot as plt


df = pd.read_csv(sys.argv[1], skipinitialspace=True)


for field in ["num_nodes", "num_edges", "average_degree", "max_degree", "num_triangles", "new_edges"]:
    df.plot(x="timestep", y=field)
    plt.ylim(ymin=0)
    plt.savefig(field+".png", bbox_inches="tight")
