#!/usr/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --time=0:30:00
#SBATCH --mail-type=FAIL
#SBATCH --job-name liam-traffic-weak

EXE=$1/Traffic_EduHPC-23/Code/traffic
TF=$1/Traffic_EduHPC-23/Code/testfiles/

if [ ! -f $EXE ]; then
    echo "Forgot to compile traffic, was looking for: '$A4'"
    echo "Maybe try adding an argument on your sbatch invocation"
    echo "sbatch path/to/script path/to/a4"
    exit 1
fi


function run_program() {
    export OMP_NUM_THREADS=$1
    # Run the program on a fixed work size, then scrape the amount of time output by the tracer.
    $EXE $TF$1.ini | grep "Tock" | cut -d' ' -f4
}

function run_with_num_cores() {
    echo "|core|time|" 
    for i in "$@"; do
        t1=$(run_program $i)
        echo "|$i|$t1|"
    done
}

module load gcc/13.2.0

echo "Ready to begin"
run_with_num_cores $(seq 1 16)
echo "Done!"
