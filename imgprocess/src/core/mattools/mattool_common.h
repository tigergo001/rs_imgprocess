//
// Created by penglei on 18-9-12.
//
// 封装“Eigen”矩阵库，用到什么功能就封装什么功能

#ifndef IMGPROCESS_MATTOOL_COMMON_H
#define IMGPROCESS_MATTOOL_COMMON_H

#include "Eigen/Dense"
using namespace Eigen;

namespace MatTool {

    // 数据存储在矩阵内部
    using Matrixub = Eigen::Matrix<unsigned char, Dynamic, Dynamic, RowMajor>;
    using Matrixus = Eigen::Matrix<unsigned short, Dynamic, Dynamic, RowMajor>;
    using Matrixs = Eigen::Matrix<short, Dynamic, Dynamic, RowMajor>;
    using Matrixui = Eigen::Matrix<unsigned int, Dynamic, Dynamic, RowMajor>;
    using Matrixi = Eigen::Matrix<int, Dynamic, Dynamic, RowMajor>;
    using Matrixf = Eigen::Matrix<float, Dynamic, Dynamic, RowMajor>;
    using Matrixd = Eigen::Matrix<double, Dynamic, Dynamic, RowMajor>;

    using Vectord = Eigen::Matrix<double, Dynamic, 1>;  // 列向量
    using RVectord = Eigen::Matrix<double, 1, Dynamic>;  // 行向量

    // 数据存储在外部（比如存储在外部数组中）
    using ExtMatrixub = Eigen::Map<Matrixub>;
    using ExtMatrixus = Eigen::Map<Matrixus>;
    using ExtMatrixs = Eigen::Map<Matrixs>;
    using ExtMatrixui = Eigen::Map<Matrixui>;
    using ExtMatrixi = Eigen::Map<Matrixi>;
    using ExtMatrixf = Eigen::Map<Matrixf>;
    using ExtMatrixd = Eigen::Map<Matrixd>;

    using ExtVectord = Eigen::Map<Vectord>;
    using ExtRVectord = Eigen::Map<RVectord>;

    /**
     * 判断矩阵是否可逆
     * @tparam _MatrixType  矩阵类型
     * @param mat           待判断的矩阵（注意不能为“Ext***”类型，否则会修改原矩阵数据）
     * @return 矩阵可逆就返回 true，不可逆就返回false
     */
    template <typename _MatrixType>
    inline bool isInvertible(const _MatrixType &mat) {
        return Eigen::FullPivLU<_MatrixType>(mat).isInvertible();
    }

} // namespace MatTool

#endif //IMGPROCESS_MATTOOL_COMMON_H
