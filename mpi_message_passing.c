#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int process_Rank, size_Of_Cluster, message_Item;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    printf("number of processes: %d\n", size_Of_Cluster);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    if(process_Rank == 0){
        message_Item = 42;
        MPI_Send(&message_Item, // the message
                 1,             // count
                 MPI_INT,       // data type
                 1,             // destination process id
                 1,             // tag for message
                 MPI_COMM_WORLD);
        printf("Machine: %s, Message Sent: %d\n", processor_name, message_Item);
    }

    else if(process_Rank == 1){
        MPI_Recv(&message_Item, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Machine: %s, Message Received: %d\n", processor_name, message_Item);
    }

    MPI_Finalize();
    return 0;
}