//
// Created by yair.shtern on 04/06/2021.
//

#include "Matrix.h"
#define INVALID_PARM_FOR_MATRIX "Error: Rows and cols should be natural > 0."
#define INVALID_MATRIX "Error: Num of rows / cols of "\
"the tow matrix not legal to operate."
#define FAIL_TO_READ "Error: Fails to read and fill al the matrix."
#define INDEX_OUT_OF_RANGE "Error: Index (row/col) out of range."
#define ALLOCATION_FAILURE "Error: Allocation was not successful."
#define FLOAT_SIZE 4
#define MIN_TO_PRINT 0.1

/**
 * Constructs Matrix rows × cols Inits all elements to 0.
 * @param rows rows of the Matrix.
 * @param cols cols of the Matrix.
 */
Matrix::Matrix (int rows, int cols)
{
  if ((rows < 1) || (cols < 1))
    {
      std::cerr << INVALID_PARM_FOR_MATRIX << std::endl;
      exit (EXIT_FAILURE);
    }
  _rows = rows;
  _cols = cols;
  _matrix = new (std::nothrow)float[_rows * _cols];
  if (_matrix == nullptr)
    {
      std::cerr << ALLOCATION_FAILURE << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < _rows * _cols; i++)
    {
      _matrix[i] = 0;
    }
}
/**
 * Copy construct, constructs matrix from another Matrix mat.
 * @param mat matrix to copy from.
 */
Matrix::Matrix (const Matrix &mat)
{
  _rows = mat._rows;
  _cols = mat._cols;
  _matrix = new (std::nothrow)float[_rows * _cols];
  if (_matrix == nullptr)
    {
      std::cerr << ALLOCATION_FAILURE << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < _rows * _cols; i++)
    {
      _matrix[i] = mat._matrix[i];
    }
}
/**
 * Transforms a matrix into its transpose matrix.
 * @return the transposed matrix.
 */
Matrix &Matrix::transpose ()
{
  float *new_mat = new (std::nothrow)float[_rows * _cols];
  if (new_mat == nullptr)
    {
      std::cerr << ALLOCATION_FAILURE << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < _cols; i++)
    {
      for (int j = 0; j < _rows; ++j)
        {
          new_mat[i * _rows + j] = _matrix[j * _cols + i];
        }
    }
  delete[] _matrix;
  _matrix = new_mat;
  int tmp = _cols;
  _cols = _rows;
  _rows = tmp;
  return *this;
}
/**
 * Transforms a matrix into a column vector.
 */
Matrix &Matrix::vectorize ()
{
  _rows *= _cols;
  _cols = 1;
  return *this;
}
/**
 * Prints matrix elements, no return value.
 */
void Matrix::plain_print () const
{
  for (int i = 0; i < _rows; i++)
    {
      for (int j = 0; j < _cols; ++j)
        {
          std::cout << _matrix[i * _cols + j] << " ";
        }
      std::cout << std::endl;
    }
}
/**
 * Returns a matrix which is the dot product of
 * this matrix and another matrix m.
 * @param mat to dot with.
 * @return the dot matrix.
 */
Matrix Matrix::dot (const Matrix &mat) const
{
  if ((mat._rows != _rows) || (mat._cols != _cols))
    {
      std::cerr << INVALID_MATRIX << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix my_mat (_rows, _cols);
  for (int i = 0; i < _rows * _cols; ++i)
    {
      my_mat._matrix[i] = mat._matrix[i] * _matrix[i];
    }
  return my_mat;
}
/**
 * @return Returns the Frobenius norm of the given matrix.
 */
float Matrix::norm () const
{
  float mat_norm = 0;
  for (int i = 0; i < _rows * _cols; ++i)
    {
      mat_norm += powf (_matrix[i], 2);
    }
  return sqrtf (mat_norm);
}
/**
 * Reads into a given matrix from std::istream.
 * @param is std::istream arg to read from.
 * @param mat matrix to write in.
 * @return std::istream.
 */
std::istream &read_binary_file (std::istream &is, Matrix &mat)
{
  int size = mat.get_rows () * mat.get_cols ();
  is.read ((char *) mat._matrix, size * FLOAT_SIZE);
  if (!is.good ())
    {
      std::cerr << FAIL_TO_READ << std::endl;
      exit (EXIT_FAILURE);
    }
  return is;
}
/**
 * + operator. Gets 2 matrix and adds the i element, for all elements.
 * @return the result matrix by value.
 */
Matrix operator+ (const Matrix &mat1, const Matrix &mat2)
{
  if ((mat1._rows != mat2._rows) || (mat1._cols != mat2._cols))
    {
      std::cerr << INVALID_MATRIX << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix mat (mat1._rows, mat1._cols);
  for (int i = 0; i < mat._rows * mat._cols; i++)
    {
      mat._matrix[i] = mat1._matrix[i] + mat2._matrix[i];
    }
  return mat;
}
/**
 * Assignment operator.
 * @return matrix by reference.
 */
Matrix &Matrix::operator= (const Matrix &mat)
{
  if (&mat != this)
    {
      delete[] _matrix;
      _rows = mat._rows;
      _cols = mat._cols;
      _matrix = new (std::nothrow)float[_rows * _cols];
      if (_matrix == nullptr)
        {
          std::cerr << ALLOCATION_FAILURE << std::endl;
          exit (EXIT_FAILURE);
        }
      for (int i = 0; i < _rows * _cols; i++)
        {
          _matrix[i] = mat._matrix[i];
        }
    }
  return *this;
}
/**
 * Multiplication operator.
 * @return matrix by value.
 */
Matrix operator* (const Matrix &mat1, const Matrix &mat2)
{
  if (mat1._cols != mat2._rows)
    {
      std::cerr << INVALID_MATRIX << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix mat (mat1._rows, mat2._cols);
  for (int i = 0; i < mat1._rows; i++)
    {
      for (int j = 0; j < mat2._cols; j++)
        {
          for (int k = 0; k < mat1._cols; k++)
            {
              mat._matrix[i * mat2._cols + j] +=
                  mat1._matrix[i * mat1._cols + k]
                  * mat2._matrix[k * mat2._cols + j];
            }
        }
    }
  return mat;
}
/**
 * Scalar multiplication on the right.
 * @param c multiplication mat by c.
 * @return matrix by value.
 */
Matrix operator* (const Matrix &mat, float c)
{
  Matrix output_mat (mat._rows, mat._cols);
  for (int i = 0; i < mat._rows * mat._cols; i++)
    {
      output_mat._matrix[i] = mat._matrix[i] * c;
    }
  return output_mat;
}
/**
 * Scalar multiplication on the left.
 * @param c multiplication mat by c.
 * @return matrix by value.
 */
Matrix operator* (float c, const Matrix &mat)
{
  Matrix output_mat (mat._rows, mat._cols);
  for (int i = 0; i < mat._rows * mat._cols; i++)
    {
      output_mat._matrix[i] = c * mat._matrix[i];
    }
  return output_mat;
}
/**
 * Addition accumulation operator.
 * @return matrix by reference.
 */
Matrix &Matrix::operator+= (const Matrix &mat)
{
  if ((mat._rows != _rows) || (mat._cols != _cols))
    {
      std::cerr << INVALID_MATRIX << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < _rows * _cols; i++)
    {
      _matrix[i] += mat._matrix[i];
    }
  return *this;
}
/**
 * Parenthesis indexing operator.
 * @return by reference object in row i col j.
 */
float &Matrix::operator() (int i, int j)
{
  if ((i < 0) || (i >= _rows) || (j < 0) || (j >= _cols))
    {
      std::cerr << INDEX_OUT_OF_RANGE << std::endl;
      exit (EXIT_FAILURE);
    }
  return _matrix[i * _cols + j];
}
/**
 * Parenthesis indexing operator.
 * @return by value object in row i col j.
 */
float Matrix::operator() (int i, int j) const
{
  if ((i < 0) || (i >= _rows) || (j < 0) || (j >= _cols))
    {
      std::cerr << INDEX_OUT_OF_RANGE << std::endl;
      exit (EXIT_FAILURE);
    }
  return _matrix[i * _cols + j];
}
/**
 * Brackets indexing.
 * @return by reference object i in matrix.
 */
float &Matrix::operator[] (int i)
{
  if ((i < 0) || (i >= _rows * _cols))
    {
      std::cerr << INDEX_OUT_OF_RANGE << std::endl;
      exit (EXIT_FAILURE);
    }
  return _matrix[i];
}
/**
 * Brackets indexing.
 * @return by value object i in matrix.
 */
float Matrix::operator[] (int i) const
{
  if ((i < 0) || (i >= _rows * _cols))
    {
      std::cerr << INDEX_OUT_OF_RANGE << std::endl;
      exit (EXIT_FAILURE);
    }
  return _matrix[i];
}
/**
 * Prints struct of the matrix.
 * @return Output stream.
 */
std::ostream &operator<< (std::ostream &os, const Matrix &mat)
{
  for (int i = 0; i < mat._rows; i++)
    {
      for (int j = 0; j < mat._cols; j++)
        {
          if (mat._matrix[i * mat._cols + j] >= MIN_TO_PRINT)
            {
              os << "  ";
            }
          else
            {
              os << "**";
            }
        }
      os << std::endl;
    }
  return os;
}