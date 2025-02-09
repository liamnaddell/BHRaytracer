#!/usr/bin/env python3

#module load python/3.9.10
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
    print("Please give path to results.txt")
    sys.exit(1)

file=sys.argv[1]

first_time=0.0

def parse_row(ls):
    l=ls.split('|')
    c=int(l[1])
    t=float(l[2])
    s=first_time/t
    return (c,t,s)

core=[]
time=[]
speedup=[]
speedup_ahmd=[]

i=0
with open(file) as f:
    for line in f:
        if i==0:
            i+=1
            continue;

        (c,t,s) = parse_row(line)

        if i==1:
            first_time=t
            speed = 1
            i+=1

        core.append(c)
        time.append(t)
        speedup.append(s)
        speedup_ahmd.append(first_time/(12 + (first_time-12)/(c)))

xlabel = 'Cores'
xlabel = 'Nodes'
fig, ax1 = plt.subplots(figsize=(6,5))
ax2 = ax1.twinx()
bloo='#1f77b4'
gren='#2ca02c'
ax1.plot(core,time,'o-',label='time',color=bloo)
ax1.set_xlabel(xlabel, fontsize=14)
ax1.set_ylabel('Time (seconds)', fontsize=14,color=bloo)
ax2.plot(core,speedup,'o-',label='speedup',color=gren)
ax2.set_ylabel('Speedup', fontsize=14,color=gren)
#ax2.plot(core,speedup_ahmd,'o-',label='Ideal speedup',color='orange')
#ax1.set_title('Cores,Time,Speedup')

plt.savefig(f'{file}.png', bbox_inches='tight', dpi=400)
