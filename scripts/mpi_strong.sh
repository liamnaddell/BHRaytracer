#!/usr/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=40
#SBATCH --time=1:00:00
#SBATCH --mail-type=FAIL
#SBATCH --job-name liam-mpi-strong

FINAL=$1
EXE=$1/release/main

source $FINAL/scripts/niagarasetup

if [ ! -f $EXE ]; then
    echo "Forgot to compile final, was looking for: '$EXE'"
    echo "Maybe try adding an argument on your sbatch invocation"
    echo "sbatch path/to/script path/to/final"
    exit 1
fi

#Pretty grind-y settings.
args="-i $FINAL/data/squares.jpg -T -s 3"

function run_program() {
    export procs=$1

    # Run the program on a fixed work size, then scrape the amount of time output by the tracer.
    mpirun -c $procs $EXE $args -c 1 2>/dev/null | grep "tock" | cut -d' ' -f3 | tail -n1
}

function run_with_num_cores() {
    echo "|core|time|" 
    for i in "$@"; do
        t1=$(run_program $i)
        echo "|$i|$t1|"
    done
}

echo "Ready to begin"
run_with_num_cores $(seq 1 40)
echo "Done!"
