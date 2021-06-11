import matplotlib.pyplot as plt
import pandas as pd
import glob
import sys

path = 'logs/'
all_files = glob.glob(path + "*.csv")

for file in all_files:
    lbl = "Host " + file[5:6]
    #print(lbl)
    df = pd.read_csv(file, index_col=0)
    plt.plot(df, label=lbl)

plt.ylabel("Elapsed Time (ms)")
plt.xlabel("Number of Files Downloaded")
plt.title("Time Results for Experiment " + sys.argv[1])
plt.legend()
#plt.show()
outFname = sys.argv[1]+".png"
plt.savefig(outFname)


