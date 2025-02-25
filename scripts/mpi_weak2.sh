#!/usr/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=40
#SBATCH --time=3:00:00
#SBATCH --mail-type=FAIL
#SBATCH --job-name liam-mpi-weak

FINAL=$1
EXE=$1/release/main

source $FINAL/scripts/niagrasetup

if [ ! -f $EXE ]; then
    echo "Forgot to compile final, was looking for: '$EXE'"
    echo "Maybe try adding an argument on your sbatch invocation"
    echo "sbatch path/to/script path/to/final"
    exit 1
fi

#Pretty grind-y settings.
args="-i $FINAL/bgedit.jpg -T -s 3 -e 0.5"
#Test image width ending at 900

function run_program() {
    export procs=$1
    wk=$2

    set -x
    # Run the program on a fixed work size, then scrape the amount of time output by the tracer.
    mpirun -c $procs $EXE $args -c 1 -W $2 2>/dev/null | grep "tock" | cut -d' ' -f3 | tail -n1
}

function run_with_num_cores() {
    echo "|core|width|time|" 
    maxcores=40
    for i in "$@"; do
        wk=$(echo "scale=10; ((1920 * 1080) / $maxcores) * $i" | bc)
	width=$(echo "scale=10; (sqrt($wk * (16 / 9) ))" | bc)

        t1=$(run_program $i $width)
        echo "|$i|$wk|$t1|"
    done
}

run_with_num_cores $(seq 1 40)
