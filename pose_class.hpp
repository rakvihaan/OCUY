#include <string>
#include <vector>
#include <variant>
#include <yaml-cpp/yaml.h>

namespace pose_model
{
    // attribute abstract class
    class Attribute
    {
    protected: 
        std::string label;
        // std::string unit;
    public:
        // Attribute() {}
        Attribute(const std::string &label_t, const std::string &unit_t)
            : label(label_t), unit(unit_t) {}

        virtual void display() const = 0; //debug func to print members

        // virtual string getVal(){}
    };
    // feature abstract class
    class Feature
    {
    protected:
        std::string tag;
        int attribute_size;// dont need this anymore
        int feature_size;
        std::vector<std::unique_ptr<Attribute>> attributes;
        std::vector<std::unique_ptr<Feature>> children;

    public:
        Feature(std::string t, int attri_size, int feat_size) : tag(t), attribute_size(attri_size), feature_size(feat_size) {}
        virtual void display() const = 0;

        // functions to add attributes and children to their resp. vectors
        void addAttribute(std::unique_ptr<Attribute> attribute)
        {
            attributes.push_back(std::move(attribute));
        }

        void addFeature(std::unique_ptr<Feature> feature)
        {
            children.push_back(std::move(feature));
        }
    };

    class angle : public Attribute
    {
    protected:
        float datum;
        static std::string unit;
    public:
        angle(const std::string &label_t, const std::string &unit_t, const float datum_t)
            : Attribute(label_t, unit_t), datum(datum_t) {}

        static angle parse_yaml(const YAML::Node &node)
        {
            // std::cout << node << "\n";
            std::string label_t = "";
            std::string unit_t = "";
            float datum_t = 0.0f;
            try
            {
                if (node.IsDefined())
                {
                    label_t = node["label"].as<std::string>();
                    unit_t = node["unit"].as<std::string>();
                    if (node["datum"].IsDefined())
                    {
                        datum_t = node["datum"].as<float>();
                    }
                }
                return angle(label_t, unit_t, datum_t);
            }
            catch (const YAML::BadConversion &e)
            {
                std::cout << "bad yaml" << "\n";
            }
            // handle proper exception of not having proper angle vals
            return angle("", "", 0.0f);
        }

        void display() const override
        {
            std::cout << "Label: " << label << "\n"
                      << "Unit: " << unit << "\n"
                      << "Datum: " << datum << "\n\n";
        }
    };

    class coordinate : public Attribute
    {
    protected:
        float datum;

    public:
        coordinate(const std::string &label_t, const std::string &unit_t, const float datum)
            : Attribute(label_t, unit_t), datum(datum) {}

        static coordinate parse_yaml(const YAML::Node &node)
        {
            // std::cout<<node<<"\n";
            try
            {
                std::string label_t = "";
                std::string unit_t = "";
                float datum_t = 0.0f;
                if (node.IsDefined())
                {
                    label_t = node["label"].as<std::string>();
                    unit_t = node["unit"].as<std::string>();
                    if (node["datum"].IsDefined())
                    {
                        datum_t = node["datum"].as<float>();
                    }
                }
                return coordinate(label_t, unit_t, datum_t);
            }
            catch (const YAML::BadConversion &e)
            {
                std::cout << "coord bad yaml" << "\n";
            }
            // handle proper exception of not having proper angle vals
            return coordinate("", "", 0.0f);
        }

        void display() const override
        {
            std::cout << "Label: " << label << "\n"
                      << "Unit: " << unit << "\n"
                      << "Datum: " << datum << "\n\n";
        }
    };

    class Point : public Feature
    {
    public:
        Point(const std::string &tag_t, const int attribute_size_t, const int feature_size_t)
            : Feature(tag_t, attribute_size_t, feature_size_t)
        {} // feature setter should be called here

        static std::unique_ptr<Point> parse_yaml(const YAML::Node &node)//try to abstract it
        {
            // std::vector<std::unique_ptr<Attribute>> attr;
            // std::cout<<node<<"\n";
            try
            {
                if (node.IsDefined())
                {
                    std::string tag_t = node["tag"].as<std::string>();
                    int attribute_size_t = node["attribute_size"].as<int>();
                    int feature_size_t = node["feature_size"].as<int>();
                    auto point = std::make_unique<Point>(tag_t, attribute_size_t, feature_size_t);
                    if (node["attributes"].IsDefined())
                    {
                        // std::cout<<node["attributes"]<<"\n";
                        for (const auto &attributeNode : node["attributes"])
                        {
                            const YAML::Node &pointNode = attributeNode["coordinate"]; //use keys to identify the attribute
                            // std::cout<<pointNode<<"\n";
                            auto coord = std::make_unique<coordinate>(coordinate::parse_yaml(pointNode));
                            point->addAttribute(std::move(coord));
                            // point->addAttribute(std::make_unique<coordinate>(coordinate::parse_yaml(pointNode)));
                            // attr.push_back(std::move(coordinate::parse_yaml(pointNode)));
                        }
                    }
                    if (node["features"].IsDefined())
                    {
                        
                    }
                    return point;
                }
            }
            catch (const YAML::BadConversion &e)
            {
                std::cout << "Point bad yaml" << "\n";
            }
            return nullptr;
        }

        void display() const override
        {
            std::cout << "Display Point: " << "\n\n";
            std::cout << "Tag: " << tag << "\n"
                      << "Attribute size: " << attribute_size << "\n"
                      << "Feature size: " << feature_size << "\n"
                      << "---------------------\n";
            for (const auto &attr : attributes)
            {
                attr->display();
                std::cout << "-------\n";
            }
        }

    };

    class Orientation : public Feature
    {
    protected:
        // std::vector<angle> angles;

    public:
        // Orientation(const std::string &tag_t, const int attribute_size_t, const int feature_size_t, const std::vector<angle> &angles_t)
        // : Feature(tag_t, attribute_size_t, feature_size_t), angles(angles_t) {} // feature setter should be called here
        Orientation(const std::string &tag_t, int attribute_size_t, int feature_size_t)
            : Feature(tag_t, attribute_size_t, feature_size_t) {}

        static std::unique_ptr<Orientation> parse_yaml(const YAML::Node &node)
        {
            // std::vector<angle> angles;
            // std::cout << node << "\n";
            try
            {
                if (node.IsDefined())
                {
                    std::string tag_t = node["tag"].as<std::string>();
                    int attribute_size_t = node["attribute_size"].as<int>();
                    int feature_size_t = node["feature_size"].as<int>();

                    auto orientation = std::make_unique<Orientation>(tag_t, attribute_size_t, feature_size_t);

                    if (node["attributes"].IsDefined())
                    {
                        for (const auto &attributeNode : node["attributes"])
                        {
                            const YAML::Node &angleNode = attributeNode["angle"];
                            // orientation->addAttribute(std::make_unique<angle>(angle::parse_yaml(angleNode)));
                            auto ang = std::make_unique<angle>(angle::parse_yaml(angleNode));
                            orientation->addAttribute(std::move(ang));
                            // angles.push_back(angle::parse_yaml(angleNode));
                        }
                    }
                    return orientation;
                    // return Orientation(tag_t, attribute_size_t, feature_size_t, angles);
                }
            }
            catch (const YAML::InvalidNode &e)
            {
                std::cout << "orientation bad yaml" << "\n";
            }
            return nullptr;
        }

        void display() const override
        {
            std::cout << "Display Orientation: " << "\n\n";
            std::cout << "Tag: " << tag << "\n"
                      << "Attribute size: " << attribute_size << "\n"
                      << "Feature size: " << feature_size << "\n"
                      << "---------------------\n";
            for (const auto &attr : attributes)
            {
                attr->display();
                std::cout << "-------\n";
            }
            std::cout << "-------\n";
        }
    };

    class Pose : public Feature
    {
    public:
        Pose(const std::string &tag_t, int attribute_size_t, int feature_size_t)
            : Feature(tag_t, attribute_size_t, feature_size_t) {}

        static std::unique_ptr<Pose> parse_yaml(const YAML::Node &node)
        {
            // std::cout << node << "\n";
            std::string tag_t = node["tag"].as<std::string>();
            int attribute_size_t = node["attribute_size"].as<int>();
            int feature_size_t = node["feature_size"].as<int>();
            try
            {
                if (node.IsDefined())
                {
                    if (node["features"].IsDefined())
                    {
                        auto pose = std::make_unique<Pose>(tag_t, attribute_size_t, feature_size_t);
                        // std::cout<<node["features"]<<"\n";
                        for (const auto &attributeNode : node["features"])
                        {
                            // std::cout<<attributeNode<<"\n";
                            if (attributeNode["Orientation"].IsDefined()) //checking for particular doesnt make it abstract, need to work on this
                            {
                                const YAML::Node &orientationNode = attributeNode["Orientation"];
                                auto orientation = Orientation::parse_yaml(orientationNode);
                                pose->addFeature(std::move(orientation));
                            }
                            if (attributeNode["Point"].IsDefined())
                            {
                                const YAML::Node &pointNode = attributeNode["Point"];
                                auto point = Point::parse_yaml(pointNode);
                                pose->addFeature(std::move(point));
                            }
                        }
                        return pose;
                    }
                }
            }
            catch (const YAML::InvalidNode &e)
            {
                std::cout << "Pose bad yaml" << "\n";
            }
            return nullptr;
        }

        void display() const override
        {
            std::cout << "Tag: " << tag << "\n"
                      << "Attribute size: " << attribute_size << "\n"
                      << "Feature size: " << feature_size << "\n"
                      << "---------------------\n";
            for (const auto &child : children)
            {
                child->display(); // Polymorphic call to child's display method
            }
        }
    };

}