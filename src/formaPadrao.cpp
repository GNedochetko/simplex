#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../include/formaPadrao.hpp"

using namespace std;

FormaPadraoSimplex montarFormaPadrao(const ProblemaLinear& problema) {
    // Verificar se o tipo de objetivo é válido
    FormaPadraoSimplex formaPadrao;
    double fatorObjetivo = problema.tipoObjetivo == "max" ? -1.0 : 1.0;

    formaPadrao.variaveis = problema.variaveis;

    // Construir o vetor c para a função objetivo
    for (size_t i = 0; i < problema.variaveis.size(); i++) {
        const string& variavel = problema.variaveis[i];
        auto it = problema.coeficientesObjetivoPorVariavel.find(variavel);
        double coeficiente = it != problema.coeficientesObjetivoPorVariavel.end() ? it->second : 0.0;
        formaPadrao.c.push_back(fatorObjetivo * coeficiente);
        //ainda não foi adicionado as variáveis de folga aqui
    }

    // Determinar o tipo de folga para cada restrição e contar quantas folgas serão necessárias
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
        } else if (sinal == "=") {
            tipoFolga[i] = 0.0;
        } else {
            throw invalid_argument("Erro: sinal de restricao invalido para montar a forma padrao.");
        }
    }
    
    // Adicionar as variáveis de folga à lista de variáveis e ao vetor c
    for (int i = 0; i < quantidadeFolgas; i++) {
        formaPadrao.variaveis.push_back("f" + to_string(i + 1));
        formaPadrao.c.push_back(0.0);
    }
    
    // Construir a matriz A
    formaPadrao.A.assign(
        // Linhas correspondem as restrições
        problema.restricoes.size(),
        // Colunas correspondem as variáveis originais + variáveis de folga
        vector<double>(problema.variaveis.size() + quantidadeFolgas, 0.0)
    );
    // Construir o vetor b
    formaPadrao.b.assign(problema.restricoes.size(), 0.0);

    // A colunaFolga começa após as variáveis originais
    int colunaFolga = static_cast<int>(problema.variaveis.size());

    // Montagem de fato da matriz A e do vetor b
    for (size_t i = 0; i < problema.restricoes.size(); i++) {
        const Restricao& restricao = problema.restricoes[i];
        // O vetor b recebe o termo independente da restrição
        formaPadrao.b[i] = restricao.termoIndependente;

        // Preencher a matriz A com os coeficientes das variáveis originais
        for (size_t j = 0; j < problema.variaveis.size(); j++) {
            const string& variavel = problema.variaveis[j];
            auto it = restricao.coeficientesPorVariavel.find(variavel);
            if (it != restricao.coeficientesPorVariavel.end()) {
                formaPadrao.A[i][j] = it->second;
            }
        }

        // Adicionar as variáveis de folga à matriz A
        if (tipoFolga[i] != 0.0) {
            formaPadrao.A[i][colunaFolga] = tipoFolga[i];
            colunaFolga++;
        }
    }

    return formaPadrao;
}
