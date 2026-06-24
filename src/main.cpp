#include <exception>
#include <iostream>
#include "../include/arquivo.hpp"
#include "../include/formaPadrao.hpp"
#include "../include/simplex.hpp"

using namespace std;

int main() {
    try {
        ProblemaLinear problema = lerArquivo("data/entrada.txt");
        normalizarRestricoes(problema);
        bool precisaFase1 = fase1EhNecessaria(problema);
        FormaPadraoSimplex formaPadrao = montarFormaPadrao(problema);
    
        if (precisaFase1) {
            cout << "Fase I necessaria." << endl;
            FormaPadraoSimplex problemaArtificial = formaPadrao;
            adicionarVariaveisArtificiais(problemaArtificial);

            ResultadoSimplex resultadoFase1 = executarFase1(problemaArtificial);
            if (resultadoFase1.problemaInviavel) {
                cout << "Problema infactivel." << endl;
                return 0;
            }

            cout << "Fase I concluida. Solucao basica viavel encontrada." << endl;
            cout << "Valor objetivo artificial: " << resultadoFase1.valorObjetivo << endl;
            cout << "Executando Fase II..." << endl;

            vector<int> baseFase2 = prepararBaseFase2(formaPadrao, resultadoFase1.indicesBase);
            ResultadoSimplex resultadoFase2 = executarSimplex(formaPadrao, baseFase2);
            imprimirResultadoSimplex(formaPadrao, resultadoFase2);
            return 0;
        } else {
            cout << "Fase I nao necessaria. Indo direto para a Fase II." << endl;
        }

        cout << "Executando Fase II..." << endl;
        simplex(formaPadrao);

        return 0;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}
