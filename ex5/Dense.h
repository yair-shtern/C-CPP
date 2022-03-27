#ifndef C___PROJECT_DENSE_H
#define C___PROJECT_DENSE_H

#include "Activation.h"

class Dense {
 public:
  /**
   * Constructs Dense.
   * @param w weight matrix.
   * @param bias bias matrix.
   * @param act activation type.
   */
  Dense (const Matrix &w, const Matrix &bias, ActivationType act)
      : _weights (w), _bias (bias), _activation (Activation (act))
  {}
  /**
   * @return weight matrix.
   */
  Matrix get_weights () const
  { return _weights; }
  /**
   * @return bias matrix.
   */
  Matrix get_bias () const
  { return _bias; }
  /**
   * @return activation.
   */
  Activation get_activation () const
  { return _activation; }
  /**
   * Parenthesis operator. Applies the layer on input
   * and returns output matrix.
   * @return matrix by value.
   */
  Matrix operator() (const Matrix &mat) const;
 private:
  Matrix _weights;
  Matrix _bias;
  Activation _activation;
};
#endif //C___PROJECT_DENSE_H
