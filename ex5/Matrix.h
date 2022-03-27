// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims {
    int rows, cols;
} matrix_dims;

class Matrix {
 public:
  /**
   * Constructs Matrix rows × cols Inits all elements to 0.
   * @param rows rows of the Matrix.
   * @param cols cols of the Matrix.
   */
  Matrix (int rows, int cols);
  /**
   * Constructs 1×1 Matrix Inits the single element to 0.
   */
  Matrix () : Matrix (1, 1)
  {}
  /**
   * Copy construct, constructs matrix from another Matrix mat.
   * @param mat matrix to copy from.
   */
  Matrix (const Matrix &mat);
  /**
   * Destructor.
   */
  ~Matrix ()
  { delete[] _matrix; }
  /**
   * @return returns the amount of rows as int.
   */
  int get_rows () const
  { return _rows; }
  /**
   * returns the amount of cols as int.
   */
  int get_cols () const
  { return _cols; }
  /**
   * Transforms a matrix into its transpose matrix.
   * @return the transposed matrix.
   */
  Matrix &transpose ();
  /**
   * Transforms a matrix into a column vector.
   */
  Matrix &vectorize ();
  /**
   * Prints matrix elements, no return value.
   */
  void plain_print () const;
  /**
   * Returns a matrix which is the dot product of
   * this matrix and another matrix m.
   * @param mat to dot with.
   * @return the dot matrix.
   */
  Matrix dot (const Matrix &mat) const;
  /**
   * @return Returns the Frobenius norm of the given matrix.
   */
  float norm () const;
  /**
   * Reads into a given matrix from std::istream.
   * @param is std::istream arg to read from.
   * @param mat matrix to write in.
   * @return std::istream.
   */
  friend std::istream &read_binary_file (std::istream &in, Matrix &mat);
  /**
   * + operator. Gets 2 matrix and adds the i element, for all elements.
   * @return the result matrix by value.
   */
  friend Matrix operator+ (const Matrix &mat1, const Matrix &mat2);
  /**
   * Assignment operator.
   * @return matrix by reference.
   */
  Matrix &operator= (const Matrix &mat);
  /**
   * Multiplication operator.
   * @return matrix by value.
   */
  friend Matrix operator* (const Matrix &mat1, const Matrix &mat2);
  /**
   * Scalar multiplication on the right.
   * @param c multiplication mat by c.
   * @return matrix by value.
   */
  friend Matrix operator* (const Matrix &mat, float c);
  /**
   * Scalar multiplication on the left.
   * @param c multiplication mat by c.
   * @return matrix by value.
   */
  friend Matrix operator* (float c, const Matrix &mat);
  /**
   * Addition accumulation operator.
   * @return matrix by reference.
   */
  Matrix &operator+= (const Matrix &mat);
  /**
   * Parenthesis indexing operator.
   * @return by reference object in row i col j.
   */
  float &operator() (int i, int j);
  /**
   * Parenthesis indexing operator.
   * @return by value object in row i col j.
  */
  float operator() (int i, int j) const;
  /**
   * Brackets indexing.
   * @return by reference object i in matrix.
   */
  float &operator[] (int i);
  /**
   * Brackets indexing.
   * @return by value object i in matrix.
   */
  float operator[] (int i) const;
  /**
   * Prints struct of the matrix.
   * @return Output stream.
   */
  friend std::ostream &operator<< (std::ostream &os, const Matrix &mat);

 private:
  int _rows;
  int _cols;
  float *_matrix;

};
#endif //MATRIX_H
