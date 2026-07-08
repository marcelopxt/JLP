#ifndef OBJECTIVEFUNCTION_H
#define OBJECTIVEFUNCTION_H

#include "Job.h"
#include <string>
#include <vector>    

extern int o;
extern int t;
extern int c;

int objectiveFunction(const std::vector<Job> &maquina);

#endif