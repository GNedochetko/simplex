#ifndef FUNCOES_HPP
#define FUNCOES_HPP

#include <vector>

double determinanteLaplace(const std::vector<std::vector<double>>& matriz);

std::vector<std::vector<double>> inversaMatriz(const std::vector<std::vector<double>>& matriz);

void multiplicaMatrizes(const std::vector<std::vector<double>>& M, const std::vector<std::vector<double>>& N, std::vector<std::vector<double>>& resultado);

#endif
