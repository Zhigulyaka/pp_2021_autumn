// Copyright 2021 TexHik620953
#include <mpi.h>
#include <list>
#include <cstring>
#include <iostream>
#include <random>
#include "../../../modules/task_2/rodionov_n_broadcast_message/broadcast_message.h"


int BroadcastSum(
    void* sendbuf,
    void* outSum,
    int count,
    int root,
    MPI_Op op,
    MPI_Datatype type
) {
    int commSize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == root) {
        for (int i = 0; i < commSize; i++) {
            if (i != root) {
                MPI_Send(sendbuf, count * commSize, type, i, 0, MPI_COMM_WORLD);
            }
        }
    }
    if (type == MPI_DOUBLE) {
        double* recvbuf = nullptr;
        if (rank != root) {
            recvbuf = new double[count * commSize];
            MPI_Status status{};
            MPI_Recv(recvbuf, count * commSize, type,
                root, 0, MPI_COMM_WORLD, &status);
        } else {
            recvbuf = reinterpret_cast<double*>(sendbuf);
        }
        // Now all processes has same data in recvbuffer
        // Calculate summ
        double local_val = 0;
        if (op == MPI_SUM) {
            for (int i = 0; i < count; i++) {
                local_val += recvbuf[rank * count + i];
            }
        } else if (op == MPI_MAX) {
            for (int i = 0; i < count; i++) {
                if (recvbuf[rank * count + i] > local_val) {
                    local_val = recvbuf[rank * count + i];
                }
            }
        }
        if (rank != root) {
            // Send data to root process
            MPI_Send(&local_val, 1, type, root, 1, MPI_COMM_WORLD);
        }

        // Receive and summ data
        if (rank == root) {
            double global_val = local_val;
            // Receive data from non-root
            for (int i = 0; i < commSize; i++) {
                if (i != root) {
                    double process_val = 0;
                    MPI_Status status{};
                    MPI_Recv(&process_val, 1, type,
                        i, 1, MPI_COMM_WORLD, &status);
                    if (op == MPI_SUM) {
                        global_val += process_val;
                    } else if (op == MPI_MAX) {
                        if (process_val > global_val) {
                            global_val = process_val;
                        }
                    }
                }
            }

            *reinterpret_cast<double*>(outSum) = global_val;
        }
    } else if (type == MPI_FLOAT) {
        float* recvbuf = nullptr;
        if (rank != root) {
            recvbuf = new float[count * commSize];
            MPI_Status status{};
            MPI_Recv(recvbuf, count * commSize, type,
                root, 0, MPI_COMM_WORLD, &status);
        } else {
            recvbuf = reinterpret_cast<float*>(sendbuf);
        }
        // Now all processes has same data in recvbuffer
        // Calculate summ
        float local_val = 0;
        if (op == MPI_SUM) {
            for (int i = 0; i < count; i++) {
                local_val += recvbuf[rank * count + i];
            }
        } else if (op == MPI_MAX) {
            for (int i = 0; i < count; i++) {
                if (recvbuf[rank * count + i] > local_val) {
                    local_val = recvbuf[rank * count + i];
                }
            }
        }
        if (rank != root) {
            // Send data to root process
            MPI_Send(&local_val, 1, type, root, 1, MPI_COMM_WORLD);
        }

        // Receive and summ data
        if (rank == root) {
            float global_val = local_val;
            // Receive data from non-root
            for (int i = 0; i < commSize; i++) {
                if (i != root) {
                    float process_val = 0;
                    MPI_Status status{};
                    MPI_Recv(&process_val, 1, type,
                        i, 1, MPI_COMM_WORLD, &status);
                    if (op == MPI_SUM) {
                        global_val += process_val;
                    } else if (op == MPI_MAX) {
                        if (process_val > global_val) {
                            global_val = process_val;
                        }
                    }
                }
            }

            *reinterpret_cast<float*>(outSum) = global_val;
        }
    } else if (type == MPI_INT) {
    int* recvbuf = nullptr;
    if (rank != root) {
        recvbuf = new int[count * commSize];
        MPI_Status status{};
        MPI_Recv(recvbuf, count * commSize, type,
            root, 0, MPI_COMM_WORLD, &status);
    } else {
        recvbuf = reinterpret_cast<int*>(sendbuf);
    }
    // Now all processes has same data in recvbuffer
    // Calculate summ
    int local_val = 0;
    if (op == MPI_SUM) {
        for (int i = 0; i < count; i++) {
            local_val += recvbuf[rank * count + i];
        }
    } else if (op == MPI_MAX) {
        for (int i = 0; i < count; i++) {
            if (recvbuf[rank * count + i] > local_val) {
                local_val = recvbuf[rank * count + i];
            }
        }
    }
    if (rank != root) {
        // Send data to root process
        MPI_Send(&local_val, 1, type,
            root, 1, MPI_COMM_WORLD);
    }

    // Receive and summ data
    if (rank == root) {
        int global_val = local_val;
        // Receive data from non-root
        for (int i = 0; i < commSize; i++) {
            if (i != root) {
                int process_val = 0;
                MPI_Status status{};
                MPI_Recv(&process_val, 1, type, i,
                    1, MPI_COMM_WORLD, &status);
                if (op == MPI_SUM) {
                    global_val += process_val;
                } else if (op == MPI_MAX) {
                    if (process_val > global_val) {
                        global_val = process_val;
                    }
                }
            }
        }

        *reinterpret_cast<int*>(outSum) = global_val;
    }
    }
    return 0;
}

int randint(int min, int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

