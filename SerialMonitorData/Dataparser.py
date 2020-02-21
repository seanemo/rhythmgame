import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import re


class FrequencyData:
    def __init__(self, data_file, duration):
        self.data_file = data_file
        self.frequencies = {'63 Hz':0, 
                            '160 Hz':1, 
                            '400 Hz':2, 
                            '1 kHz':3, 
                            '2.5 kHz':4, 
                            '6.25 kHz':5, 
                            '16 kHz':6}
        self.freqs_rev = {v:k for k, v in self.frequencies.items()}
        self.duration = duration
        
    def reformat(self, cols=7):
        lines = []
        with open(self.data_file, 'r') as f:
            for line in f:
                numbers = re.findall(r'\d+', line)
                if len(numbers) == cols:
                    lines.append(numbers)
        lines = np.array(lines).T
        self.dataframe = pd.DataFrame({f"{self.freqs_rev[i]}": v for i,v in enumerate(lines)})
    
    def listFrequencies(self):
        print( ''.join(str(k)+'\n' for k in self.frequencies.keys()))
    
    def toCSV(self, filename='', separator='\t'):
        
        if filename == '':
            for char in self.data_file:
                if char != '.':
                    filename += char
                else:
                    break
            self.dataframe.to_csv(f'reformatted_{filename}.csv', sep=separator)
        else:
            self.dataframe.to_csv(f'reformatted_{filename}.csv', sep=separator)
            
    def plot(self, time_stop=None, freq='63 Hz', prev=True, avg=True, graph_size=(8,6)):
        
        plt.figure(num=None, figsize=graph_size, dpi=80, facecolor='w', edgecolor='k')
        
        
        if time_stop is None:
            time_stop = self.duration
        fs = len(self.dataframe) / self.duration
        num_samples = int(fs*time_stop)
        time = np.linspace(0, time_stop, num_samples)
        column = self.frequencies[freq]
        
        plots = []
        if prev:
            rows, cols = self.dataframe.shape
            assert column <= cols
            freqs_title = ''
            for j in range(column+1):
                values = np.array(self.dataframe.iloc[:num_samples, j], dtype=np.float64)
                p, = plt.plot(time, values, label=self.freqs_rev[j]) # tuple assignment throws away part of tuple
                plots.append(p)
                
                freqs_title += self.freqs_rev[j] + ', '
            plt.title(freqs_title[:len(freqs_title)-2])
            
        else:
            values = np.array(self.dataframe.iloc[:num_samples, column], dtype=np.float64)
            p, = plt.plot(time, values, label=self.freqs_rev[column])
            if avg:
                a, = plt.plot(time, np.average(values)*np.ones(num_samples), label=f'Average: {np.average(values)}')
            
            plots.extend((p, a))
            plt.title(freq)
        
        plt.legend(handles=plots)
        plt.show()
        
    
