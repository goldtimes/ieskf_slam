/*
 * @Author: lihang 1019825699@qq.com
 * @Date: 2024-04-16 23:10:10
 * @LastEditors: lihang 1019825699@qq.com
 * @LastEditTime: 2024-04-16 23:18:30
 * @FilePath: /lio_ws/src/ieskf_slam/src/lidar_process/lidar_process.hh
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#pragma once
#include <livox_ros_driver/CustomMsg.h>
#include <pcl_conversions/pcl_conversions.h>
#include <yaml-cpp/yaml.h>
#include "common/common_headers.h"
#include "sensors/point_types.hh"

namespace ctlio::slam {
/**
 * @brief 处理多种雷达数据
 */
class LidarProcess {
   public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    enum class LidarType {
        AVIA = 1,
        VELO32,
        OUST64,
        ROBOSENSE16,
        PANDAR,
        LEISHEN,
    };

    LidarProcess() = default;
    ~LidarProcess() = default;

    LidarType lidar_type_ = LidarType::ROBOSENSE16;
    // 加载配置文件
    void LoadFromYaml(const std::string& yaml);

    const double GetTimeSpan() const {
        return timespan_;
    }

    void Process(const livox_ros_driver::CustomMsg::ConstPtr& msg, std::vector<point3D>& pcl_out);
    void Process(const sensor_msgs::PointCloud2::ConstPtr& msg, std::vector<point3D>& pcl_out);

   private:
    void AviaHandler(const livox_ros_driver::CustomMsg::ConstPtr& msg);
    void Oust64Handler(const sensor_msgs::PointCloud2::ConstPtr& msg);
    void VelodyneHandler(const sensor_msgs::PointCloud2::ConstPtr& msg);
    void RobosenseHandler(const sensor_msgs::PointCloud2::ConstPtr& msg);
    void PandarHandler(const sensor_msgs::PointCloud2::ConstPtr& msg);
    void LeishenHandler(const sensor_msgs::PointCloud2::ConstPtr& msg);

   private:
    int point_filter_num = 1;  // 跳点
    double blind = 0.1;        // 盲区
    double timespan_;          // 时间间隔

    std::vector<point3D> cloud_full_;
    std::vector<point3D> cloud_out_;
};
}  // namespace ctlio::slam