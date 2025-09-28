#!/bin/bash
#SBATCH --job-name=test
#SBATCH --partition=hype
#SBATCH --nodes=1
#SBATCH --ntasks=2
#SBATCH --time=15:00
#SBATCH --output=%x_%j.out
#SBATCH --error=%x_%j.err

make
./sim_anneal 9000000 0.999 919 > "results_normal.txt"
./sim_anneal_parallel 9000000 0.999 919 > "results_parallel.txt"