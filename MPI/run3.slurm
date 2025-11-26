#!/bin/bash
#SBATCH --job-name=mpi_coletiva_ag_3
#SBATCH --partition=hype
#SBATCH --nodes=3
#SBATCH --ntasks=60
#SBATCH --time=6:00:00
#SBATCH --output=logs/%x_%j.out
#SBATCH --error=logs/%x_%j.err

SRC_DIR="src"

SRC_LIST=(
    "mpi_coletiva.c"
    "mpi_p2p_bloqueante.c"
    "mpi_p2p_naobloqueante.c"
)

BIN_LIST=(
    "mpi_coletiva"
    "mpi_p2p_bloq"
    "mpi_p2p_nb"
)

SIZES=(
       512 
       1024 
       2048
       4096
       )

PROCS=(
#       1 
#       2 
#       4
#       5 
#       10 
#       20 
#       40
       60
#       80
       )

MACHINEFILE="logs/nodes.$SLURM_JOB_ID"
srun -l hostname | sort -n | awk '{print $2}' > $MACHINEFILE

echo "Compiling source files..."
for ((i=0; i<${#SRC_LIST[@]}; i++)); do
    mpicc -O3 -march=native -o ${BIN_LIST[$i]} $SRC_DIR/${SRC_LIST[$i]}
done

mkdir -p logs

REP_COUNT=5

for ((i=0; i<${#BIN_LIST[@]}; i++)); do
    PROGRAM=${BIN_LIST[$i]}

    for P in "${PROCS[@]}"; do
        for N in "${SIZES[@]}"; do

            echo "Running $PROGRAM with P=$P N=$N"

            for R in $(seq 1 $REP_COUNT); do

                OUTFILE="${PROGRAM}_results_3.csv"

                mpirun -np $P \
                  -machinefile $MACHINEFILE \
                  --mca btl ^openib \
                  --mca btl_tcp_if_include eno2 \
                  --bind-to none \
                  ./$PROGRAM $N \
                  >> "$OUTFILE"
            done
        done
    done
done
