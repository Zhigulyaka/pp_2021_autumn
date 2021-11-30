// Copyright 2021 Chelebaev Artem
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <cctype>
#include <iostream>
#include <algorithm>
#include "../../../modules/task_1/chelebaev_a_symbol_frequency/symbol_frequency.h"




//--------------------------------------------------------------

double parFrequency(char* symbol, std::string text) {
    int my_rank, tasks, ccount = 0;    // variables for the rank of ongoing process,
                                       // number of processes and symbol count respectively

    double freq = 0;    // variable representing the frequency of a desired symbol
    int N = text.length();    // length of the incoming string

    MPI_Comm_size(MPI_COMM_WORLD, &tasks);    // stating the number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    // stating the rank of ongoing process

    if (N < tasks) {    // in case there is less data than the number of processes, we terminate the program
        throw "There is less data than the number of processes!";
    }
    int k = N / tasks;    // distribute the data between processes
    int i1 = k * my_rank;    // state the first index

    int i2 = k * (my_rank + 1);    // state second index

    int count = 0;    // number of symbol entries in the string

    if (my_rank == (tasks - 1)) {    // if current process is the last one, change the second index
                                    // so that the last process gets a bit more data than the others
        i2 = N;
    }

    for (int i = i1; i < i2; i++) {    // look for symbol entries
        if (text[i] == *symbol || text[i] == *symbol - 32) {
            count++;
        }
    }
    // std::cout << "thread " << my_rank << " count: " << count << std::endl;

    MPI_Reduce(&count, &ccount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {    // if current process is the first one, compute the frequency
        // std::cout << "ccount: " << ccount << std::endl;
        freq = (static_cast<double>(ccount) / static_cast<double>(N)) * 100;
        // std::cout << "freq = " << freq << std::endl;
    }
    return freq;
}

double seqFrequency(char* symbol, std::string text) {
    int N = text.length();
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (text[i] == *symbol || text[i] == *symbol - 32) {
            count++;
        }
    }
    double freq = (static_cast<double>(count) / static_cast<double>(N)) * 100;
    return freq;
}

//-----------------------------------------------------------------


