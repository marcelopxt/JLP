#include "ObjectiveFunction.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include "Job.h"
#include <climits>
#include <limits>

using namespace std;

int objectiveFunction(const std::vector<Job> &solucao)
{
  
    vector<int> custo(t, 0); // Custo de troca por ferramenta

    for (size_t i = 0; i < solucao.size(); i++)
    {
        int id = solucao[i].toolSetId;
        if (id >= 0 && id < t)
            custo[id] = solucao[i].toolCoust;
    }
    

    int setups = 0;

    int u = 0; // quantidade de ferramentas atualmente carregadas
    vector<int> carregados(t, 0);
    vector<vector<int>> magazine(vector<vector<int>>(t, vector<int>())); // magazine de presença de ferramentas
    vector<vector<int>> prioridades(vector<vector<int>>(t, vector<int>()));

    vector<int> setsSize(t);
    int maxJobId = 0;
    int maxOpId = 0;

    for (size_t i = 0; i < solucao.size(); i++)
    {
        int id = solucao[i].toolSetId;
        if (id >= 0 && id < t)
            setsSize[id] = solucao[i].toolSetSize;
    }

    for (int j = 0; j < t; j++)
    {
        for (int k = 0; k < solucao.size(); k++)
        {
            magazine[j].push_back(0);
            prioridades[j].push_back(0);
        }
    }

    for (int j = 0; j < solucao.size(); ++j) // estágio do magazine
    {
        magazine[solucao[j].toolSetId][j] = 1;
    }

    // KTNS
    for (unsigned i = 0; i < t; ++i) // ferramenta
    {
        for (unsigned j = 0; j < solucao.size(); ++j) // operação
        {
            if (magazine[i][j] == 1) // se o estágio atual precisa daquela ferramenta, prioridade zero
                prioridades[i][j] = 0;
            else
            {
                int proxima = 0;
                bool usa = false;
                for (unsigned l = j + 1; l < solucao.size(); ++l)
                {
                    ++proxima;
                    if (magazine[i][l] == 1)
                    {
                        usa = true;
                        break;
                    }
                }
                if (usa)
                    prioridades[i][j] = proxima;
                else
                    prioridades[i][j] = -1;
            }
        }
    }

    // carrego o máximo possível

    for (size_t i = 0; i < solucao.size(); i++) // job
    {
        int id = solucao[i].toolSetId;
        if (id >= 0 && id < t)
        {
            if (carregados[id] == 0 && u + solucao[i].toolSetSize <= c)
            {
                u += solucao[i].toolSetSize;
                carregados[id] = 1;
            }
        }
    }

    for (size_t i = 0; i < solucao.size(); i++)
    {
        int idJob = solucao[i].idJob;
        bool troca = false;

        double tempoPai = 0.0;
        int idFerramenta = -1;
        if (carregados[solucao[i].toolSetId] == 0)
        {
            u += solucao[i].toolSetSize;
            carregados[solucao[i].toolSetId] = 1; // carrega a ferramenta necessária para a operação atual
            troca = true;

            while (u > c)
            {
                int maior = 0;
                int pMaior = -1;

                for (unsigned k = 0; k < (unsigned)t; ++k) // ferramentas
                {
                    // magazine[tool][operation] => magazine[k][i]
                    if (magazine[k][i] != 1) // matriz de ferramentas
                    {
                        if ((carregados[k] == 1) && (prioridades[k][i] == -1))
                        {
                            pMaior = k;
                            break;
                        }
                        else
                        {
                            if ((prioridades[k][i] > maior) && carregados[k] == 1)
                            {
                                maior = prioridades[k][i];
                                pMaior = k;
                            }
                        }
                    }
                }
                if (pMaior == -1)
                    return INT_MAX;
                carregados[pMaior] = 0;
                if (pMaior >= 0 && pMaior < (int)setsSize.size())
                    u -= setsSize[pMaior];
                idFerramenta = pMaior;
            }
        }
        if (troca)
        {
            if (idFerramenta >= 0 && idFerramenta < (int)custo.size())
                setups += custo[idFerramenta];
        }

    }

    return setups;
}