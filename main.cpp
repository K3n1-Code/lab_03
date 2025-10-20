#include "SistemaGerenciador.h"

int main() {
  // Define os nomes dos arquivos
  SistemaGerenciador sistema("alunos.csv", "alunos.dat", "indices.dat");

  // Inicia a execucao do sistema (menu, etc.)
  sistema.iniciar();

  return 0;
}