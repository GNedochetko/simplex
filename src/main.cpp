#include <exception>
#include <iostream>
#include "../include/arquivo.hpp"
#include "../include/formaPadrao.hpp"
#include "../include/simplex.hpp"

using namespace std;

int main() {
    try {
        ProblemaLinear problema = lerArquivo("data/entrada.txt");
        FormaPadraoSimplex formaPadrao = montarFormaPadrao(problema);

        cout << "Executando simplex..." << endl;
        simplex(formaPadrao);

        return 0;
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
}
