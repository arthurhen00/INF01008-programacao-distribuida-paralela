#!/bin/bash
#SBATCH --job-name=mpi_coletiva
#SBATCH --partition=hype
#SBATCH --nodes=2
#SBATCH --ntasks=40
#SBATCH --time=2:00:00
#SBATCH --output=logs/%x_%j.out
#SBATCH --error=logs/%x_%j.err

N=2048

mkdir -p logs

MACHINEFILE="logs/nodes.$SLURM_JOB_ID"
srun -l hostname | sort -n | awk '{print $2}' > $MACHINEFILE

mpirun -np $SLURM_NTASKS \
       -machinefile $MACHINEFILE \
       --mca btl ^openib \
       --mca btl_tcp_if_include eno2 \
       --bind-to none -np $SLURM_NTASKS \
       ./mpi_coletiva $N
