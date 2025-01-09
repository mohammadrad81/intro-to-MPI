# Introduction to MPI
This project is my cloud computing presentation at Amirkabir University of Technology

## commands

### compilation
`mpicc source.c -o compiled.out`

### execution using mpirun
`mpirun --host localhost:<NUMBER_OF_PROCESSES> compiled.out`

### execution using mpirun on multiple machines
`mpirun --host <WORKER_1_IP>:<NUMBER_OF_PROCESSES_FOR_WORKER_1>,<WORKER_2_IP>:<NUMBER_OF_PROCESSES_FOR_WORKER_2> compiled.out`

### execution using mpirun on multiple machines using hostfile
`mpirun -hostfile <HOSTFILE_NAME> compiled.out`

## Notice
communicating processes must be on machines that can ssh to eachother without password, so you have to add ssh-keys to them.
