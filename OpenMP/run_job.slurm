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
SEEDS=(919 1234 4321)
FST_SEED=${SEEDS[0]}

mkdir -p inputs results vtune

for N in 100 1000 10000 100000
do
    INPUT_FILE="inputs/case_${N}.txt"
    python3 generate_case.py $START_TEMP $DECAY_RATE $FST_SEED $N $INPUT_FILE
    for SEED in "${SEEDS[@]}"
    do

        sed -i "3s/.*/seed=${SEED}/" "$INPUT_FILE"

        ./sim_anneal $INPUT_FILE > "results/normal_SEED=${SEED}_INPUTS=${N}.txt"

        for t in 1 2 4 8 16
        do
            export OMP_NUM_THREADS=$t
            echo "Running with OMP_NUM_THREADS=$t on N=$N, SEED=$SEED"
            vtune -collect hpc-performance -result-dir vtune/SEED-${SEED}-INPUTS-${N}-THREADS=${t} -- ./sim_anneal_parallel $INPUT_FILE > "results/parallel_SEED=${SEED}_INPUTS=${N}_THREADS=${t}.txt"
            #./sim_anneal_parallel $INPUT_FILE > "results/parallel_SEED=${SEED}_INPUTS=${N}_THREADS=${t}.txt"
        done
    done
done
