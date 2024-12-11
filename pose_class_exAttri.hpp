#include <string>
#include <vector>
#include <variant>
#include <yaml-cpp/yaml.h>

// attribute abstract class
class Attribute
{
protected: // looking into why we are not able to set private members
    std::string label;
    std::string unit;
    // std::vector<std::variant<angle,coord>> attributes; //since its abstract, cant set particular attribute class
    // std::vector<coord> coords;
public:
    Attribute() {}
    Attribute(const std::string &label_t, const std::string &unit_t)
        : label(label_t), unit(unit_t) {}

    virtual void display() const = 0;
};
// attribute feature class
class Feature
{
protected:
    std::string tag;
    int attribute_size;
    int feature_size;
    // std::vector<Attribute> attributes;
    // std::vector<Feature> children;
public:
    Feature(std::string t, int attri_size, int feat_size) : tag(t), attribute_size(attri_size), feature_size(feat_size) {}
    virtual void display() const = 0;
    // virtual void func() {}
};

class angle : public Attribute
{
protected:
    float datum;

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

class coord : public Attribute
{
protected:
    float datum;

public:
    coord(const std::string &label_t, const std::string &unit_t, const float datum)
        : Attribute(label_t, unit_t), datum(datum) {}

    static coord parse_yaml(const YAML::Node &node)
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
            return coord(label_t, unit_t, datum_t);
        }
        catch (const YAML::BadConversion &e)
        {
            std::cout << "coord bad yaml" << "\n";
        }
        // handle proper exception of not having proper angle vals
        return coord("", "", 0.0f);
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
protected:
    std::vector<coord> coords; // a vecotr of attribute is set here as i need members from coord, cannot access if its in parent class
public:
    Point(const std::string &tag_t, const int attribute_size_t, const int feature_size_t, const std::vector<coord> &coords_t)
        : Feature(tag_t, attribute_size_t, feature_size_t), coords(coords_t) {} // feature setter should be called here

    static Point parse_yaml(const YAML::Node &node)
    {
        std::vector<coord> coords;
        // std::cout<<node<<"\n";
        try
        {
            if (node.IsDefined())
            {
                std::string tag_t = node["tag"].as<std::string>();
                int attribute_size_t = node["attribute_size"].as<int>();
                int feature_size_t = node["feature_size"].as<int>();
                if (node["attributes"].IsDefined())
                {
                    for (const auto &attributeNode : node["attributes"])
                    {
                        const YAML::Node &pointNode = attributeNode["coordinate"];
                        coords.push_back(coord::parse_yaml(pointNode));
                    }
                    return Point(tag_t, attribute_size_t, feature_size_t, coords);
                }
            }
        }
        catch (const YAML::BadConversion &e)
        {
            std::cout << "Point bad yaml" << "\n";
        }
    }

    void display() const override
    {
        std::cout << "Display Point: " << "\n\n";
        std::cout << "Tag: " << tag << "\n"
                  << "Attribute size: " << attribute_size << "\n"
                  << "Feature size: " << feature_size << "\n"
                  << "---------------------\n";
        for (const auto &coord : coords)
        {
            coord.display();
            std::cout << "-------\n";
        }
        std::cout << "-------\n";
    }
};

class Orientation : public Feature
{
protected:
    std::vector<angle> angles;

public:
    Orientation(const std::string &tag_t, const int attribute_size_t, const int feature_size_t, const std::vector<angle> &angles_t)
        : Feature(tag_t, attribute_size_t, feature_size_t), angles(angles_t) {} // feature setter should be called here

    static Orientation parse_yaml(const YAML::Node &node)
    {
        std::vector<angle> angles;
        // std::cout << node << "\n";
        try
        {
            if (node.IsDefined())
            {
                std::string tag_t = node["tag"].as<std::string>();
                int attribute_size_t = node["attribute_size"].as<int>();
                int feature_size_t = node["feature_size"].as<int>();
                if (node["attributes"].IsDefined())
                {
                    for (const auto &attributeNode : node["attributes"])
                    {
                        const YAML::Node &angleNode = attributeNode["angle"];
                        angles.push_back(angle::parse_yaml(angleNode));
                    }
                }
                return Orientation(tag_t, attribute_size_t, feature_size_t, angles);
            }
        }
        catch (const YAML::InvalidNode &e)
        {
            std::cout << "orientation bad yaml" << "\n";
        }
    }

    void display() const override
    {
        std::cout << "Display Orientation: " << "\n\n";
        std::cout << "Tag: " << tag << "\n"
                  << "Attribute size: " << attribute_size << "\n"
                  << "Feature size: " << feature_size << "\n"
                  << "---------------------\n";
        for (const auto &angle : angles)
        {
            angle.display();
            std::cout << "-------\n";
        }
        std::cout << "-------\n";
    }
};

class Pose : public Feature
{
protected:
    std::vector<std::variant<Point, Orientation>> features;
    // std::vector<Orientation> Orientations;
public:
    // Pose(std::string tag_t, int attribute_size_t, int feature_size_t, std::vector<Point> Points, std::vector<Orientation> Orientations): Feature(tag_t, attribute_size_t, feature_size_t), Points(Points), Orientations(Orientations){}
    Pose(std::string tag_t, int attribute_size_t, int feature_size_t, std::vector<std::variant<Point, Orientation>> features)
        : Feature(tag_t, attribute_size_t, feature_size_t), features(features) {}
        
    static Pose parse_yaml(const YAML::Node &node)
    {
        // std::vector<Point> Points;
        // std::vector<Orientation> Orientations;
        std::vector<std::variant<Point, Orientation>> features;

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
                    // std::cout<<node["features"]<<"\n";
                    for (const auto &attributeNode : node["features"])
                    {
                        // std::cout<<attributeNode<<"\n";
                        if (attributeNode["Orientation"].IsDefined())
                        {
                            const YAML::Node &orientationNode = attributeNode["Orientation"];
                            // std::cout<<orientationNode<<"\n";
                            features.push_back(Orientation::parse_yaml(orientationNode));
                        }
                        if (attributeNode["Point"].IsDefined())
                        {
                            const YAML::Node &pointNode = attributeNode["Point"];
                            // std::cout<<pointNode <<"\n";
                            features.push_back(Point::parse_yaml(pointNode));
                        }
                    }
                }
                return Pose(tag_t, attribute_size_t, feature_size_t, features);
            }
        }
        catch (const YAML::InvalidNode &e)
        {
            std::cout << "Pose bad yaml" << "\n";
        }
    }

    void display() const override
    {
        std::cout << "Tag: " << tag << "\n"
                  << "Attribute size: " << attribute_size << "\n"
                  << "Feature size: " << feature_size << "\n"
                  << "---------------------\n";
        for (const auto &feature : features)
        {
            std::visit([](const auto &arg)
                       { arg.display(); }, feature);
        }
    }
};