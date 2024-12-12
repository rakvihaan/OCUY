#include <yaml-cpp/yaml.h>
#include <iostream>
#include "pose_class.hpp"




int main() {
    YAML::Node config = YAML::LoadFile("DNA/Model/pose.yaml");
    for (const auto& poseNode : config){
        YAML::Node node = poseNode["Pose"];
        // std::cout << node["features"]["Orientation"]<< std::endl;
        // std::cout << node["features"][1]["Orientation"] << std::endl;
        // std::unique_ptr<pose_model::Orientation> angle_test = pose_model::Orientation::parse_yaml(node["features"]["Orientation"]);
        // angle_test->display();
        // std::cout << node["features"][0]["Point"] << std::endl;
        // std::unique_ptr<pose_model::Point> point_test = pose_model::Point::parse_yaml(node["features"][0]["Point"]);
        // point_test->display();    
        std::unique_ptr<pose_model::Pose> pose = pose_model::Pose::parse_yaml(node);
        pose->display();
    }

    return 0;
}