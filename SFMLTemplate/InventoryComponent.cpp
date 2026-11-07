#include "InventoryComponent.h"

#include "Constants.h"
#include "HealthComponent.h"
#include "Loger.h"
#include "StaminaComponent.h"

#include <algorithm>
#include <limits>
#include <random>

namespace XYZRoguelike
{
    const char* GetItemTextureName(ItemType type)
    {
        switch (type)
        {
        case ItemType::Sword:
            return "item_sword";
        case ItemType::Bow:
            return "item_bow";
        case ItemType::HealingPotion:
            return "item_health_potion";
        case ItemType::ShieldPotion:
            return "item_shield_potion";
        case ItemType::StaminaPotion:
            return "item_stamina_potion";
        case ItemType::Key:
            return "item_key";
        case ItemType::Empty:
        default:
            return "";
        }
    }

    const char* GetItemDisplayName(ItemType type)
    {
        switch (type)
        {
        case ItemType::Sword:
            return "Sword";
        case ItemType::Bow:
            return "Bow";
        case ItemType::HealingPotion:
            return "Health potion";
        case ItemType::ShieldPotion:
            return "Shield potion";
        case ItemType::StaminaPotion:
            return "Stamina potion";
        case ItemType::Key:
            return "Key";
        case ItemType::Empty:
        default:
            return "Empty";
        }
    }

    InventoryComponent::InventoryComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject)
    {
    }

    void InventoryComponent::Update(float deltaTime) {}

    void InventoryComponent::Render() {}

    bool InventoryComponent::AddItem(ItemType type, unsigned int quantity)
    {
        if (!CanAddItem(type, quantity))
        {
            return false;
        }

        const bool stackable = type == ItemType::HealingPotion || type == ItemType::ShieldPotion ||
                               type == ItemType::StaminaPotion || type == ItemType::Key;
        if (stackable)
        {
            auto existingStack = std::find_if(items.begin(), items.end(), [type](const auto& stack)
                                              { return stack.type == type; });
            if (existingStack != items.end())
            {
                const unsigned int capacity =
                    std::numeric_limits<unsigned int>::max() - existingStack->quantity;
                if (capacity < quantity)
                {
                    return false;
                }

                existingStack->quantity += quantity;
                LOG_INFO(std::string("Inventory stack increased: ") + GetItemDisplayName(type));
                return true;
            }
        }
        else if (HasItem(type))
        {
            return true;
        }

        auto freeSlot = std::find_if(items.begin(), items.end(),
                                     [](const auto& stack) { return stack.quantity == 0; });
        if (freeSlot == items.end())
        {
            LOG_WARN("Inventory is full; item remains in the world");
            return false;
        }

        *freeSlot = {type, quantity};
        LOG_INFO(std::string("Item added to inventory: ") + GetItemDisplayName(type));
        return true;
    }

    ItemType InventoryComponent::AddRandomPotionDrop()
    {
        static std::mt19937 randomEngine(std::random_device{}());
        static const ItemType potionTypes[] = {ItemType::HealingPotion, ItemType::ShieldPotion,
                                               ItemType::StaminaPotion};
        std::uniform_int_distribution<std::size_t> choosePotion(0, 2);
        const ItemType potion = potionTypes[choosePotion(randomEngine)];
        return AddItem(potion) ? potion : ItemType::Empty;
    }

    bool InventoryComponent::CanAddItem(ItemType type, unsigned int quantity) const
    {
        if (type == ItemType::Empty || quantity == 0)
        {
            return false;
        }

        const bool stackable = type == ItemType::HealingPotion || type == ItemType::ShieldPotion ||
                               type == ItemType::StaminaPotion || type == ItemType::Key;
        if (stackable)
        {
            auto existingStack = std::find_if(items.begin(), items.end(), [type](const auto& stack)
                                              { return stack.type == type; });
            if (existingStack != items.end())
            {
                return quantity <=
                       std::numeric_limits<unsigned int>::max() - existingStack->quantity;
            }
        }
        else if (HasItem(type))
        {
            return true;
        }

        return std::any_of(items.begin(), items.end(),
                           [](const auto& stack) { return stack.quantity == 0; });
    }

    bool InventoryComponent::UseItem(std::size_t slotIndex)
    {
        if (slotIndex >= items.size() || items[slotIndex].quantity == 0)
        {
            return false;
        }

        bool effectApplied = false;
        switch (items[slotIndex].type)
        {
        case ItemType::HealingPotion:
        {
            auto* health = gameObject->GetComponent<HealthComponent>();
            effectApplied = health != nullptr && health->Heal(PLAYER_HEALING_POTION_AMOUNT) > 0;
            break;
        }
        case ItemType::ShieldPotion:
        {
            auto* health = gameObject->GetComponent<HealthComponent>();
            effectApplied =
                health != nullptr && health->RestoreArmor(PLAYER_SHIELD_POTION_AMOUNT) > 0;
            break;
        }
        case ItemType::StaminaPotion:
        {
            auto* stamina = gameObject->GetComponent<StaminaComponent>();
            effectApplied = stamina != nullptr && stamina->ApplyStaminaPotion();
            break;
        }
        case ItemType::Empty:
        case ItemType::Sword:
        case ItemType::Bow:
        case ItemType::Key:
        default:
            return false;
        }

        if (!effectApplied)
        {
            return false;
        }

        ConsumeOne(slotIndex);
        LOG_INFO("Inventory item used");
        return true;
    }

    bool InventoryComponent::UseSelectedItem()
    {
        return UseItem(selectedSlot);
    }

    bool InventoryComponent::ConsumeSelectedKey()
    {
        if (GetSelectedItem() != ItemType::Key || !ConsumeOne(selectedSlot))
        {
            return false;
        }

        if (items[0].type == ItemType::Sword && items[0].quantity > 0)
        {
            selectedSlot = 0;
        }

        return true;
    }

    bool InventoryComponent::SelectSlot(std::size_t slotIndex)
    {
        if (slotIndex >= items.size())
        {
            return false;
        }

        selectedSlot = slotIndex;
        return true;
    }

    bool InventoryComponent::HasItem(ItemType type) const
    {
        return std::any_of(items.begin(), items.end(), [type](const auto& stack)
                           { return stack.type == type && stack.quantity > 0; });
    }

    const InventorySlots& InventoryComponent::GetItems() const
    {
        return items;
    }

    std::size_t InventoryComponent::GetSelectedSlot() const
    {
        return selectedSlot;
    }

    ItemType InventoryComponent::GetSelectedItem() const
    {
        return selectedSlot < items.size() && items[selectedSlot].quantity > 0
                   ? items[selectedSlot].type
                   : ItemType::Empty;
    }

    bool InventoryComponent::ConsumeOne(std::size_t slotIndex)
    {
        if (slotIndex >= items.size() || items[slotIndex].quantity == 0)
        {
            return false;
        }

        --items[slotIndex].quantity;
        if (items[slotIndex].quantity == 0)
        {
            items[slotIndex] = {};
        }

        return true;
    }
} // namespace XYZRoguelike
