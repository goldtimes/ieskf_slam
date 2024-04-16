
#ifndef SLAM_IN_AUTO_DRIVING_POINT_TYPES_H
#define SLAM_IN_AUTO_DRIVING_POINT_TYPES_H
// 多种点云类型的定义
#include <livox_ros_driver/CustomMsg.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/impl/pcl_base.hpp>
#include <vector>
#include "common/eigen_types.hh"

namespace ctlio::slam {
// 定义点和点云类型
using PointType = pcl::PointXYZI;
using PointCloudType = pcl::PointCloud<PointType>;
using CloudPtr = PointCloudType::Ptr;
using PointsVec = std::vector<PointType, Eigen::aligned_allocator<PointType>>;
using IndexVec = std::vector<int>;

/**
 * @brief PointType -> Eigen::Vector
 */
inline Vec3f ToVec3f(const PointType& pt) {
    return pt.getVector3fMap();
}
inline Vec3d ToVec3d(const PointType& pt) {
    return pt.getArray3fMap().cast<double>();
}

template <typename T, int dim>
inline Eigen::Matrix<T, dim, 1> ToEigen(const PointType& pt);

template <>
inline Eigen::Matrix<float, 2, 1> ToEigen(const PointType& pt) {
    return Vec2f(pt.x, pt.y);
}

template <>
inline Eigen::Matrix<float, 3, 1> ToEigen(const PointType& pt) {
    return Vec3f(pt.x, pt.y, pt.z);
}

template <typename S>
inline PointType ToPointType(const Eigen::Matrix<S, 3, 1>& pt) {
    PointType p;
    p.x = pt.x();
    p.y = pt.y();
    p.z = pt.z();
    return p;
}

struct FullPointType {
    PCL_ADD_POINT4D;
    float range = 0;
    float radius = 0;
    uint8_t intensity = 0;
    uint8_t ring = 0;
    uint8_t angle = 0;
    double time = 0;
    float height = 0;
    inline FullPointType() {
    }
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

using FullPointCloudType = pcl::PointCloud<FullPointType>;
using FullCloudPtr = FullPointCloudType::Ptr;

inline Vec3f ToVec3f(const FullPointType& pt) {
    return pt.getArray3fMap();
}

inline Vec3d ToVec3d(const FullPointType& pt) {
    return pt.getArray3fMap().cast<double>();
}

struct point3D {
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Eigen::Vector3d raw_point;  //  raw point
    Eigen::Vector3d point;      //  global frame

    double intensity;            //   intensity
    double alpha_time = 0.0;     //  reference to last point of current frame [0,1]
    double relative_time = 0.0;  //  feference to current frame
    double timespan;             //   total time of current frame
    double timestamp = 0.0;      //   global timestamp
    int ring;                    //   ring

    point3D() = default;
};

}  // namespace ctlio::slam

POINT_CLOUD_REGISTER_POINT_STRUCT(ctlio::slam::FullPointType,
                                  (float, x, x)(float, y, y)(float, z, z)(float, range, range)(float, radius, radius)(
                                      std::uint8_t, intensity, intensity)(std::uint16_t, angle, angle)(
                                      std::uint8_t, ring, ring)(double, time, time)(float, height, height))
namespace liovx_ros {
struct EIGEN_ALIGN16 Point {
    PCL_ADD_POINT4D
    PCL_ADD_INTENSITY;
    float time;
    uint16_t ring;
    uint16_t tag;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
}  // namespace liovx_ros
POINT_CLOUD_REGISTER_POINT_STRUCT(liovx_ros::Point,
                                  (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)(
                                      float, time, time)(uint16_t, ring, ring)(uint16_t, tag, tag))

namespace velodyne_ros {
struct EIGEN_ALIGN16 Point {
    PCL_ADD_POINT4D;
    float intensity;
    float time;
    std::uint16_t ring;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
}  // namespace velodyne_ros

// clang-format off
POINT_CLOUD_REGISTER_POINT_STRUCT(velodyne_ros::Point,
                                  (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)
                                      (float, time, time)(std::uint16_t, ring, ring))
// clang-format on

namespace robosense_ros {
struct EIGEN_ALIGN16 Point {
    PCL_ADD_POINT4D;
    uint8_t intensity;
    uint16_t ring = 0;
    double timestamp = 0;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
}  // namespace robosense_ros

POINT_CLOUD_REGISTER_POINT_STRUCT(robosense_ros::Point,
                                  (float, x, x)(float, y, y)(float, z, z)(uint8_t, intensity, intensity)(
                                      uint16_t, ring, ring)(double, timestamp, timestamp))

namespace ouster_ros {
struct EIGEN_ALIGN16 Point {
    PCL_ADD_POINT4D;
    float intensity;
    uint32_t t;
    uint16_t reflectivity;
    uint8_t ring;
    uint16_t ambient;
    uint32_t range;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
}  // namespace ouster_ros

// clang-format off
POINT_CLOUD_REGISTER_POINT_STRUCT(ouster_ros::Point,
                                  (float, x, x)
                                      (float, y, y)
                                      (float, z, z)
                                      (float, intensity, intensity)
                                      // use std::uint32_t to avoid conflicting with pcl::uint32_t
                                      (std::uint32_t, t, t)
                                      (std::uint16_t, reflectivity, reflectivity)
                                      (std::uint8_t, ring, ring)
                                      (std::uint16_t, ambient, ambient)
                                      (std::uint32_t, range, range)
)
// clang-format on

namespace pandar_ros {
struct EIGEN_ALIGN16 Point {
    PCL_ADD_POINT4D;
    float intensity;
    double timestamp;
    uint16_t ring;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
}  // namespace pandar_ros
POINT_CLOUD_REGISTER_POINT_STRUCT(pandar_ros::Point,
                                  (float, x, x)(float, y, y)(float, z, z)(float, intensity,
                                                                          intensity)(double, timestamp,
                                                                                     timestamp)(uint16_t, ring, ring))
// 添加镭神点云定义
namespace leishen_ros {
struct EIGEN_ALIGN16 Point {
    PCL_ADD_POINT4D;
    PCL_ADD_INTENSITY;
    std::uint16_t ring;  // 行号
    double time;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
}  // namespace leishen_ros

POINT_CLOUD_REGISTER_POINT_STRUCT(leishen_ros::Point,
                                  (float, x, x)(float, y, y)(float, z, z)(float, intensity,
                                                                          intensity)(uint16_t, ring, ring)(double, time,
                                                                                                           time))

#endif