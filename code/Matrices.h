#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

namespace Matrices
{
    class Matrix
    {
        public:
            ///Construct a matrix of the specified size.
            ///Initialize each element to 0.
            Matrix(int _rows, int _cols)
            {
                rows = _rows;
                cols = _cols;
                a.resize(rows, vector<double>(cols, 0.0));
            }

            ///************************************
            ///inline accessors / mutators, these are done:

            ///Read element at row i, column j
            ///usage:  double x = a(i,j);
            const double& operator()(int i, int j) const
            {
                return a.at(i).at(j);
            }

            ///Assign element at row i, column j
            ///usage:  a(i,j) = x;
            double& operator()(int i, int j)
            {
                return a.at(i).at(j);
            }

            int getRows() const{return rows;}
            int getCols() const{return cols;}
            ///************************************
        protected:
            ///changed to protected so sublasses can modify
            vector<vector<double>> a;
        private:
            int rows;
            int cols;
    };

    /// Add each corresponding element.
    /// usage: c = a + b;
    Matrix operator+(const Matrix& a, const Matrix& b){
        if (a.getRows() == b.getRows() && a.getCols() == b.getCols()){
            Matrix c(a.getRows(), a.getCols());
            for (int i = 0; i < a.getRows(); i++){
                for (int j = 0; j < a.getCols(); j++){
                    c(i,j) = a(i,j) + b(i,j);
                }
            }
            return c;
        } else {
            throw runtime_error("Error: dimensions must agree");
        }
    }
    /// Matrix multiply. See description.
    /// usage: c = a * b;
    Matrix operator*(const Matrix& a, const Matrix& b){
        if (a.getCols() == b.getRows()){
            Matrix c(a.getRows(), b.getCols());
            for (int i = 0; i < a.getRows(); i++){
                for (int j = 0; j < b.getCols(); j++){
                    for (int n = 0; n < a.getCols(); n++)
                    {
                        c(i,j) += a(i,n) * b(n,j);
                    }
                }
            }
            return c;
        } else {
            throw runtime_error("Error: dimensions must agree");
        }
    }
    /// Matrix comparison. See description.
    /// usage: a == b
    bool operator==(const Matrix& a, const Matrix& b){
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) { return false; }
        for (int i = 0; i < a.getRows(); i++){
            for (int j = 0; j < a.getCols(); j++){
                if(abs(a(i,j) - b(i,j)) > 0.001) { return false; }
            }
        }
        return true;
    }
    /// Matrix comparison. See description.
    /// usage: a != b
    bool operator!=(const Matrix& a, const Matrix& b){
        return !(a == b);
    }
    /// Output matrix.
    /// Separate columns by ' ' and rows by '\n'
    ostream& operator<<(ostream& os, const Matrix& a){
        for(int i = 0; i < a.getRows(); i++){
            for(int j = 0; j < a.getCols(); j++){
                os << a(i,j) << " ";
            }
            os << '\n';
        }
        return os;
    }

    /*******************************************************************************/

    ///2D rotation matrix
    ///usage:  A = R * A rotates A theta radians counter-clockwise
    class RotationMatrix : public Matrix
    {
        public:
            ///Call the parent constructor to create a 2x2 matrix
            ///Then assign each element as follows:
            /*
            cos(theta)  -sin(theta)
            sin(theta)   cos(theta)
            */
            ///theta represents the angle of rotation in radians, counter-clockwise
            RotationMatrix(double theta) : Matrix(2,2)
            {
                (*this)(0,0) = cos(theta);
                (*this)(0,1) = -sin(theta);
                (*this)(0,0) = sin(theta);
                (*this)(0,1) = cos(theta);
            }
    };

    ///2D scaling matrix
    ///usage:  A = S * A expands or contracts A by the specified scaling factor
    class ScalingMatrix : public Matrix
    {
        public:
            ///Call the parent constructor to create a 2x2 matrix
            ///Then assign each element as follows:
            /*
            scale   0
            0       scale
            */
            ///scale represents the size multiplier
            ScalingMatrix(double scale) : Matrix(2,2)
            {
                (*this)(0,0) = scale;
                (*this)(0,1) = 0;
                (*this)(0,0) = 0;
                (*this)(0,1) = scale;
            }
    };

    ///2D Translation matrix
    ///usage:  A = T + A will shift all coordinates of A by (xShift, yShift)
    class TranslationMatrix : public Matrix
    {
        public:
            ///Call the parent constructor to create a 2xn matrix
            ///Then assign each element as follows:
            /*
            xShift  xShift  xShift  ...
            yShift  yShift  yShift  ...
            */
            ///paramaters are xShift, yShift, and nCols
            ///nCols represents the number of columns in the matrix
            ///where each column contains one (x,y) coordinate pair
            TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2,nCols)
            {
                for (int i = 0; i < nCols; i++)
                {
                    (*this)(0,i) = xShift;
                    (*this)(1,i) = yShift;
                }
            }
    };
}

#endif // MATRIX_H_INCLUDED
