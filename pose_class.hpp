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

    public:
        Attribute() {}
        Attribute(const YAML::Node &node)
        {
            label = "";
            try
            {
                if (node.IsDefined() && node["label"].IsDefined())
                {
                    label = node["label"].as<std::string>();
                }
            }
            catch (const YAML::BadConversion &e)
            {
                std::cout << "bad yaml" << "\n";
            }
        }

        virtual void display() const = 0; // debug func to print members

        virtual string getVal(){}
    };
    // feature abstract class
    class Feature
    {
    protected:
        std::string tag;
        int attribute_size; // dont need this anymore
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

    using FeatureCreator = std::function<std::unique_ptr<Feature>(const YAML::Node&)>;

    class FeatureFactory {
    private:
        // Map to store feature type name to its creation function
        // Defined as static to be accessible across all instances
        static std::unordered_map<std::string, FeatureCreator> creators;

    public:
        template<typename T>
        static void registerType(const std::string& typeName) {
            creators[typeName] = [](const YAML::Node& node) {
                return T::parse_yaml(node);
            };
        }

        static std::unique_ptr<Feature> createFeature(const std::string& featureType, const YAML::Node& node) {
            auto it = creators.find(featureType);
            if (it != creators.end()) {
                return it->second(node);
            }
            // Could throw an exception or log a warning here
            return nullptr;
        }
    };

// Initialize the static map
    std::unordered_map<std::string, FeatureCreator> FeatureFactory::creators;

    class angle : public Attribute
    {
    protected:
        float datum;
        static std::string unit;
        static std::string getStringFormat = "%.2f";
        static std::string setStringFormat = "%f";
    public:
        angle(const YAML::Node &node, const std::string &unit_t, const float datum_t)
            : Attribute(node), datum(datum_t) { angle::unit = unit_t; }

        static angle parse_yaml(const YAML::Node &node)
        {
            std::string unit_t = "";
            float datum_t = 0.0f;
            // std::cout<<node;
            angle angle(node, unit_t, datum_t);

            try
            {
                if (node.IsDefined())
                {
                    if (node["unit"].IsDefined())
                    {
                        angle.unit = node["unit"].as<std::string>();
                    }
                    if (node["datum"].IsDefined())
                    {
                        angle.datum = node["datum"].as<float>();
                    }
                }
                return angle;
            }
            catch (const YAML::BadConversion &e)
            {
                std::cout << "bad yaml" << "\n";
            }
            // handle proper exception of not having proper angle vals
            return angle;
        }

        static void setGetterStringFormat(std::string format){

        }

        static void setSetterStringFormat(std::string format){
            
        }

        std::string getValueAsStr(){
            char buffer[64];
            snprintf(buffer, sizeof(buffer), getStringFormat.c_str(), datum);
            return std::string(buffer);
        }

        void setValue(std::string value){
            scanf(setStringFormat,datum);
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
        static std::string unit;
        static std::string getStringFormat = "%.2f";
        static std::string setStringFormat = "%f";
    public:
        coordinate(const YAML::Node &node, const std::string &unit_t, const float datum_t)
            : Attribute(node), datum(datum_t) { coordinate::unit = unit_t; }

        static coordinate parse_yaml(const YAML::Node &node)
        {
            std::string unit_t = "";
            float datum_t = 0.0f;

            coordinate coordinate(node, unit_t, datum_t);
            try
            {
                if (node.IsDefined())
                {
                    if (node["unit"].IsDefined())
                    {
                        coordinate.unit = node["unit"].as<std::string>();
                    }
                    if (node["datum"].IsDefined())
                    {
                        coordinate.datum = node["datum"].as<float>();
                    }
                }
                return coordinate;
            }
            catch (const YAML::BadConversion &e)
            {
                std::cout << "coord bad yaml" << "\n";
            }
            // handle proper exception of not having proper angle vals
            return coordinate;
        }

        static void setGetterStringFormat(std::string format){

        }

        static void setSetterStringFormat(std::string format){
            
        }

        std::string getValueAsStr(){
            char buffer[64];
            snprintf(buffer, sizeof(buffer), getStringFormat.c_str(), datum);
            return std::string(buffer);
        }

        void setValue(std::string value){
            scanf(setStringFormat,datum);
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
        {
        } // feature setter should be called here

        static std::unique_ptr<Point> parse_yaml(const YAML::Node &node) // try to abstract it
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
                            const YAML::Node &pointNode = attributeNode["coordinate"]; // use keys to identify the attribute
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
            : Feature(tag_t, attribute_size_t, feature_size_t) {
            static bool registered = false;
            if (!registered) {
                FeatureFactory::registerType<Point>("Point");
                FeatureFactory::registerType<Orientation>("Orientation");
                registered = true;
            }
            }

        static std::unique_ptr<Pose> parse_yaml(const YAML::Node &node)
        {
            // std::cout << node << "\n";
            std::string tag_t = node["tag"].as<std::string>();
            int attribute_size_t = node["attribute_size"].as<int>();
            int feature_size_t = node["feature_size"].as<int>();
            auto pose = std::make_unique<Pose>(tag_t, attribute_size_t, feature_size_t);
            try
            {
                if (node["features"].IsDefined()) {
                for (const auto& featureNode : node["features"]) {
                    for (const auto& feature : featureNode) {
                        std::string featureType = feature.first.as<std::string>();
                        auto newFeature = FeatureFactory::createFeature(featureType, feature.second);
                        if (newFeature) {
                            pose->addFeature(std::move(newFeature));
                        }
                    }
                }
            }
            return pose;
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
std::string pose_model::angle::unit;
std::string pose_model::coordinate::unit;