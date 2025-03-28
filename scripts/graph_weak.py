#!/usr/bin/env python3

#module load python/3.9.10
import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 3:
    print("Please give path to results.txt x 2")
    sys.exit(1)

file1=sys.argv[1]
file2=sys.argv[2]

def parse_row(ls):
    l=ls.split('|')
    c=int(l[1])
    t=float(l[3])
    e=float(l[2])
    return (c,t,e)


res=[]
for my_file in [file1,file2]:
    i=0
    core=[]
    time=[]
    eps=[]
    with open(my_file) as f:
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
    d={"core":core,"time":time,"eps":eps}
    res.append(d)

bloo='#1f77b4'
# Come up with acc color
red='#ff82b20'
gren='#2ca02c'
epsilon="# of Pixels"
fig, ax1 = plt.subplots(figsize=(6,5))
ax2 = ax1.twinx()
ax1.plot(res[0]["core"],res[0]["time"],'o-',label='time with MPI')
ax1.plot(res[1]["core"],res[1]["time"],'o-',label='time with OMP')
ax1.set_xlabel('Cores', fontsize=14)
ax1.set_ylabel('Time (seconds)', fontsize=14,color=bloo)
ax2.plot(res[0]["core"],res[0]["eps"],'-',label=epsilon,color=gren)
ax2.set_ylabel(f'{epsilon}', fontsize=12,color=gren)
ax1.legend()

plt.savefig(f'{file1}.png', bbox_inches='tight', dpi=400)
