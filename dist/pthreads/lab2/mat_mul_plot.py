#!/usr/bin/env python3
import matplotlib.pyplot as plt
import pandas as pd
with open('mat_mul.csv', 'r') as datfile:
    fig = plt.figure(figsize=(15,15))
    df = pd.read_csv(datfile,dtype='float')
    df.set_index('M', inplace=True)
    df.groupby('nthreads')['time'].plot(loglog=True, legend=True)
    plt.xlabel('Side Length of square matrix')
    plt.ylabel('Total Time (s)')
    plt.legend(title='Number of Threads')
    plt.title('Matrix Multiplication in Parallel with pthreads')
    plt.savefig('matmul2.png')
