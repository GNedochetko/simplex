#ifndef FUNCOES_SIMPLEX_HPP
#define FUNCOES_SIMPLEX_HPP

#include "arquivo.hpp"
#include <string>
#include <vector>

struct FormaPadraoSimplex {
    std::vector<std::vector<double>> A;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<std::string> variaveis;
    std::string tipoObjetivoOriginal;
    std::vector<int> indicesArtificiais;
};

bool fase1EhNecessaria(const ProblemaLinear& problema);

void normalizarRestricoes(ProblemaLinear& problema);

FormaPadraoSimplex montarFormaPadrao(const ProblemaLinear& problema);

void adicionarVariaveisArtificiais(FormaPadraoSimplex& formaPadrao);

#endif
