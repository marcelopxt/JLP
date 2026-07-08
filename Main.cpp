
#include <iostream>
#include <climits>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include "Job.h"
#include "ObjectiveFunction.h"
#include <chrono>
#include <deque>
#include <random>

using namespace std;
using namespace std::chrono;

std::ofstream fileSolution;
int o, t, c;  // número de jobs, ferramentas, capacidade
high_resolution_clock::time_point t1;
std::chrono::high_resolution_clock::duration tempo_execucao;

// Variáveis globais para armazenar dados da instância
std::vector<int> tamanhoFerramentas;  // tamanho de cada ferramenta
std::vector<int> custoPorFerramentas; // custo de troca de cada ferramenta
std::vector<std::vector<int>> matrizRelacao;  // matriz ferramenta x job

std::vector<Job> lerInstancia(const std::string& nomeArquivo)
{
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir arquivo: " << nomeArquivo << std::endl;
        return std::vector<Job>();
    }

    arquivo >> o >> t >> c;

    tamanhoFerramentas.clear();
    custoPorFerramentas.clear();
    matrizRelacao.clear();

    tamanhoFerramentas.resize(t);
    for (int i = 0; i < t; ++i)
    {
        arquivo >> tamanhoFerramentas[i];
    }

    custoPorFerramentas.resize(t);
    for (int i = 0; i < t; ++i)
    {
        arquivo >> custoPorFerramentas[i];
    }

    matrizRelacao.resize(t);
    for (int i = 0; i < t; ++i)
    {
        matrizRelacao[i].resize(o);
        for (int j = 0; j < o; ++j)
        {
            arquivo >> matrizRelacao[i][j];
        }
    }

    arquivo.close();

    // Cria vector<Job> onde cada job armazena seu ID e as informações da ferramenta atrelada
    std::vector<Job> vetJob;
    for (int jobId = 0; jobId < o; ++jobId)
    {
        // Encontra a primeira ferramenta válida para este job (onde matriz[ferramenta][job] == 1)
        int toolId = -1;
        for (int ferrId = 0; ferrId < t; ++ferrId)
        {
            if (matrizRelacao[ferrId][jobId] == 1)
            {
                toolId = ferrId;
                break;
            }
        }
        
        // Se encontrou uma ferramenta válida, cria o job com suas informações
        if (toolId != -1)
        {
            Job job(jobId, toolId, tamanhoFerramentas[toolId], custoPorFerramentas[toolId]);
            vetJob.push_back(job);
        }
    }

    return vetJob;
}

std::vector<Job> gerarSolucao(std::vector<Job> vetJob)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vetJob.begin(), vetJob.end(), g);
    return vetJob;
}

int parseHeaderValue(string line)
{
    replace(line.begin(), line.end(), ',', ' ');
    stringstream ss(line);
    ss.imbue(locale("C"));
    string key;
    int value;
    ss >> key >> value;
    return value;
}

int main(int argsc, char *argv[])
{
    if (argsc < 3)
    {
        std::cerr << "Uso: ./Main <arquivo_saida> <arquivo_instancia>" << std::endl;
        return 1;
    }

    ios_base::sync_with_stdio(false);
    cin.imbue(locale("C"));

    fileSolution.open(argv[1]);
    if (!fileSolution.is_open())
    {
        std::cerr << "Erro ao criar arquivo de saida: " << argv[1] << std::endl;
        return 1;
    }

    // Lê a instância do arquivo
    std::vector<Job> vetJob = lerInstancia(argv[2]);
    if (vetJob.empty())
    {
        std::cerr << "Falha ao carregar instância." << std::endl;
        return 1;
    }

    t1 = high_resolution_clock::now();

    double sol_inicial = INT_MAX;

    while (sol_inicial == INT_MAX)
    {
        vetJob = gerarSolucao(vetJob);
        sol_inicial = objectiveFunction(vetJob);
    }
    

    tempo_execucao = high_resolution_clock::now() - t1;

    // Extrai o nome da instância do caminho do arquivo
    std::string instancePath(argv[2]);
    size_t lastSlash = instancePath.find_last_of("/\\");
    std::string instanceName = (lastSlash == std::string::npos) ? instancePath : instancePath.substr(lastSlash + 1);

    fileSolution
        << "Instance,Jobs,Tools,Magazine,Solution,ExecutionTime(s)" << endl
        << instanceName << ","
        << o << ","
        << t << ","
        << c << ","
        << sol_inicial << ","
        << tempo_execucao.count() << endl;

    fileSolution.close();

    return 0;
}