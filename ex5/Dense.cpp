//
// Created by yair shtern on 04/06/2021.
//

#include "Dense.h"

/**
 * Parenthesis operator. Applies the layer on input
 * and returns output matrix.
 * @return matrix by value.
 */
Matrix Dense::operator() (const Matrix &mat) const
{
  Matrix output_matrix = get_activation()((_weights * mat) + _bias);
  return output_matrix;
}