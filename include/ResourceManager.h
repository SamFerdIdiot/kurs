#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>

// Enum defining all resource types in the game
// These resources are consumed and managed during gameplay
enum class ResourceType {
    MONEY,          // Currency for purchasing items and services
    FUEL,           // Gas for the vehicle
    DURABILITY,     // Vehicle condition/health
    FOOD,           // Player hunger resource
    WATER           // Player hydration resource
};

// Hash function specialization for ResourceType enum
// Required for using ResourceType as key in std::unordered_map
namespace std {
    template <>
    struct hash<ResourceType> {
        size_t operator()(const ResourceType& type) const {
            return static_cast<size_t>(type);
        }
    };
}

// ResourceManager class
// Purpose: Centralized management of game resources (money, fuel, durability, food, water)
// Features:
//   - Get and set resource values
//   - Add/subtract resources with validation
//   - Check if sufficient resources are available
//   - Get maximum values for resources
//   - Reset resources to default values
//   - Save/load resource state (stub for future implementation)
//
// Заметки по дизайну:
//   - Uses std::unordered_map for flexible resource storage
//   - All resources have min (0) and max values
//   - Thread-safe methods can be added in future if needed
//   - Resource events/notifications can be added for UI updates
class ResourceManager {
public:
    // Конструктор - инициализирует ресурсы значениями по умолчанию
    ResourceManager();
    
    // Get the current value of a resource
    // Returns: Current resource amount (0 if resource doesn't exist)
    float getResource(ResourceType type) const;
    
    // Set a resource to a specific value
    // Parameters:
    //   - type: The resource to modify
    //   - value: New value (will be clamped to min/max)
    void setResource(ResourceType type, float value);
    
    // Add to a resource (positive) or subtract (negative)
    // Parameters:
    //   - type: The resource to modify
    //   - amount: Amount to add (negative to subtract)
    // Returns: True if operation succeeded, false if it would go below 0
    bool modifyResource(ResourceType type, float amount);
    
    // Check if there are enough resources available
    // Parameters:
    //   - type: The resource to check
    //   - amount: Required amount
    // Returns: True if current amount >= required amount
    bool hasEnoughResource(ResourceType type, float amount) const;
    
    // Get the maximum value for a resource
    // Returns: Maximum allowed value for the resource
    float getMaxResource(ResourceType type) const;
    
    // Set the maximum value for a resource
    // Parameters:
    //   - type: The resource type
    //   - maxValue: New maximum value
    void setMaxResource(ResourceType type, float maxValue);
    
    // Reset all resources to their default values
    void resetResources();
    
    // Reset a specific resource to its default value
    void resetResource(ResourceType type);
    
    // Get the default value for a resource
    float getDefaultResource(ResourceType type) const;
    
    // TODO: Future extensions
    // - Add resource consumption rates
    // - Add resource regeneration rates (e.g., energy over time)
    // - Add resource events/callbacks for UI updates
    // - Add save/load functionality
    // - Add resource thresholds and warnings
    // - Add multipliers/modifiers for difficulty levels
    // - Add resource conversion (e.g., money to fuel)
    
private:
    // Current values for each resource
    std::unordered_map<ResourceType, float> m_resources;
    
    // Maximum values for each resource
    std::unordered_map<ResourceType, float> m_maxResources;
    
    // Default starting values for each resource
    std::unordered_map<ResourceType, float> m_defaultResources;
    
    // Initialize default and max values
    void initializeDefaults();
    
    // Clamp a value between min and max
    float clampResource(float value, float min, float max) const;
};

#endif // RESOURCE_MANAGER_H
