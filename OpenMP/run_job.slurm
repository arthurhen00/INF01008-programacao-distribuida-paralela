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

mkdir -p inputs results vtune

# -------------------------------
# Loop 0: inputs
# -------------------------------
for N in 100 1000 10000
do
    python3 generate_case.py $START_TEMP $DECAY_RATE $SEED $N $INPUT_FILE
done

# -------------------------------
# Loop 1: sim_anneal (single-thread)
# -------------------------------
for N in 100 1000 10000
do
    INPUT_FILE="inputs/case_${N}.txt"
    echo "Running sim_anneal on N=$N"
    ./sim_anneal $INPUT_FILE 1
done

# -------------------------------
# Loop 2: sim_anneal_parallel (multi-thread)
# -------------------------------
for N in 100 1000 10000
do
    INPUT_FILE="inputs/case_${N}.txt"

    for t in 1 3 5 10 20
    do
        export OMP_NUM_THREADS=$t
        echo "Running sim_anneal_parallel with OMP_NUM_THREADS=$t on N=$N"
        ./sim_anneal_parallel $INPUT_FILE $t
    done
done

# -------------------------------
# Loop 3: sim_anneal_seed_parallel (multi-thread, multiple seeds)
# -------------------------------
for N in 100 1000 10000
do
    INPUT_FILE="inputs/case_${N}.txt"

    for t in 1 3 5 10 20
    do
        export OMP_NUM_THREADS=$t
        echo "Running sim_anneal_seed_parallel with OMP_NUM_THREADS=$t on N=$N"
        ./sim_anneal_seed_parallel $INPUT_FILE $t
    done
done

# vtune -collect hpc-performance -result-dir vtune/SEED-${SEED}-INPUTS-${N}-THREADS=${t} -- ./sim_anneal_parallel $INPUT_FILE > "results/parallel_SEED=${SEED}_INPUTS=${N}_THREADS=${t}.txt"