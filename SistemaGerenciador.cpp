

#include "SistemaGerenciador.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>
using namespace std;

SistemaGerenciador::SistemaGerenciador(const string &csv, const string &dados, const string &indice) : arquivoCSV(csv), arquivoDados(dados), arquivoIndice(indice) {}

void SistemaGerenciador::iniciar() {
  int escolha;
    do {
        cout << "Sistema de Gerenciamento de Alunos" << endl;
        cout << "-----------------------------------" << endl;
        cout << "1. Gerar Arquivo de Dados a partir do CSV" << endl;
        cout << "2. Gerar Arquivo de Indice" << endl;
        cout << "3. Buscar Aluno por Matricula" << endl;
        cout << "4. Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> escolha;

        switch (escolha) {
            case 1:
                gerarArquivoDados();
                break;
            case 2:
                gerarArquivoIndice();
                break;
            case 3:
                buscarRegistroPorMatricula();
                break;
            case 4:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
        }
        cout << endl;
    } while (escolha != 4);
}

void SistemaGerenciador::escreverRegistro(ofstream &out, const Aluno &aluno) {
    if (!out.is_open()) {
        throw runtime_error("Arquivo de dados não está aberto para escrita.");
    }
    out.write(reinterpret_cast<const char *>(&aluno), sizeof(Aluno));
}

bool SistemaGerenciador::lerRegistro(ifstream &in, Aluno &aluno, long offset) {
    if (!in.is_open()) {
        throw runtime_error("Arquivo de dados não está aberto para leitura.");
    }
    if (in.seekg(offset)) {
        in.read(reinterpret_cast<char *>(&aluno), sizeof(Aluno));
        return in.gcount() == sizeof(Aluno);
    }
    return false;
}

void SistemaGerenciador::gerarArquivoDados() {
    ifstream arquivoCSV(this->arquivoCSV, ios::binary);
    if (!arquivoCSV.is_open()) {
        cerr << "Erro ao abrir o arquivo CSV: " << this->arquivoCSV<< endl;
        return;
    }

    ofstream arquivoDados(this->arquivoDados, ios::binary);
    if (!arquivoDados.is_open()) {
        cerr << "Erro ao criar o arquivo de dados: " << this->arquivoDados<< endl;
        return;
    }

    string linha;
    // Pula cabeçalho
    if (!getline(arquivoCSV, linha)) {
        cerr << "Arquivo CSV vazio ou erro ao ler o cabeçalho." << endl;
        return;
    }

    while (getline(arquivoCSV, linha)) {
        stringstream ss(linha);
        string campo;
        Aluno aluno;
        // Matrícula
        getline(ss, campo, ',');
        aluno.matricula = stoi(campo);

        // Nome
        getline(ss, campo, ',');
        strncpy(aluno.nome, campo.c_str(), sizeof(aluno.nome) - 1);
        aluno.nome[sizeof(aluno.nome) - 1] = '\0'; // Garante terminação nula

        // Curso
        getline(ss, campo, ',');
        strncpy(aluno.curso, campo.c_str(), sizeof(aluno.curso) - 1);
        aluno.curso[sizeof(aluno.curso) - 1] = '\0'; // Garante terminação nula

        // Escreve o registro do aluno no arquivo de dados
        escreverRegistro(arquivoDados, aluno);
    }
    arquivoCSV.close();
    arquivoDados.close();
    return;
}

void quickSort(vector<Indice>& indices, int low, int high) {
    if (low < high) {
        int pivot = indices[high].matricula;
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (indices[j].matricula < pivot) {
                i++;
                swap(indices[i], indices[j]);
            }
        }
        swap(indices[i + 1], indices[high]);
        int pi = i + 1;

        quickSort(indices, low, pi - 1);
        quickSort(indices, pi + 1, high);
    }
}

void SistemaGerenciador::gerarArquivoIndice() {
    Aluno aluno;
    vector<Indice> indices;
    Indice indice;
    ifstream arquivoDados(this->arquivoDados, ios::binary);
    if (!arquivoDados.is_open()) {
        cerr << "Erro ao abrir o arquivo de dados: " << this->arquivoDados<< endl;
        return;
    }
    long offset = 0;
    while (lerRegistro(arquivoDados, aluno, offset)) {
        indice.matricula = aluno.matricula;
        indice.byte_Offset = offset;
        indices.push_back(indice);
        offset += sizeof(Aluno);
    }

    quickSort(indices, 0, indices.size() - 1);

    // Resto da implementação
    ofstream arquivoIndice(this->arquivoIndice, ios::binary);
    if (!arquivoIndice.is_open()) {
        cerr << "Erro ao criar o arquivo de índice: " << this->arquivoIndice
                            << endl;
        return;
    }

    arquivoIndice.write(reinterpret_cast<const char *>(indices.data()),indices.size() * sizeof(Indice));

    arquivoDados.close();
    arquivoIndice.close();
}

int binarySearch(const vector<Indice>& indices, int target) {
    int left = 0;
    int right = indices.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (indices[mid].matricula == target) {
            return mid; // Encontrou o índice
        }
        else if (indices[mid].matricula < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1; // Não encontrado
}

void SistemaGerenciador::buscarRegistroPorMatricula() {
    ifstream ArquivoIndice(this->arquivoIndice,ios::binary);
    if (!ArquivoIndice.is_open()) {
        cerr << "Erro ao abrir o arquivo de índice: " << this->arquivoIndice<< endl;
        return;
    }
    // Carrega o vetor de indices
    long tamanhoArquivo;
    ArquivoIndice.seekg(0, ios::end);
    tamanhoArquivo = ArquivoIndice.tellg();
    ArquivoIndice.seekg(0);
    long vecQuant = tamanhoArquivo / sizeof(Indice);
    vector<Indice> indices(vecQuant);
    ArquivoIndice.read(reinterpret_cast<char *>(indices.data()), tamanhoArquivo);

    cout << "Insira uma Matrícula para busca: ";
    string input;
    cin >> input;

    int search = stoi(input);
    int found = binarySearch(indices, search);
    
    if (found == -1) {
        cout << "Matrícula " << search << " não encontrada." << endl;
    }else{
        ifstream ArquivoDados(this->arquivoDados, ios::binary);
        if (!ArquivoDados.is_open()) {
            cerr << "Erro ao abrir o arquivo de dados: " << this->arquivoDados<< endl;
            return;
        }
        Aluno aluno;
        long offset = indices[found].byte_Offset;
        if (lerRegistro(ArquivoDados, aluno, offset)) {
            cout << "Registro encontrado:" << endl;
            cout << "Matrícula: " << aluno.matricula << endl;
            cout << "Nome: " << aluno.nome << endl;
            cout << "Curso: " << aluno.curso << endl;
        } else {
            cout << "Erro ao ler o registro no arquivo de dados." << endl;
        }
        ArquivoDados.close();
    }


    
    ArquivoIndice.close();
    return;
}