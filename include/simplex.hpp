#ifndef SIMPLEX_HPP
#define SIMPLEX_HPP

#include "formaPadrao.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <random>

std::vector<std::vector<double>> montarMatrizPorColunas(
    const std::vector<std::vector<double>>& matriz,
    const std::vector<int>& indicesColunas
);

bool matrizEhInversivel(const std::vector<std::vector<double>>& matriz);

bool baseJaFoiSorteada(
    const std::vector<int>& base,
    const std::vector<std::vector<int>>& basesJaSorteadas
);

long long contarBasesPossiveis(int quantidadeVariaveis, int quantidadeRestricoes);

std::vector<int> sortearBaseCandidata(
    int quantidadeVariaveis,
    int quantidadeRestricoes,
    std::mt19937& gerador
);

std::vector<int> sortearBaseInicial(
    const std::vector<std::vector<double>>& matriz,
    const std::vector<double>& b
);

std::vector<double> extrairCustos(
    const std::vector<double>& custos,
    const std::vector<int>& indices
);

std::vector<double> calcularXB(
    const std::vector<std::vector<double>>& B,
    const std::vector<double>& b
);

std::vector<double> calcularCustosReduzidos(
    const std::vector<std::vector<double>>& N,
    const std::vector<std::vector<double>>& inversaB,
    const std::vector<double>& cN,
    const std::vector<double>& cB
);

int determinarEntradaBase(const std::vector<double>& custosReduzidos);

int determinarSaidaBase(
    const std::vector<double>& xB,
    const std::vector<std::vector<double>>& direcao
);

void simplex(const FormaPadraoSimplex& formaPadrao);

#endif 
