// Copyright 2017 Dr. Xavier JUVIGNY

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// Test Parallel library on Parallel product matrix--matrix
// with row and column communicators
#include "core/log_to_std_output.hpp"
#include "core/logger.hpp"
#include "core/multitimer.hpp"
#include "core/std_cpp_chronometer.hpp"
#include "parallel/communicator"
#include "parallel/context.hpp"
#include "parallel/log_from_distributed_file.hpp"
#include "parallel/log_from_root_output.hpp"
#include <cassert>
#include <cmath>
#include <string>
#include <tuple>
#include <vector>

template <typename K>
class BlockMatrix : public std::vector<K> {
  public:
    BlockMatrix() = default;
    BlockMatrix(std::size_t nrows, std::size_t ncols) : std::vector<K>(nrows * ncols), m_nrows(nrows), m_ncols(ncols) {}
    BlockMatrix(std::size_t nrows, std::size_t ncols, const K &val)
      : std::vector<K>(nrows * ncols, val), m_nrows(nrows), m_ncols(ncols) {}
    BlockMatrix(const BlockMatrix &A) = default;
    BlockMatrix(BlockMatrix &&A)      = default;
    ~BlockMatrix()                    = default;

    BlockMatrix &operator=(const BlockMatrix &A) = default;
    BlockMatrix &operator=(BlockMatrix &&A) = default;

    std::size_t getNRows() const { return m_nrows; }
    std::size_t getNCols() const { return m_ncols; }

    const K &operator()(std::size_t i, std::size_t j)  const && {
        assert(i < m_nrows);
        assert(j < m_ncols);
        return (*this)[i + j * m_nrows];
    }
    K &operator()(std::size_t i, std::size_t j) & {
        assert(i < m_nrows);
        assert(j < m_ncols);
        return (*this)[i + j * m_nrows];
    }
    const K &operator()(std::size_t i, std::size_t j) const & {
        assert(i < m_nrows);
        assert(j < m_ncols);
        return (*this)[i + j * m_nrows];
    }

  private:
    std::size_t m_nrows, m_ncols;
};

/**
 * brief       Compute tensorial vectors
 * 
 * Compute two pairs of vectors. Each pair of vectors define a tensor product to
 * compute respectivly the coefficients of the A and B matrices.
 */
template <typename K>
auto computeTensorVectors(std::size_t dim, std::size_t dim_block, std::size_t begRows, std::size_t begCols) {
    const K pi = std::acos(K(-1));
    std::vector<K> u1_r(dim_block), v1_c(dim_block), u2_r(dim_block), v2_c(dim_block);
    const K lx = 1., ly = 2.;
    for (size_t i = 0; i < dim_block; ++i) {
        u1_r[i] = std::cos(2 * (i + begRows) * pi / dim);
        v1_c[i] = std::sin(2 * (i + begCols) * pi / dim);
        u2_r[i] = (i + begRows) * lx / dim;
        v2_c[i] = (i + begCols) * ly / dim;
    }
    return std::make_tuple(u1_r, v1_c, u2_r, v2_c);
}
// -----------------------------------------------------------------------------
/**
 * brief       Compute a matrix from a pair of vector
 *
 * Compute a matrix block with the tensor product of a pair of vectors.
 */
template <typename K>
auto computeMatrice(const std::vector<K> &u_row, const std::vector<K> &v_col) {
    BlockMatrix<K> A(u_row.size(), v_col.size());
    for (std::size_t irow = 0; irow < u_row.size(); ++irow) {
        for (std::size_t jcol = 0; jcol < v_col.size(); ++jcol) {
            A(irow, jcol) = u_row[irow] * v_col[jcol];
        }
    }
    return A;
}
// -----------------------------------------------------------------------------
/**
 * brief       Compute product of block matrix with block matrix
 *
 * Product of two block matrices A  and B  which is added to a third block matrix C
 */
template <typename K>
void prodMatrixMatrixBloc(const BlockMatrix<K> &A, const BlockMatrix<K> &B, BlockMatrix<K> &C) {
    assert(A.getNCols() == B.getNRows());
    for (std::size_t k = 0; k < A.getNCols(); ++k) {
        for (std::size_t j = 0; j < B.getNCols(); ++j) {
            for (std::size_t i = 0; i < A.getNRows(); ++i) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}
// -----------------------------------------------------------------------------
/**
 * brief       Dot product of two vectors
 *
 * Dot product of two local vectors ( not a global dot product ! )
 */
template <typename K>
K dotProduct(const std::vector<K> &u, const std::vector<K> &v) {
    assert(u.size() == v.size());
    K sum = 0;
    for (std::size_t i = 0; i < u.size(); ++i) sum += u[i] * v[i];
    return sum;
}
// -----------------------------------------------------------------------------
/**
 * brief       Verification of the result of the matrix-matrix product
 *
 * Verification of the product matrix matrix result. The result matrix must
 * verify the following formulae :
 *  A = uA x vA^{T} and B = uB x vB^{T}
 *  so C = A.B = (vA|uB) uA x vB^{T}
 *  a.k.a C_ij = (vA|uB) uA_{i}vB_{j}
 */
template <typename K>
bool verifyProdMatMat(std::size_t dimBlock, const K &alpha, const std::vector<K> &uA, const std::vector<K> &vB,
                      const BlockMatrix<K> &C) {
    bool isOK = true;
    for (std::size_t i = 0; i < dimBlock; ++i) {
        for (std::size_t j = 0; j < dimBlock; ++j) {
            K val = alpha * uA[i] * vB[j];
            if (std::abs(val - C(i, j)) > 1.E-6 * std::abs(C(i, j))) {
                std::cerr << "Detecting a problem in result coefficients : "
                          << "Value computed : " << C(i, j) << " and value excepted : " << val << std::endl;
                isOK = false;
                break;
            }
        }
    }
    return isOK;
}
// _____________________________________________________________________________
// =============================================================================
int main(int nargs, char *argv[]) {
    Parallel::Context context(nargs, argv);
    Parallel::Communicator globCom;
    Core::Logger log;
    int root_listeners = Core::Logger::information;
    int file_listeners =
        Core::Logger::assertion + Core::Logger::error + Core::Logger::warning + Core::Logger::information;
    bool profiled = false;
    for (int iargs = 2; iargs < nargs; ++iargs) {
        if (std::string(argv[iargs]) == std::string("trace"))
            log.subscribe(new Parallel::LogFromDistributedFile(Core::Logger::trace, globCom, "Trace"));

        if (std::string(argv[iargs]) == std::string("profile")) {
            profiled = true;
        }
    }
    log.subscribe(new Parallel::LogFromDistributedFile(file_listeners, globCom, "Output"));
    log.subscribe(new Parallel::LogFromRootOutput<Core::LogToStdOutput>(root_listeners));

    Core::MultiTimer<std::string> timer;

    std::size_t dim    = 120;
    if (nargs > 1) dim = std::stoul(std::string(argv[1]));
    // Prepare the parallel computation :
    int p                 = int(std::sqrt(globCom.size));
    std::size_t dim_block = dim / p;
    int IBlock            = globCom.rank % p;
    int JBlock            = globCom.rank / p;
    std::size_t begRow    = IBlock * dim_block;
    std::size_t begCol    = JBlock * dim_block;
    log << LogInformation << "Number of blocks per direction " << p << std::endl
        << "Dimension of each block : " << dim_block << std::endl
        << "Indice of C block : " << IBlock << " : " << JBlock << std::endl
        << "Beginning of the row and column indices : " << begRow << ", " << begCol << std::endl;
    Parallel::Communicator rowCom(globCom, IBlock, JBlock);
    Parallel::Communicator colCom(globCom, JBlock, IBlock);
    assert(rowCom.size == p);
    assert(rowCom.rank == JBlock);
    assert(colCom.size == p);
    assert(colCom.rank == IBlock);
    if (profiled) {
        timer.subscribe<Parallel::Communicator::Chronometer>("Row Communicator", rowCom);
        timer.subscribe<Parallel::Communicator::Chronometer>("Column Communicator", colCom);
    }
    timer.subscribe<Core::StdChronometer>("Compute tensor vectors");
    timer.subscribe<Core::StdChronometer>("Compute matrices");
    timer.subscribe<Core::StdChronometer>("Product Matrix-matrix");
    timer.subscribe<Core::StdChronometer>("Verify Matrix-matrix");
    
    timer["Compute tensor vectors"].start();
    std::vector<double> uA, vA, uB, vB;
    std::tie(uA, vA, uB, vB) = computeTensorVectors<double>(dim, dim_block, begRow, begCol);
    timer["Compute tensor vectors"].stop();

    timer["Compute matrices"].start();
    auto A = computeMatrice(uA, vA);
    auto B = computeMatrice(uB, vB);
    timer["Compute matrices"].stop();
    BlockMatrix<double> C(A.getNRows(), B.getNCols());
    // Parallel product :

    timer["Product Matrix-matrix"].start();
    BlockMatrix<double> Atmp(dim_block, dim_block), Btmp(dim_block, dim_block);

    for (int kBlock = 0; kBlock < p; ++kBlock) {
        rowCom.bcast(A, Atmp, kBlock);
        colCom.bcast(B, Btmp, kBlock);
        prodMatrixMatrixBloc(Atmp, Btmp, C);
    }
    timer["Product Matrix-matrix"].stop();

    timer["Verify Matrix-matrix"].start();
    std::tie(std::ignore, vA, uB, std::ignore) = computeTensorVectors<double>(dim, dim, 0, 0);
    double vAdotuB = dotProduct(vA, uB);
    if (verifyProdMatMat(dim_block, vAdotuB, uA, vB, C)) {
        log << LogInformation << Core::Logger::BGreen << "Test passed." << Core::Logger::Normal << std::endl;
    } else {
        log << LogError << "Test failed !\n";
    }
    timer["Verify Matrix-matrix"].stop();    
    log << LogInformation << timer << std::endl;
    return EXIT_SUCCESS;
}
