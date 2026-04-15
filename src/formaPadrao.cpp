#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../include/formaPadrao.hpp"

using namespace std;

FormaPadraoSimplex montarFormaPadrao(const ProblemaLinear& problema) {
    FormaPadraoSimplex formaPadrao;
    double fatorObjetivo = problema.tipoObjetivo == "max" ? -1.0 : 1.0;

    formaPadrao.variaveis = problema.variaveis;

    for (size_t i = 0; i < problema.variaveis.size(); i++) {
        const string& variavel = problema.variaveis[i];
        auto it = problema.coeficientesObjetivoPorVariavel.find(variavel);
        double coeficiente = it != problema.coeficientesObjetivoPorVariavel.end() ? it->second : 0.0;
        formaPadrao.c.push_back(fatorObjetivo * coeficiente);
    }

    vector<double> tipoFolga(problema.restricoes.size(), 0.0);
    int quantidadeFolgas = 0;

    for (size_t i = 0; i < problema.restricoes.size(); i++) {
        const string& sinal = problema.restricoes[i].sinal;

        if (sinal == "<=" || sinal == "<") {
            tipoFolga[i] = 1.0;
            quantidadeFolgas++;
        } else if (sinal == ">=" || sinal == ">") {
            tipoFolga[i] = -1.0;
            quantidadeFolgas++;
        } else if (sinal != "=") {
            throw invalid_argument("Sinal de restricao invalido para montar a forma padrao.");
        }
    }

    for (int i = 0; i < quantidadeFolgas; i++) {
        formaPadrao.variaveis.push_back("f" + to_string(i + 1));
        formaPadrao.c.push_back(0.0);
    }

    formaPadrao.A.assign(
        problema.restricoes.size(),
        vector<double>(problema.variaveis.size() + quantidadeFolgas, 0.0)
    );
    formaPadrao.b.assign(problema.restricoes.size(), 0.0);

    int colunaFolga = static_cast<int>(problema.variaveis.size());

    for (size_t i = 0; i < problema.restricoes.size(); i++) {
        const Restricao& restricao = problema.restricoes[i];
        formaPadrao.b[i] = restricao.termoIndependente;

        for (size_t j = 0; j < problema.variaveis.size(); j++) {
            const string& variavel = problema.variaveis[j];
            auto it = restricao.coeficientesPorVariavel.find(variavel);
            if (it != restricao.coeficientesPorVariavel.end()) {
                formaPadrao.A[i][j] = it->second;
            }
        }

        if (tipoFolga[i] != 0.0) {
            formaPadrao.A[i][colunaFolga] = tipoFolga[i];
            if (tipoFolga[i] == 1.0) {
                formaPadrao.indicesBase.push_back(colunaFolga);
            }
            colunaFolga++;
        }
    }

    return formaPadrao;
}
