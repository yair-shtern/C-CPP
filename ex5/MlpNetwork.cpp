//
// Created by yair.shtern on 04/06/2021.
//

#include "MlpNetwork.h"
#define RESULT_MAT_SIZE 10


/**
 * Finds the value with the highest probability.
 * @return digit struct with the value found and its probability.
 */
digit get_result_digit (const Matrix &mat)
{
  unsigned int val_result = 0;
  float prob_result = mat[0];

  for (int i = 1; i < RESULT_MAT_SIZE; i++)
    {
      if (mat[i] > prob_result)
        {
          val_result = i;
          prob_result = mat[i];
        }
    }
  digit result = {val_result, prob_result};
  return result;
}
/**
 * Parenthesis operator. Applies the entire network on mat.
 * @return digit struct with the result.
 */
digit MlpNetwork::operator() (const Matrix &mat) const
{
  Matrix output_matrix(mat);
  for (int i = 0; i < MLP_SIZE; i++)
    {
      if (i < MLP_SIZE - 1)
        {
          Dense dense (_weights[i], _biases[i], RELU);
          output_matrix = dense (output_matrix);
        }
      else
        {
          Dense dense (_weights[MLP_SIZE - 1],
                       _biases[MLP_SIZE - 1], SOFTMAX);
          output_matrix = dense (output_matrix);
        }
    }
  return get_result_digit (output_matrix);
}


