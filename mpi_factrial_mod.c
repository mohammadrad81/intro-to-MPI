#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <mpi.h>

int min(int a, int b);
int max(int a, int b);
long long sum_of_array(int* numbers, int count);
int factorial_mod(int n, int mod);
void parallel_factorial_mod_array(int* numbers, int count, int world_size);

int main(int argc, char** argv) {
    int process_Rank, size_Of_Cluster;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("I am rank: %d from machine: %s\n", process_Rank, processor_name);

    srand(0);
    const int NUMBER_COUNT = 6000;
    const int MINIMUM = 100000; // 10^5
    const int MAXIMUM = 1000000;// 10^6
    int range_length = MAXIMUM - MINIMUM;

    int numbers[NUMBER_COUNT];

    if(process_Rank == 0){ // Master
        for(int i = 0; i < NUMBER_COUNT; i++){
            numbers[i] = (rand() % range_length) + MINIMUM;
        }
        parallel_factorial_mod_array(numbers, NUMBER_COUNT, size_Of_Cluster);
        printf("sum of result: %lld\n", sum_of_array(numbers, NUMBER_COUNT));
    }

    else{ // Worker
        int worker_numbers_count;
        MPI_Recv(&worker_numbers_count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(numbers, worker_numbers_count, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i = 0; i < worker_numbers_count; i++){
            numbers[i] = factorial_mod(numbers[i], numbers[i] + 1);
        }
        printf("process rank %d: I finished my calculations\n", process_Rank);
        MPI_Send(numbers, worker_numbers_count, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

int min(int a, int b){
    if (a < b){
        return a;
    }
    return b;
}

int max(int a, int b){
    if (a < b){
        return b;
    }
    return a;
}

long long sum_of_array(int* numbers, int count){
    long long sum = 0;
    for(int i = 0; i < count; i++){
        sum += numbers[i];
    }
    return sum;
}

int factorial_mod(int n, int mod){
    int result = 1;
    for(int i = 2; i <= n; i++){
        result *= i;
        result %= mod;
    }
    return result;
}

void parallel_factorial_mod_array(int* numbers, int count, int world_size){
    int chunk_size = count / world_size;

    for(int i = 1; i < world_size; i++){ //distribute work between workers
        int start = (i * chunk_size);
        int end;
        if (i == world_size - 1){
            end = count;
        }
        else{
            end = (i + 1) * chunk_size;
        }
        int chunk_count = end - start;
        MPI_Send(&chunk_count, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        MPI_Send(&numbers[start], chunk_count, MPI_INT, i, 1, MPI_COMM_WORLD);
        printf("Master: rank %d must calculate from %d to %d\n", i, start, end);
    }

    for(int i = 0; i < chunk_size; i++){ // master should work too :)
        numbers[i] = factorial_mod(numbers[i], numbers[i] + 1);
    }
    printf("Master: I finished my calculations\n");

    for(int i = 1; i < world_size; i++){
        int start = (i * chunk_size);
        int end;
        if (i == world_size - 1){
            end = count;
        }
        else{
            end = (i + 1) * chunk_size;
        }
        int chunk_count = end - start;
        MPI_Recv(&numbers[start], chunk_count, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Master: rank %d result received\n", i);
    }

}