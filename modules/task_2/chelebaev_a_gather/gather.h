// Copyright 2021 Chelebaev Artem
#ifndef MODULES_TASK_2_CHELEBAEV_A_GATHER_GATHER_H_
#define MODULES_TASK_2_CHELEBAEV_A_GATHER_GATHER_H_

#include <mpi.h>
#include <vector>
#include <string>

template <class T>
std::vector<T> getRandomVector(int size);

int Gather(void *sbuf, int scount, MPI_Datatype stype, void *rbuf, int rcount,
MPI_Datatype rtype, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_CHELEBAEV_A_GATHER_GATHER_H_
