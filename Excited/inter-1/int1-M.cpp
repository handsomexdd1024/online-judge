#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cassert>
#include <initializer_list>

typedef int Numtype;

// Numtype modulus;

#define MODULUS 2

class Matrix{
    std::vector<std::vector<Numtype>> mat;
public:
    std::vector<Numtype> &operator[] (const size_t &_index) {
        return this->mat[_index];
    }
    const std::vector<Numtype> &operator[] (const size_t &_index) const {
        return this->mat[_index];
    }
    /**
     * @brief Construct a new Matrix object by size and default element
     * 
     * @param row size of row
     * @param col size of column
     * @param elem default element, defaults to 0
     */
    Matrix(const size_t &row, const size_t &col, const Numtype &elem = 0) {
        this->mat.resize(row);
        for(auto &_row: mat) 
            _row.resize(col, elem);
    }

    /**
     * @brief Construct a new vector matrix
     * 
     * @param isColVec marks if this vector is a column vector
     * @param lst list of elements
     */
    Matrix(const bool &isColVec, const std::initializer_list<Numtype> &lst) {
        if(isColVec) {
            mat.resize(lst.size());
            auto r = mat.begin();
            for(const auto &rl: lst) {
                r->resize(1);
                r->front() = rl;
                r++;
            }
        } else {
            mat.resize(1);
            mat[0] = lst;
        }
    }

    /**
     * @brief Construct a new Matrix object by a 2-dimensional initializer_list
     * 
     * @param lst a 2-dimensional initializer_list
     */
    Matrix(const std::initializer_list<std::initializer_list<Numtype>> &lst) {
        this->mat.resize(lst.size());
        auto r = mat.begin();
        for(const auto &rl: lst) {
            assert(rl.size() == lst.begin()->size());
            *r = rl;
            r++;
        }
    }

    size_t row() const {
        return mat.size();
    }
    size_t col() const {
        return mat[0].size();
    }
    Numtype trace() const {
        assert(this->row() == this->col());
        Numtype ans = 0;
        for(size_t k = 0, r = this->row(); k < r ; k++) {
            #ifdef MODULUS
            ans += this->mat[k][k] % MODULUS;
            ans %= MODULUS;
            #else
            ans += this->mat[k][k];
            #endif
        }
        return std::move(ans); 
    }
    /**
     * @brief Returns an identity matrix of size s
     * 
     * @param s size of the identity matrix
     * @return Matrix identity matrix
     */
    static Matrix identity(const size_t &s) {
        assert(s >= 0);
        Matrix ans(s, s);
        for(size_t i = 0; i < s; i++)
            ans[i][i] = 1;
        return ans;
    }
};

Matrix operator+ (const Matrix &lhs, const Matrix &rhs) {
    assert(lhs.col() == rhs.col() && lhs.row() == rhs.row());
    Matrix ans(lhs.row(), lhs.col());
    for(size_t r = 0; r < ans.row(); r++) {
        for(size_t c = 0; c < ans.col(); c++) {
            #ifdef MODULUS
            ans[r][c] = lhs[r][c] + rhs[r][c] % MODULUS;
            #else
            ans[r][c] = lhs[r][c] + rhs[r][c];
            #endif
        }
    }
    return ans;
}

Matrix operator* (const Matrix &lhs, const Matrix &rhs) {
    assert(lhs.col() == rhs.row());
    Matrix ans(lhs.row(), rhs.col());
    for(size_t r = 0; r < ans.row(); r++) {
        for(size_t c = 0; c < ans.col(); c++) {
            for(size_t k = 0; k < lhs.col(); k++) {
                #ifdef MODULUS
                ans[r][c] += lhs[r][k] * rhs[k][c] % MODULUS;
                ans[r][c] %= MODULUS;
                #else
                ans[r][c] += lhs[r][k] * rhs[k][c];
                #endif
            }
        }
    }
    return ans;
}

Matrix pow(const Matrix &mat, Numtype p) {
    assert(p >= 0);
    assert(mat.row() == mat.col());

    Matrix ans(Matrix::identity(mat.row())), tmp(mat);
    while(p > 0) {
        if(p % 2) ans = ans * tmp;
        p >>= 1;
        tmp = tmp * tmp;
    }
    return ans;
}

int main() {
    Numtype t;
    std::string tmp;
    while(std::cin >> t >> tmp) {
        Matrix init(tmp.size(), 1), iter(tmp.size(), tmp.size());
        for(size_t i = 0; i < tmp.size(); i++) {
            init[i][0] = tmp[i] - '0';
        }
        iter[0][0] = iter[0][iter.col() - 1] = 1;
        for(size_t i = 1; i < iter.row(); i++) {
            iter[i][i-1] = iter[i][i] = 1;
        }
        Matrix ans = pow(iter, t) * init;
        for(size_t i = 0; i < tmp.size(); i++) {
            std::cout << ans[i][0];
        }
        std::cout << '\n';
    }
    return 0;
}