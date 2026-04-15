#include <iostream>
#include <vector>
#include <fstream>
#include <cctype>
#include "../include/arquivo.hpp"

using namespace std;

ProblemaLinear lerArquivo(const string& nomeArquivo) {
    ProblemaLinear problema;
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return problema;
    }
    string linha;
    getline(arquivo, linha);

    if(linha.find("max") != string::npos) {
        problema.tipoObjetivo = "max";
    } else if(linha.find("min") != string::npos) {
        problema.tipoObjetivo = "min";
    } else {
        cout << "Erro: Tipo de objetivo não encontrado (max ou min)." << endl;
        return problema;
    }

    size_t pos = linha.find("=");
    if (pos == string::npos) {
        cout << "Erro: '=' nao encontrado na funcao objetivo." << endl;
        return problema;
    }

    for (size_t i = pos + 1; i < linha.size();) {
        while (i < linha.size() && isspace(static_cast<unsigned char>(linha[i]))) {
            i++;
        }

        if (i >= linha.size()) {
            break;
        }

        int sinal = 1;
        if (linha[i] == '+') {
            i++;
        } else if (linha[i] == '-') {
            sinal = -1;
            i++;
        }

        while (i < linha.size() && isspace(static_cast<unsigned char>(linha[i]))) {
            i++;
        }

        string coeficiente;
        while (i < linha.size() &&
               (isdigit(static_cast<unsigned char>(linha[i])) || linha[i] == '.')) {
            coeficiente += linha[i];
            i++;
        }

        double valorCoeficiente = coeficiente.empty() ? 1.0 : stod(coeficiente);
        valorCoeficiente *= sinal;

        while (i < linha.size() && isspace(static_cast<unsigned char>(linha[i]))) {
            i++;
        }

        if (i >= linha.size() || !isalpha(static_cast<unsigned char>(linha[i]))) {
            cout << "Erro: variavel invalida na funcao objetivo." << endl;
            return problema;
        }

        string variavel;
        while (i < linha.size() &&
               (isalpha(static_cast<unsigned char>(linha[i])) ||
                isdigit(static_cast<unsigned char>(linha[i])))) {
            variavel += linha[i];
            i++;
        }

        problema.coeficientesObjetivo.push_back(valorCoeficiente);
        problema.variaveis.push_back(variavel);
        problema.coeficientesObjetivoPorVariavel[variavel] = valorCoeficiente;
    }

    while(getline(arquivo, linha)) {
        size_t posSinal;
        string sinal;
        Restricao restricao;

        if ((posSinal = linha.find("<=")) != string::npos) {
            sinal = "<=";
        } else if ((posSinal = linha.find(">=")) != string::npos) {
            sinal = ">=";
        } else if ((posSinal = linha.find("=")) != string::npos) {
            sinal = "=";
        } else if ((posSinal = linha.find("<")) != string::npos) {
            sinal = "<";
        } else if ((posSinal = linha.find(">")) != string::npos) {
            sinal = ">";
        } else {
            cout << "Erro: Sinal de restricao nao encontrado." << endl;
            return problema;
        }

        restricao.sinal = sinal;
        string parteEsquerda = linha.substr(0, posSinal);
        string parteDireita = linha.substr(posSinal + sinal.size());

        for (size_t i = 0; i < parteEsquerda.size();) {
            while (i < parteEsquerda.size() && isspace(static_cast<unsigned char>(parteEsquerda[i]))) {
                i++;
            }

            if (i >= parteEsquerda.size()) {
                break;
            }

            int sinal = 1;
            if (parteEsquerda[i] == '+') {
                i++;
            } else if (parteEsquerda[i] == '-') {
                sinal = -1;
                i++;
            }

            while (i < parteEsquerda.size() && isspace(static_cast<unsigned char>(parteEsquerda[i]))) {
                i++;
            }

            string coeficiente;
            while (i < parteEsquerda.size() &&
                   (isdigit(static_cast<unsigned char>(parteEsquerda[i])) || parteEsquerda[i] == '.')) {
                coeficiente += parteEsquerda[i];
                i++;
            }

            double valorCoeficiente = coeficiente.empty() ? 1.0 : stod(coeficiente);
            valorCoeficiente *= sinal;

            while (i < parteEsquerda.size() && isspace(static_cast<unsigned char>(parteEsquerda[i]))) {
                i++;
            }

            if (i >= parteEsquerda.size() || !isalpha(static_cast<unsigned char>(parteEsquerda[i]))) {
                cout << "Erro: variavel invalida na restricao." << endl;
                return problema;
            }

            string variavel;
            while (i < parteEsquerda.size() &&
                   (isalpha(static_cast<unsigned char>(parteEsquerda[i])) ||
                    isdigit(static_cast<unsigned char>(parteEsquerda[i])))) {
                variavel += parteEsquerda[i];
                i++;
            }
            restricao.coeficientes.push_back(valorCoeficiente);
            restricao.coeficientesPorVariavel[variavel] = valorCoeficiente;
        }
        try {
            restricao.termoIndependente = stod(parteDireita);
        } catch (const invalid_argument& e) {
            cout << "Erro: termo independente invalido na restricao." << endl;
            return problema;
        }
        problema.restricoes.push_back(restricao);
    }
    return problema;
}
