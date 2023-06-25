#pragma once

#include <optional>

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "GameLoop/IUpdatable.h"
#include "Utils/Common.h"
#include "Utils/PoolObjects.h"

class World;

/**
 * The structure responsible for how to draw this damage cell
 */
struct DrawDamageInfo
{
    /**
     * position relative to the map
     */
    const Vec2Int position;
    /**
     * The color that the damage cell will be painted with
     */
    const cocos2d::Color3B color;
    /**
     * Damage that will be dealt to the entity in this cell
     */
    const std::optional<float> damage;
};

/**
 * A class that is a display of damage cells on the map
 */
class DamageIndicator : public cocos2d::Node
{
public:
    /**
     * Static method for creating a damage indicator \n
     * Additionally puts a new object in autorealease queue
     * @param world world in which this entity was created
     * @return new damage indicator
     */
    static DamageIndicator* create(World* world);

    /**
     * Calls when creating an class
     */
    bool init() override;

    /**
     * Sets in this cell the color and damage that will be displayed for the player
     * @param color by which the damage designation sprite will be multiplied 
     * @param damage displayed on this cell
     */
    void setColorAndDamage(cocos2d::Color3B color, std::optional<float> damage);
    /**
     * Allows you to add or reduce damage
     * @param changeValue by what value will the amount of damage be changed
     */
    void changeDamageValueBy(float changeValue);

    /**
     * Sets whether the damage indicator is visible
     * @param visible   true if the node is visible, false if the node is hidden.
     */
    void setVisible(bool visible) override;
    /**
     * Sets the position (x,y) of the node in its parent's coordinate system.
     * @param position The position (x,y) of the node in OpenGL coordinates.
     */
    void setPosition(const cocos2d::Vec2& position) override;
    /**
     * Sets the position (x,y) of the node in its parent's coordinate system.
     * @param position The position (x,y) of the node in OpenGL coordinates.
     */
    void setPosition(float x, float y) override;

private:
    DamageIndicator(World* world);

    float m_currentDamage;
    cocos2d::Sprite* m_sprite;
    cocos2d::Label* m_label;
    World* m_world;
};

class DamageIndicatorsSystems : public cocos2d::Node, public IUpdatable
{
public:
    /**
     * Static method for creating a damage indicators system \n
     * Additionally puts a new object in autorealease queue
     * @param world world in which this entity was created
     * @return new damage indicators system
     */
    static DamageIndicatorsSystems* create(World* world);
 
    /**
     * Allows you to schedule the drawing of the damage indicator until the end of the world update
     * @param scheduleDrawFunc functor that allows you to get information about damage rendering
     */
    void scheduleDraw(const std::function<void(std::function<void(DrawDamageInfo)>)>& scheduleDrawFunc);
 
    /**
     * Updates the system state -- draws all scheduled indicators and clears the queue
     */
    void update() override;

private:
    DamageIndicatorsSystems(World* world);

    void reset();
    
    std::map<Vec2Int, DamageIndicator*> m_showedObjects;
    PoolObjects<DamageIndicator> m_indicators;
    World* m_world;

    std::vector<std::function<void(std::function<void(DrawDamageInfo)>)>> m_deferredDraws;
};
