#!/usr/bin/env python3

#module load python/3.9.10
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
    print("Please give path to results.txt")
    sys.exit(1)

file=sys.argv[1]

def parse_row(ls):
    l=ls.split('|')
    c=int(l[1])
    t=float(l[3])
    e=float(l[2])
    return (c,t,e)

core=[]
time=[]
eps=[]

i=0
with open(file) as f:
    for line in f:
        if line[0] != '|':
            continue;
        if i==0:
            i+=1
            continue;

        (c,t,e) = parse_row(line)

        if i==1:
            speed = 1
            i+=1

        core.append(c)
        time.append(t)
        eps.append(e)

bloo='#1f77b4'
gren='#2ca02c'
epsilon="Epsilon"
epsilon="Width (pixels)"
fig, ax1 = plt.subplots(figsize=(6,5))
ax2 = ax1.twinx()
ax1.plot(core,time,'o-',label='time')
ax1.set_xlabel('Cores', fontsize=14)
ax1.set_ylabel('Time (seconds)', fontsize=14,color=bloo)
ax2.plot(core,eps,'-',label=epsilon,color=gren)
ax2.set_ylabel(f'{epsilon}', fontsize=12,color=gren)

plt.savefig(f'{file}.png', bbox_inches='tight', dpi=400)
