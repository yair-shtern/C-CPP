//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H
#include "Matrix.h"
/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType {
    RELU,
    SOFTMAX
};

class Activation {
 public:
  /**
   * Constructs Activation.
   * @param act_type ActivationTape.
   */
  Activation (ActivationType act_type) : _activation (act_type)
  {}
  /**
   * @return activation type.
   */
  ActivationType get_activation_type () const
  { return _activation; }
  /**
   * Parenthesis operator. make activation according to activation type.
   * @return matrix by value.
   */
  Matrix operator() (const Matrix &mat) const;
 private:
  ActivationType _activation;
};

#endif //ACTIVATION_H
