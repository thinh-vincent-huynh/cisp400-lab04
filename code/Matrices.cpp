#include "Matrices.h"
#include <stdexcept>

namespace Matrices
{
    Matrix::Matrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;
        a.resize(rows, vector<double>(cols, 0.0));
    }

    const double& Matrix::operator()(int i, int j) const
    {
        return a.at(i).at(j);
    }

    double& Matrix::operator()(int i, int j)
    {
        return a.at(i).at(j);
    }

    int Matrix::getRows() const { return rows; }
    int Matrix::getCols() const { return cols; }

    Matrix operator+(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() == b.getRows() && a.getCols() == b.getCols())
        {
            Matrix c(a.getRows(), a.getCols());
            for (int i = 0; i < a.getRows(); i++)
            {
                for (int j = 0; j < a.getCols(); j++)
                {
                    c(i, j) = a(i, j) + b(i, j);
                }
            }
            return c;
        }
        else
        {
            throw runtime_error("Error: dimensions must agree");
        }
    }

    Matrix operator*(const Matrix& a, const Matrix& b)
    {
        if (a.getCols() == b.getRows())
        {
            Matrix c(a.getRows(), b.getCols());
            for (int i = 0; i < a.getRows(); i++)
            {
                for (int j = 0; j < b.getCols(); j++)
                {
                    for (int n = 0; n < a.getCols(); n++)
                    {
                        c(i, j) += a(i, n) * b(n, j);
                    }
                }
            }
            return c;
        }
        else
        {
            throw runtime_error("Error: dimensions must agree");
        }
    }

    bool operator==(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) { return false; }
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                if (abs(a(i, j) - b(i, j)) > 0.001) { return false; }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& a, const Matrix& b)
    {
        return !(a == b);
    }

    ostream& operator<<(ostream& os, const Matrix& a)
    {
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                os << a(i, j) << " ";
            }
            os << '\n';
        }
        return os;
    }

    RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
    {
        (*this)(0, 0) = cos(theta);
        (*this)(0, 1) = -sin(theta);
        (*this)(1, 0) = sin(theta);
        (*this)(1, 1) = cos(theta);
    }

    ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
    {
        (*this)(0, 0) = scale;
        (*this)(0, 1) = 0;
        (*this)(1, 0) = 0;
        (*this)(1, 1) = scale;
    }

    TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
    {
        for (int i = 0; i < nCols; i++)
        {
            (*this)(0, i) = xShift;
            (*this)(1, i) = yShift;
        }
    }
}
