#!/bin/bash -l
# The -l above is required to get the full environment with modules

#SBATCH -J myjob
#SBATCH -t 00:05:00
#SBATCH -A edu24.DD2356
#SBATCH -p shared
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=2
#SBATCH --nodes=1
#SBATCH -e error_file.e

# Run the executable file 
# and write the output into my_output_file
srun -n 1 perf stat ./myexecutable.out ./profiling/results1.txt