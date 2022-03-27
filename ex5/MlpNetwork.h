//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Matrix.h"
#include "Digit.h"
#include "Activation.h"
#include "Dense.h"

#define MLP_SIZE 4

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

class MlpNetwork {
 public:
  /**
   * Constructs MlpNetwork.
   * @param weights an array of weight matrices.
   * @param biases an array of bias matrices.
   */
  MlpNetwork (Matrix weights[], Matrix biases[])
      : _weights (weights), _biases (biases)
  {}
  /**
   * Parenthesis operator. Applies the entire network on mat.
   * @return digit struct with the result.
   */
  digit operator() (const Matrix &mat) const;
 private:
  Matrix *_weights;
  Matrix *_biases;
};

#endif // MLPNETWORK_H
