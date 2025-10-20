#include "Aluno.h"
#include "Indice.h"
#include <string>

class SistemaGerenciador {
public:
  // Construtor e método principal do menu
  SistemaGerenciador(const std::string &csv, const std::string &dados,
                     const std::string &indice);
  void iniciar();

private:
  // Métodos de alto nível para as funcionalidades
  void gerarArquivoDados();
  void gerarArquivoIndice();
  void buscarRegistroPorMatricula();

  // --- Métodos privados para I/O (inspirados na classe Buffer) ---
  void escreverRegistro(std::ofstream &out, const Aluno &aluno);
  bool lerRegistro(std::ifstream &in, Aluno &aluno, long offset);

private:
  std::string arquivoCSV;
  std::string arquivoDados;
  std::string arquivoIndice;
};
