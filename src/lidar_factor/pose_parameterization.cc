#include "pose_parameterization.hh"

/**
 * @brief 定义pose的加法
 * @param x
 * @param delta
 * @param x_plus_delta
 */
bool PoseParameterization::Plus(const double *x, const double *delta, double *x_plus_delta) const {
    Eigen::Map<const Eigen::Vector3d> _p(x);
    Eigen::Map<const Eigen::Vector3d> _q(x + 3);
    Eigen::Map<const Eigen::Vector3d> dp(delta);
    Eigen::Quaterniond dq = ctlio::slam::numType::deltaQ(Eigen::Map<const Eigen::Vector3d>(delta + 3));

    Eigen::Map<Eigen::Vector3d> p(x_plus_delta);
    Eigen::Map<Eigen::Vector3d> q(x_plus_delta + 3);
    p = _p + dp;
    q = (_q * dp).normalized();
    return true;
}
bool PoseParameterization::ComputeJacobian(const double *x, double *jacobian) const {
    // x的p,q 求雅克比矩阵为7*6
    Eigen::Map<Eigen::Matrix<double, 7, 6, Eigen::RowMajor>> j(jacobian);
    j.topRows<6>().setIdentity();
    j.bottomRows<1>().setZero();

    return true;
}

bool RotationParameterization::Plus(const double *x, const double *delta, double *x_plus_delta) const {
    Eigen::Map<const Eigen::Quaterniond> _q(x);
    Eigen::Quaterniond dq = ctlio::slam::numType::deltaQ(Eigen::Map<const Eigen::Vector3d>(delta + 3));
    Eigen::Map<Eigen::Quaterniond> q(x_plus_delta);
    q = (_q * dq).normalized();
    return true;
}
bool RotationParameterization::ComputeJacobian(const double *x, double *jacobian) const {
    Eigen::Map<Eigen::Matrix<double, 4, 3, Eigen::RowMajor>> j(jacobian);
    j.topRows<3>().setIdentity();
    j.bottomRows<1>().setZero();
    return true;
}