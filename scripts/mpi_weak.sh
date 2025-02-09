#!/usr/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --time=0:45:00
#SBATCH --mail-type=FAIL
#SBATCH --job-name liam-mpi-weak

FINAL=$1
EXE=$1/release/main

source $FINAL/niagrasetup

if [ ! -f $EXE ]; then
    echo "Forgot to compile final, was looking for: '$EXE'"
    echo "Maybe try adding an argument on your sbatch invocation"
    echo "sbatch path/to/script path/to/final"
    exit 1
fi

#Pretty grind-y settings.
args="-i $FINAL/bgedit.jpg -T -s 3"
#Test epsilon starting from 6

function run_program() {
    export procs=$1
    wk=$2

    set -x
    # Run the program on a fixed work size, then scrape the amount of time output by the tracer.
    mpirun -c $procs $EXE $args -c 1 -e $2 2>/dev/null | grep "tock" | cut -d' ' -f3 | tail -n1
}

function run_with_num_cores() {
    echo "|core|eps|time|" 
    maxcores=16
    for i in "$@"; do
        wk=$(echo "scale=10; 6 - ((6 / $maxcores) * ($i - 1))" | bc)
        t1=$(run_program $i $wk)
        echo "|$i|$wk|$t1|"
    done
}

run_with_num_cores $(seq 1 16)
