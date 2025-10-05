#!/bin/bash
#SBATCH --job-name=sim_anneal_test
#SBATCH --partition=hype
#SBATCH --ntasks=1
#SBATCH --time=120:00
#SBATCH --output=results/%x_%j.out
#SBATCH --error=results/%x_%j.err

make clean
make

START_TEMP=9000000
DECAY_RATE=0.999
SEED=919
INPUT_SIZES=(100 1000 10000 100000)
THREADS=(1 3 5 10 20)

mkdir -p inputs results vtune

# -------------------------------
# Loop 0: inputs
# -------------------------------
for N in "${INPUT_SIZES[@]}"
do
    INPUT_FILE="inputs/case_${N}.txt"
    python3 generate_case.py $START_TEMP $DECAY_RATE $SEED $N $INPUT_FILE
done

# -------------------------------
# Loop 1: sim_anneal (single-thread)
# -------------------------------
for N in "${INPUT_SIZES[@]}"
do
    INPUT_FILE="inputs/case_${N}.txt"
    echo "Running sim_anneal on N=$N"
     ./sim_anneal $INPUT_FILE 1
    #vtune -collect hpc-performance -result-dir vtune/sim_anneal_SEED_${SEED}_INPUTS_${N}_THREADS_${t} -- ./sim_anneal $INPUT_FILE $t
done

# -------------------------------
# Loop 2: sim_anneal_parallel (multi-thread)
# -------------------------------
for N in "${INPUT_SIZES[@]}"
do
    INPUT_FILE="inputs/case_${N}.txt"

    for t in "${THREADS[@]}"
    do
        export OMP_NUM_THREADS=$t
        echo "Running sim_anneal_parallel with OMP_NUM_THREADS=$t on N=$N"
        ./sim_anneal_parallel $INPUT_FILE $t
        #vtune -collect hpc-performance -result-dir vtune/sim_anneal_parallel_SEED_${SEED}_INPUTS_${N}_THREADS_${t} -- ./sim_anneal_parallel $INPUT_FILE $t
    done
done

# -------------------------------
# Loop 3: sim_anneal_seed_parallel (multi-thread, multiple seeds)
# -------------------------------
for N in "${INPUT_SIZES[@]}"
do
    INPUT_FILE="inputs/case_${N}.txt"

    for t in "${THREADS[@]}"
    do
        export OMP_NUM_THREADS=$t
        echo "Running sim_anneal_seed_parallel with OMP_NUM_THREADS=$t on N=$N"
        ./sim_anneal_seed_parallel $INPUT_FILE $t
        #vtune -collect hpc-performance -result-dir vtune/sim_anneal_seed_parallel_SEED_${SEED}_INPUTS_${N}_THREADS_${t} -- ./sim_anneal_seed_parallel $INPUT_FILE $t
    done
done

