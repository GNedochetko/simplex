#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <stdexcept>
#include "simplex.hpp"
#include "utils.hpp"

using namespace std;

vector<vector<double>> montarMatrizPorColunas(const vector<vector<double>>& matriz, const vector<int>& indicesColunas) {
    // Verificar se a matriz está vazia
    if (matriz.empty()) {
        return {};
    }

    // Criar uma nova matriz para armazenar as colunas selecionadas
    vector<vector<double>> submatriz(matriz.size(), vector<double>(indicesColunas.size(), 0.0));

    //Preencher a submatriz com os valores das colunas selecionadas
    for (size_t i = 0; i < matriz.size(); i++) {
        for (size_t j = 0; j < indicesColunas.size(); j++) {
            int indiceColuna = indicesColunas[j];

            // O índice da coluna será invalido se for negativo ou maior ou igual ao número de colunas da matriz original
            if (indiceColuna < 0 || indiceColuna >= static_cast<int>(matriz[i].size())) {
                throw out_of_range("Indice de coluna invalido ao montar submatriz.");
            }

            // Preencher a submatriz com o valor da coluna correspondente da matriz original
            submatriz[i][j] = matriz[i][indiceColuna];
        }
    }

    return submatriz;
}

bool matrizEhInversivel(const vector<vector<double>>& matriz) {
    //verificação se a matriz é quadrada e invertível
    if (matriz.empty() || matriz.size() != matriz[0].size()) {
        return false;
    }

    return std::abs(determinanteLaplace(matriz)) > 1e-9;
}

bool baseJaFoiSorteada(const vector<int>& base, const vector<vector<int>>& basesJaSorteadas) {
    // Verificar se a base já foi sorteada comparando com as bases já sorteadas
    return find(basesJaSorteadas.begin(), basesJaSorteadas.end(), base) != basesJaSorteadas.end();
}

long long contarBasesPossiveis(int quantidadeVariaveis, int quantidadeRestricoes) {
    // Verificar se a quantidade de restrições é válida para o número de variáveis
    if (quantidadeRestricoes < 0 || quantidadeRestricoes > quantidadeVariaveis) {
        return 0;
    }

    // Calcular o número de combinações possíveis de escolher 'quantidadeRestricoes' colunas dentre 'quantidadeVariaveis' colunas
    int k = min(quantidadeRestricoes, quantidadeVariaveis - quantidadeRestricoes);
    // Esse numero pode ser grande...
    long long total = 1;
    for (int i = 1; i <= k; i++) {
        // O número de combinações é dado por C(n, k) = n! / (k! * (n - k)!)
        // Para evitar cálculos de fatorial, podemos usar a fórmula multiplicativa: C(n, k) = (n * (n - 1) * ... * (n - k + 1)) / (k * (k - 1) * ... * 1)
        total = (total * (quantidadeVariaveis - k + i)) / i;
    }

    return total;
}

vector<int> sortearBaseCandidata(int quantidadeVariaveis, int quantidadeRestricoes, mt19937& gerador) {
    // Gerar uma permutação aleatória dos índices das variáveis
    vector<int> indices(quantidadeVariaveis);
    iota(indices.begin(), indices.end(), 0);

    // Embaralhar os índices para obter uma combinação aleatória de colunas para a base inicial
    shuffle(indices.begin(), indices.end(), gerador);

    // Selecionar os primeiros 'quantidadeRestricoes' índices para formar a base inicial
    vector<int> base(indices.begin(), indices.begin() + quantidadeRestricoes);
    // Ordenar os índices da base para facilitar a comparação com bases já sorteadas
    sort(base.begin(), base.end());
    return base;
}

bool possuiValorNegativo(const vector<double>& valores) {
    return any_of(valores.begin(), valores.end(), [](double valor) {
        return valor < 0.0;
    });
}

vector<int> sortearBaseInicial(const vector<vector<double>>& matriz, const vector<double>& b) {
    if (matriz.empty()) {
        return {};
    }

    // Pega o número de restrições e variáveis da matriz A para determinar o número de colunas disponíveis para a base inicial  
    int quantidadeRestricoes = static_cast<int>(matriz.size());
    int quantidadeVariaveis = static_cast<int>(matriz[0].size());

    if (quantidadeRestricoes > quantidadeVariaveis) {
        throw runtime_error("Nao ha colunas suficientes para montar uma base inicial.");
    }

    random_device rd;
    mt19937 gerador(rd());

    vector<vector<int>> basesJaSorteadas;
    long long totalBasesPossiveis = contarBasesPossiveis(quantidadeVariaveis, quantidadeRestricoes);

    while (static_cast<long long>(basesJaSorteadas.size()) < totalBasesPossiveis) {
        // Sortear uma base candidata
        vector<int> base = sortearBaseCandidata(quantidadeVariaveis, quantidadeRestricoes, gerador);
        // Verificar se a base já foi sorteada para evitar repetições
        if (baseJaFoiSorteada(base, basesJaSorteadas)) {
            continue;
        }

        basesJaSorteadas.push_back(base);

        // Montar a matriz B a partir da matriz A usando os índices da base sorteada
        vector<vector<double>> B = montarMatrizPorColunas(matriz, base);
        if (!matrizEhInversivel(B)) {
            continue;
        }

        // Calcular x_B = B^(-1) * b para verificar se a solução básica é viável (x_B >= 0)
        vector<double> xB = calcularXB(B, b);
        if (!possuiValorNegativo(xB)) {
            return base;
        }
    }

    throw runtime_error("Nenhuma base aleatoria gerou uma matriz B inversivel e viavel.");
}

vector<double> extrairCustos(const vector<double>& c, const vector<int>& indices) {
    vector<double> custosExtraidos;
    custosExtraidos.reserve(indices.size());

    // Pelos indices fornecidos, extrair os custos correspondentes do vetor c 
    for (int indice : indices) {
        if (indice < 0 || indice >= static_cast<int>(c.size())) {
            throw out_of_range("Indice invalido ao extrair custos.");
        }

        custosExtraidos.push_back(c[indice]);
    }

    return custosExtraidos;
}

vector<double> calcularXB(const vector<vector<double>>& B, const vector<double>& b) {
    if (B.empty()) {
        return {};
    }

    if (B.size() != b.size()) {
        throw runtime_error("As dimensoes de B e b sao incompativeis para calcular x_B.");
    }

    // Calcular a inversa de B
    vector<vector<double>> inversaB = inversaMatriz(B);
    if (inversaB.empty()) {
        throw runtime_error("Nao foi possivel calcular x_B porque B nao possui inversa.");
    }

    // Transformar o vetor b em uma matriz coluna para multiplicação
    vector<vector<double>> bColuna(b.size(), vector<double>(1, 0.0));
    for (size_t i = 0; i < b.size(); i++) {
        bColuna[i][0] = b[i];
    }

    // Calcular x_B = B^(-1) * b
    vector<vector<double>> xBColuna(B.size(), vector<double>(1, 0.0));
    multiplicaMatrizes(inversaB, bColuna, xBColuna);

    // Converter a matriz coluna xB de volta para um vetor
    vector<double> xB(b.size(), 0.0);
    for (size_t i = 0; i < xB.size(); i++) {
        xB[i] = xBColuna[i][0];
    }

    return xB;
}

vector<double> calcularCustosReduzidos(const vector<vector<double>>& N, const vector<vector<double>>& inversaB, const vector<double>& cN, const vector<double>& cB) {
    if (N.empty()) {
        return {};
    }

    if (inversaB.empty()) {
        throw runtime_error("A matriz inversa de B nao pode ser vazia.");
    }

    if (cB.size() != inversaB.size()) {
        throw runtime_error("As dimensoes de cB e inversaB sao incompativeis.");
    }

    if (cN.size() != N[0].size()) {
        throw runtime_error("As dimensoes de cN e N sao incompativeis.");
    }

    if (inversaB.size() != N.size()) {
        throw runtime_error("As dimensoes de inversaB e N sao incompativeis.");
    }

    
    vector<vector<double>> cBTransposto(1, cB);
    vector<vector<double>> lambTransposto(1, vector<double>(inversaB[0].size(), 0.0));
    // Calcular lamb^T = c_B^T * B^(-1), que é o vetor multiplicador do simplex
    multiplicaMatrizes(cBTransposto, inversaB, lambTransposto);    

    // Calcular o produto lamb^T * N, cada coluna de N é multiplicada por lamb^T
    vector<vector<double>> produtoLambN(1, vector<double>(N[0].size(), 0.0));
    multiplicaMatrizes(lambTransposto, N, produtoLambN);

    // Calcular os custos reduzidos para as variáveis não básicas: c_N - lamb^T * N
    vector<double> custosReduzidos(cN.size(), 0.0);
    for (size_t j = 0; j < cN.size(); j++) {
        custosReduzidos[j] = cN[j] - produtoLambN[0][j];
    }

    return custosReduzidos;
}

int determinarEntradaBase(const vector<double>& custosReduzidos) {
    int posicaoEntrada = 0;

    // A posição que tem o menor custo reduzido vai ser escolhida para entrar na base
    for (size_t i = 1; i < custosReduzidos.size(); i++) {
        if (custosReduzidos[i] < custosReduzidos[posicaoEntrada]) {
            posicaoEntrada = i;
        }
    }

    return posicaoEntrada;
}

int determinarSaidaBase(const vector<double>& xB, const vector<vector<double>>& direcao) {
    if (xB.size() != direcao.size()) {
        throw runtime_error("As dimensoes de x_B e da direcao sao incompativeis.");
    }

    int posicaoSaida = -1;
    double menorRazao = 0.0;

    for (size_t i = 0; i < xB.size(); i++) {
        if (direcao[i][0] <= 0.0) {
            continue;
        }

        // Calcula a razão e se ela for menor do que as já calculadas, vira a nova candidata para troca
        double razao = xB[i] / direcao[i][0];
        if (posicaoSaida == -1 || razao < menorRazao) {
            menorRazao = razao;
            posicaoSaida = i;
        }
    }

    return posicaoSaida;
}

void simplex(const FormaPadraoSimplex& formaPadrao){
    vector<int> indicesBase = sortearBaseInicial(formaPadrao.A, formaPadrao.b);
    vector<int> indicesNaoBase;
    for (size_t i = 0; i < formaPadrao.variaveis.size(); i++) {
        if (find(indicesBase.begin(), indicesBase.end(), i) == indicesBase.end()) {
            indicesNaoBase.push_back(static_cast<int>(i));
        }
    }

    vector<vector<double>> A = formaPadrao.A;
    vector<double> b = formaPadrao.b;
    vector<double> c = formaPadrao.c;

    while(true){
        // Montar as matrizes B e N a partir da matriz A usando os índices das variáveis básicas e não básicas
        vector<vector<double>> B = montarMatrizPorColunas(A, indicesBase);
        vector<vector<double>> N = montarMatrizPorColunas(A, indicesNaoBase);

        // Calcular x_B = B^(-1) * b
        vector<double> xB = calcularXB(B, b);

        // Extrair os custos correspondentes às variáveis básicas e não básicas
        vector<double> cB = extrairCustos(c, indicesBase);
        vector<double> cN = extrairCustos(c, indicesNaoBase);

        // Inversa de B para calculos futuros
        vector<vector<double>> inversaB = inversaMatriz(B);

        // Custos reduzidos para as variáveis não básicas: c_N - lamb^T * N
        vector<double> custosReduzidos = calcularCustosReduzidos(N, inversaB, cN, cB);

        // Calcula a variavel que vai entrar na B
        int posicaoEntrada = determinarEntradaBase(custosReduzidos);
        int indiceEntrada = indicesNaoBase[posicaoEntrada];

        // Se o valor do custo reduzido na posição da variável que vai entrar em B for maior que 0, o loop se encerra
        if (custosReduzidos[posicaoEntrada] >= 0.0) {
            vector<double> solucao(c.size(), 0.0);

            for (size_t i = 0; i < indicesBase.size(); i++) {
                solucao[indicesBase[i]] = xB[i];
            }

            double valorObjetivo = 0.0;

            for (size_t i = 0; i < c.size(); i++) {
                valorObjetivo += c[i] * solucao[i];
            }

            cout << "Solucao otima encontrada:" << endl;

            for (size_t i = 0; i < solucao.size(); i++) {
                cout << formaPadrao.variaveis[i] << " = " << solucao[i] << endl;
            }

            cout << "Valor objetivo na forma padrao: " << valorObjetivo << endl;

            break;
        }

        // Calcular a direcao simplex: y = B^(-1) * A_Nk
        vector<vector<double>> colunaEntrada = montarMatrizPorColunas(A, {indiceEntrada});
        vector<vector<double>> direcao(colunaEntrada.size(), vector<double>(1, 0.0));
        multiplicaMatrizes(inversaB, colunaEntrada, direcao);

        // Determinar qual variavel sai da base pelo teste da razao minima
        int posicaoSaida = determinarSaidaBase(xB, direcao);
        if (posicaoSaida == -1) {
            throw runtime_error("Problema ilimitado: nenhuma variavel pode sair da base.");
        }

        // Enfim ocorre a troca das variaveis...
        swap(indicesBase[posicaoSaida], indicesNaoBase[posicaoEntrada]);
    }
}
