#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt

with open('lab2_data.csv', 'r') as datfile:
    fig = plt.figure(figsize=(15,15))
    df = pd.read_csv(datfile,dtype='float')
    df.set_index('ssize', inplace=True)
    df.groupby('nthreads')['tot'].plot(loglog=True, legend=True)
    plt.xlabel('Size of Vector')
    plt.ylabel('Total Time (s)')
    plt.legend(title='Number of Threads')
    plt.title('Scalar Multiplication of a Vector in Parallel')
    plt.savefig('fig3.png')
