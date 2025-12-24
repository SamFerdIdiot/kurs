#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>



enum class ResourceType {
    MONEY,
    FUEL,
    DURABILITY,
    FOOD,
    WATER
};



namespace std {
    template <>
    struct hash<ResourceType> {
        size_t operator()(const ResourceType& type) const {
            return static_cast<size_t>(type);
        }
    };
}
















class ResourceManager {
public:

    ResourceManager();



    float getResource(ResourceType type) const;





    void setResource(ResourceType type, float value);






    bool modifyResource(ResourceType type, float amount);






    bool hasEnoughResource(ResourceType type, float amount) const;



    float getMaxResource(ResourceType type) const;





    void setMaxResource(ResourceType type, float maxValue);


    void resetResources();


    void resetResource(ResourceType type);


    float getDefaultResource(ResourceType type) const;










private:

    std::unordered_map<ResourceType, float> m_resources;


    std::unordered_map<ResourceType, float> m_maxResources;


    std::unordered_map<ResourceType, float> m_defaultResources;


    void initializeDefaults();


    float clampResource(float value, float min, float max) const;
};

#endif
