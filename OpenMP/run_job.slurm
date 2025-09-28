#!/bin/bash
#SBATCH --job-name=sim_anneal_test
#SBATCH --partition=hype
#SBATCH --ntasks=1
#SBATCH --time=15:00
#SBATCH --output=results/%x_%j.out
#SBATCH --error=results/%x_%j.err

make clean
make

START_TEMP=9000000
DECAY_RATE=0.999
SEED=919

mkdir -p inputs results

for N in 100 1000 10000 100000
do
    INPUT_FILE="inputs/case_${N}.txt"

    python3 generate_case.py $START_TEMP $DECAY_RATE $SEED $N $INPUT_FILE

    for t in 1 2 4 8 16
    do
        export OMP_NUM_THREADS=$t
        echo "Running with OMP_NUM_THREADS=$t on N=$N"

        ./sim_anneal $INPUT_FILE > "results/normal_${t}_${N}.txt"
        ./sim_anneal_parallel $INPUT_FILE > "results/parallel_${t}_${N}.txt"
    done
done
