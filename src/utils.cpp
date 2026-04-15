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

vector<vector<double>> inversaMatriz(const vector<vector<double>>& matriz) {
    double determinante = determinanteLaplace(matriz);
    if(abs(determinante) < 1e-9) {
        cout << "Erro: A matriz é singular e não possui inversa." << endl;
        return {};
    }
    size_t n = matriz.size();
    vector<vector<double>> identidade(n, vector<double>(n, 0.0));
    for(size_t i = 0; i < identidade[0].size(); i++){
        for(size_t j = 0; j < identidade.size(); j++){
            if(i == j){
                identidade[i][j] = 1;
            }else{
                identidade[i][j] = 0;
            }
        }
    }
    vector<vector<double>> matrizAumentada = matriz;
    for (size_t i = 0; i < matriz.size(); i++) {
        matrizAumentada[i].insert(matrizAumentada[i].end(), identidade[i].begin(), identidade[i].end());
    }
    for(size_t i = 0; i < matrizAumentada.size(); i++){
        size_t pivo = i;
        for(size_t j = 0; j < matrizAumentada.size(); j++){
            if(i != j){
                double fator = matrizAumentada[j][pivo] / matrizAumentada[pivo][pivo];
                for(size_t k = 0; k < matrizAumentada[0].size(); k++){
                    matrizAumentada[j][k] -= fator * matrizAumentada[pivo][k];
                }
            }
        }
    }
    for(size_t i = 0; i < matrizAumentada.size(); i++){
        double pivo = matrizAumentada[i][i];
        for(size_t j = 0; j < matrizAumentada[0].size(); j++){
            matrizAumentada[i][j] /= pivo;
        }
    }
    vector<vector<double>> inversa(n, vector<double>(n, 0.0));
    for(size_t i = 0; i < inversa.size(); i++){
        for(size_t j = 0; j < inversa[i].size(); j++){
            inversa[i][j] = matrizAumentada[i][j + n];
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
