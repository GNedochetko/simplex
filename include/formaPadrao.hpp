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
};

FormaPadraoSimplex montarFormaPadrao(const ProblemaLinear& problema);

#endif
