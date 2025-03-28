#!/usr/bin/bash
#SBATCH --nodes=8
#SBATCH --ntasks-per-node=16
#SBATCH --time=0:45:00
#SBATCH --mail-type=FAIL
#SBATCH --job-name liam-mpi-strong

FINAL=$1
EXE=$1/release/main
cd $SLURM_SUBMIT_DIR

source $FINAL/scripts/niagarasetup

if [ ! -f $EXE ]; then
    echo "Forgot to compile final, was looking for: '$EXE'"
    echo "Maybe try adding an argument on your sbatch invocation"
    echo "sbatch path/to/script path/to/final"
    exit 1
fi

#Pretty grind-y settings.
args="-i $FINAL/data/squares.jpg -T -s 10"

function run_program() {
    export procs=$1

    # Run the program on a fixed work size, then scrape the amount of time output by the tracer.
    mpirun -c $procs $EXE $args -c 4 2>/dev/null | grep "tock" | cut -d' ' -f3 | tail -n1
}

function run_with_num_cores() {
    echo "|core|time|" 
    for i in "$@"; do
        t1=$(run_program $i)
        echo "|$i|$t1|"
    done
}

echo "Ready to begin"
set -x
run_with_num_cores $(seq 1 8)
echo "Done!"
