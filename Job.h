#ifndef JOB_H
#define JOB_H

class Job {
public: 
    int idJob;           // Identificador do job 
    int toolSetId;       // Identificador do conjunto de ferramentas
    int toolSetSize;     // Tamanho do conjunto de ferramentas
    int toolCoust;

    Job();
    Job(int idOp, int toolSetId, int toolSetSize, int toolCoust);
};

#endif 
