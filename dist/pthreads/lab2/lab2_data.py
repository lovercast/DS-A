#!/usr/bin/env python3
import multiprocessing
from os import getcwd
import subprocess
import numpy as np
import pandas as pd

# call the lab2 function over several values of numthreads and over several working set sizes.
# Working set sizes -- 2^10, 2^15, 2^20, 2^25, 2^30
# nthreads values -- range over number of cores in system -- how to find number of cores on system?
# data will be written to stdout

if __name__ == "__main__":
    ncpus = multiprocessing.cpu_count() # count cpus
    set_sizes = [2**10, 2**11, 2**12, 2**13, 2**14, 2**15,
                 2**16, 2**17, 2**18, 2**19, 2**20,
                 2**21, 2**22, 2**23, 2**24, 2**25,
                 2**26, 2**27, 2**28, 2**29, 2**30] # memory sizes larger than 2**30 fail to allocate
    nthreads_vals = range(1,ncpus+1)
    cwd = getcwd()
    cmd = cwd + "/lab2"

    labels = {
            "nthreads":"Number of Threads",
            "ssize":"Set Size",
            "tot":"Total Time (s)",
            "mean":"Mean Time (s)",
            "med":"Median Time (s)",
            "stdev":"Standard Deviation"
            }


    df = pd.DataFrame(
        {
            "nthreads":[],
            "ssize":[],
            "tot":[],
            "mean":[],
            "med":[],
            "stdev":[]
        })

    row = []

    def putrow(df):
        df.loc[len(df.index)] = row

    for nthreads in nthreads_vals:
        for ssize in set_sizes:
            output = subprocess.check_output([cmd,"-x", "2", f"{nthreads}", f"{ssize}"]).decode("utf8")
            print(output)
            strs = output.strip().split()
            row = np.array(strs).astype(np.double)
            putrow(df)

    df.to_csv('lab2_data.csv')


    exit(0)




