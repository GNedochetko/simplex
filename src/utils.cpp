#include <cmath>
#include <iostream>
#include <vector>
#include "../include/utils.hpp"

using namespace std;

double determinanteLaplace(const vector<vector<double>>& matriz) {
    if(matriz.size() != matriz[0].size()) {
        cout << "Erro: A matriz deve ser quadrada para calcular o determinante." << endl;
        return 0;
    }
    if(matriz.size() == 1) {
        return matriz[0][0];
    }
    double det = 0;
    for(size_t j = 0; j < matriz[0].size(); j++) {
        vector<vector<double>> submatriz(matriz.size() - 1, vector<double>(matriz[0].size() - 1));
        for(size_t i = 1; i < matriz.size(); i++) {
            for(size_t k = 0; k < matriz[0].size(); k++) {
                if(k < j) {
                    submatriz[i - 1][k] = matriz[i][k];
                } else if(k > j) {
                    submatriz[i - 1][k - 1] = matriz[i][k];
                }
            }
        }
        det += (j % 2 == 0 ? 1 : -1) * matriz[0][j] * determinanteLaplace(submatriz);
    }
    return det;
}

vector<vector<double>> transpostaMatriz(const vector<vector<double>>& matriz) {
    if (matriz.empty()) {
        return {};
    }

    vector<vector<double>> transposta(matriz[0].size(), vector<double>(matriz.size(), 0.0));

    for (size_t i = 0; i < matriz.size(); i++) {
        for (size_t j = 0; j < matriz[i].size(); j++) {
            transposta[j][i] = matriz[i][j];
        }
    }

    return transposta;
}

vector<vector<double>> inversaMatriz(const vector<vector<double>>& matriz) {
    if (matriz.empty() || matriz.size() != matriz[0].size()) {
        cout << "Erro: A matriz deve ser quadrada para calcular a inversa." << endl;
        return {};
    }

    size_t n = matriz.size();
    vector<vector<double>> matrizAumentada(n, vector<double>(2 * n, 0.0));

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            matrizAumentada[i][j] = matriz[i][j];
        }

        matrizAumentada[i][n + i] = 1.0;
    }

    for (size_t coluna = 0; coluna < n; coluna++) {
        size_t linhaPivo = coluna;
        for (size_t linha = coluna + 1; linha < n; linha++) {
            if (abs(matrizAumentada[linha][coluna]) > abs(matrizAumentada[linhaPivo][coluna])) {
                linhaPivo = linha;
            }
        }

        if (abs(matrizAumentada[linhaPivo][coluna]) < 1e-9) {
            cout << "Erro: A matriz é singular e não possui inversa." << endl;
            return {};
        }

        if (linhaPivo != coluna) {
            swap(matrizAumentada[linhaPivo], matrizAumentada[coluna]);
        }

        double pivo = matrizAumentada[coluna][coluna];
        for (size_t j = 0; j < 2 * n; j++) {
            matrizAumentada[coluna][j] /= pivo;
        }

        for (size_t linha = 0; linha < n; linha++) {
            if (linha == coluna) {
                continue;
            }

            double fator = matrizAumentada[linha][coluna];
            for (size_t j = 0; j < 2 * n; j++) {
                matrizAumentada[linha][j] -= fator * matrizAumentada[coluna][j];
            }
        }
    }

    vector<vector<double>> inversa(n, vector<double>(n, 0.0));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            inversa[i][j] = matrizAumentada[i][n + j];
        }
    }

    return inversa;
}

void multiplicaMatrizes(const vector<vector<double>>& M, const vector<vector<double>>& N, vector<vector<double>>& resultado) {
    if(M[0].size() != N.size()) {
        cout << "Erro: O número de colunas da matriz M deve ser igual ao número de linhas da matriz N." << endl;
        return;
    }
    for(size_t i = 0; i < M.size(); i++) {
        for(size_t j = 0; j < N[0].size(); j++) {
            resultado[i][j] = 0;
            for(size_t k = 0; k < M[0].size(); k++) {
                resultado[i][j] += M[i][k] * N[k][j];
            }
        }
    }
}
