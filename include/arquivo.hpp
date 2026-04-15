#ifndef ARQUIVO_HPP
#define ARQUIVO_HPP

#include <map>
#include <string>
#include <vector>

struct Restricao {
    std::vector<double> coeficientes;
    std::map<std::string, double> coeficientesPorVariavel;
    std::string sinal;
    double termoIndependente;
};

struct ProblemaLinear {
    std::string tipoObjetivo;
    std::vector<std::string> variaveis;
    std::vector<double> coeficientesObjetivo;
    std::map<std::string, double> coeficientesObjetivoPorVariavel;
    std::vector<Restricao> restricoes;
};

ProblemaLinear lerArquivo(const std::string& nomeArquivo);

#endif
