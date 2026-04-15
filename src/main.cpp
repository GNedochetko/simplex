#include <iostream>
#include "../include/arquivo.hpp"
#include "../include/formaPadrao.hpp"

 using namespace std;

 int main(){
    ProblemaLinear problema = lerArquivo("data/entrada.txt");
    FormaPadraoSimplex formaPadrao = montarFormaPadrao(problema);

    cout << "Tipo de objetivo: " << problema.tipoObjetivo << endl;
    cout << "Variaveis: ";
    for (const auto& var : problema.variaveis) {
        cout << var << " "; 
    }
    cout << endl;
    cout << "Coeficientes da funcao objetivo: ";
    for (const auto& coef : problema.coeficientesObjetivo) {
        cout << coef << " ";
    }
    cout << endl;   
    cout << "Mapa da funcao objetivo:" << endl;
    for (const auto& [variavel, coeficiente] : problema.coeficientesObjetivoPorVariavel) {
        cout << variavel << " -> " << coeficiente << endl;
    }
    cout << "Restricoes:" << endl;
    for (const auto& restricao : problema.restricoes) {
        for (const auto& coef : restricao.coeficientes) {
            cout << coef << " ";
        }
        cout << restricao.sinal << " " << restricao.termoIndependente << endl;
        for (const auto& [variavel, coeficiente] : restricao.coeficientesPorVariavel) {
            cout << variavel << " -> " << coeficiente << endl;
        }
    }

    cout << "Variaveis da matriz A: ";
    for (const auto& variavel : formaPadrao.variaveis) {
        cout << variavel << " ";
    }
    cout << endl;

    cout << "Matriz A:" << endl;
    for (const auto& linha : formaPadrao.A) {
        for (const auto& valor : linha) {
            cout << valor << " ";
        }
        cout << endl;
    }

    cout << "Vetor b: ";
    for (const auto& valor : formaPadrao.b) {
        cout << valor << " ";
    }
    cout << endl;

    cout << "Vetor c: ";
    for (const auto& valor : formaPadrao.c) {
        cout << valor << " ";
    }
    cout << endl;

    return 0;
 }
