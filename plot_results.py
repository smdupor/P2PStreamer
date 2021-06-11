import matplotlib.pyplot as plt
import pandas as pd
import glob
import sys

path = 'logs/'
#all_files = glob.glob(path + "*.csv")
all_files = ("logs/a.csv", "logs/b.csv", "logs/c.csv", "logs/d.csv", "logs/e.csv")
for file in all_files:
    lbl = "Host " + file[5:6]
    df = pd.read_csv(file, index_col=1)
    #print(df)
    plt.plot(df, label=lbl)

plt.xlabel("Elapsed Time (ms)")
plt.ylabel("Number of Files Downloaded")
plt.title("Time Results for Experiment " + sys.argv[1])
plt.legend()
plt.show()
outFname = sys.argv[1]+".png"
#plt.savefig(outFname)


