//
// Created by yair.shtern on 04/06/2021.
//

#include "Activation.h"

/**
 * Makes RELU operation.
 * @return result matrix by value.
 */
Matrix make_relu_act (const Matrix &mat)
{
  Matrix matrix (mat.get_rows (), mat.get_cols ());
  for (int i = 0; i < mat.get_rows () * mat.get_cols (); i++)
    {
      if (mat[i] >= 0)
        {
          matrix[i] = mat[i];
        }
      else
        {
          matrix[i] = 0;
        }
    }
  return matrix;
}
/**
 * Makes SOFTMAX operation.
 * @return result matrix by value.
 */
Matrix make_softmax_act (const Matrix &mat)
{
  Matrix matrix (mat.get_rows (), mat.get_cols ());
  float factor = 0;
  for (int i = 0; i < mat.get_rows () * mat.get_cols (); i++)
    {
      factor += std::exp (mat[i]);
    }
  factor = 1 / factor;
  for (int i = 0; i < mat.get_rows () * mat.get_cols (); i++)
    {
      matrix[i] = factor * std::exp (mat[i]);
    }
  return matrix;
}
/**
 * Parenthesis operator. make activation according to activation type.
 * @return matrix by value.
 */
Matrix Activation::operator() (const Matrix &mat) const
{
  Matrix matrix;
  if (_activation == RELU)
    {
      matrix = make_relu_act (mat);
    }
  if (_activation == SOFTMAX)
    {
      matrix = make_softmax_act (mat);
    }
  return matrix;
}
