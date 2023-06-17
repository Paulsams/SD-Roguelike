cmake_minimum_required(VERSION 3.18.3)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

set(APP_NAME roguelike)

project(${APP_NAME})
include_directories(Include)

if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        set(CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

if(NOT DEFINED BUILD_ENGINE_DONE) # to test roguelike into root project
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O3")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )

if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

list(APPEND GAME_HEADER
    Include/AppDelegate.h
    Include/GameScene.h

    Include/GameLoop/GameLoop.h
    Include/GameLoop/IUpdatable.h

    Include/Player/Backpack.h
    Include/Player/Player.h
    Include/Player/PlayerInput.h

    Include/ItemsSystem/Accessory.h
    Include/ItemsSystem/AttackWithDamage.h
    Include/ItemsSystem/AttackHandler.h
    Include/ItemsSystem/Attacks.h
    Include/ItemsSystem/BaseItem.h
    Include/ItemsSystem/ConsumableItem.h
    Include/ItemsSystem/Damage.h
    Include/ItemsSystem/Items.h
    Include/ItemsSystem/IVisualAttack.h
    Include/ItemsSystem/Weapon.h
    
    Include/ItemsSystem/AttackSearch/AttackSearchFromStraightLines.h
    Include/ItemsSystem/AttackSearch/IAttackSearch.h

    Include/Mobs/Mob.h
    Include/Mobs/Strategy/BaseStateStrategy.h
    Include/Mobs/Strategy/IState.h
    Include/Mobs/Strategy/NothingStrategy.h
    Include/Mobs/Strategy/IMobStrategy.h
    Include/Mobs/Strategy/StateMachine.h
    Include/Mobs/Strategy/StrategyWithStates.h
    Include/Mobs/Strategy/Normal.h
    Include/Mobs/Strategy/Panic.h
    Include/Mobs/Behaviour/IMobBehaviour.h
    Include/Mobs/Behaviour/AggressiveBehaviour.h
    Include/Mobs/Behaviour/CowardlyBehaviour.h
    Include/Mobs/Behaviour/PassiveBehaviour.h
    Include/Mobs/Factory/BaseMobAbstractFactory.h
    Include/Mobs/Factory/MobsConfig.h
    Include/Mobs/Factory/MobInfo.h
    Include/Mobs/Factory/MobsFactoryFromConfig.h
    
    Include/Pathfinder/Graph.h
    Include/Pathfinder/Node.h
    Include/Pathfinder/PathfinderAStar.h
    Include/Pathfinder/IPathfindingAlgorithm.h

    Include/Stats/IHaveStats.h
    Include/Stats/IStat.h
    Include/Stats/IStatsContainer.h
    Include/Stats/LevelHandler.h
    Include/Stats/StatsContainer.h
    Include/Stats/StatType.h

    Include/Stats/Modificators/BoundsModificator.h
    Include/Stats/Modificators/IStatModificator.h
    Include/Stats/Modificators/StatWithModificators.h

    Include/UI/Canvas.h
    Include/UI/InventoryView.h
    Include/UI/MenuItemForInventory.h
    Include/UI/PlayerItemsOnUI.h
    Include/UI/StatBar.h

    Include/Utils/Common.h
    Include/Utils/Direction.h
    Include/Utils/EventsSystem.h
    Include/Utils/FontsTTF.h
    Include/Utils/PoolObjects.h
    Include/Utils/TMXGenerator.h
    Include/Utils/ValueNotifyChanged.h
    
    Include/WorldSystem/BaseEntity.h
    Include/WorldSystem/Chest.h
    Include/WorldSystem/DamageIndicatorsSystems.h
    Include/WorldSystem/Decoration.h
    Include/WorldSystem/FunctionVisitorEntities.h
    Include/WorldSystem/RandomGeneratorWorldBuilder.h
    Include/WorldSystem/ReadFileWorldBuilder.h
    Include/WorldSystem/IVisitorEntities.h
    Include/WorldSystem/World.h
    Include/WorldSystem/WorldTileConfig.h
    )

# add cross-platforms source files and header files
list(APPEND GAME_SOURCE
    Source/AppDelegate.cpp
    Source/GameScene.cpp

    Source/GameLoop/GameLoop.cpp

    Source/Player/Backpack.cpp
    Source/Player/Player.cpp
    Source/Player/PlayerInput.cpp

    Source/ItemsSystem/AttackHandler.cpp
    Source/ItemsSystem/Attacks.cpp
    Source/ItemsSystem/BaseItem.cpp
    Source/ItemsSystem/Items.cpp
    Source/ItemsSystem/Weapon.cpp

    Source/Mobs/Mob.cpp
    Source/Mobs/Strategy/StateMachine.cpp
    Source/Mobs/Strategy/StrategyWithStates.cpp
    Source/Mobs/Strategy/Normal.cpp
    Source/Mobs/Behaviour/AggressiveBehaviour.cpp
    Source/Mobs/Behaviour/CowardlyBehaviour.cpp
    Source/Mobs/Behaviour/PassiveBehaviour.cpp
    Source/Mobs/Factory/MobsConfig.cpp

    Source/Pathfinder/PathfinderAStar.cpp

    Source/Stats/StatsContainer.cpp
    Source/Stats/Modificators/StatWithModificators.cpp

    Source/UI/Canvas.cpp
    Source/UI/InventoryView.cpp
    Source/UI/MenuItemForInventory.cpp
    Source/UI/PlayerItemsOnUI.cpp
    Source/UI/StatBar.cpp

    Source/Utils/Common.cpp
    Source/Utils/TMXGenerator.cpp

    Source/WorldSystem/BaseEntity.cpp
    Source/WorldSystem/DamageIndicatorsSystems.cpp
    Source/WorldSystem/World.cpp
    Source/WorldSystem/ReadFileWorldBuilder.cpp
    Source/WorldSystem/RandomGeneratorWorldBuilder.cpp
    )

if(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

add_executable(${APP_NAME} ${all_code_files})
target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
    PRIVATE Classes
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})

if(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()


#enable google tests
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../tests)
add_executable(test_base tests/baseTest.cpp)
add_executable(test_pathfinder tests/pathfinderTests.cpp Source/Pathfinder/PathfinderAStar.cpp)
add_executable(test_stateMachine tests/stateMachineTests.cpp)

add_executable(test_generator tests/generatorTest.cpp
    Source/Mobs/Mob.cpp

    Source/Pathfinder/PathfinderAStar.cpp

    Source/WorldSystem/RandomGeneratorWorldBuilder.cpp
    Source/WorldSystem/BaseEntity.cpp
    Source/WorldSystem/DamageIndicatorsSystems.cpp
    Source/WorldSystem/World.cpp

    Source/ItemsSystem/AttackHandler.cpp
    Source/ItemsSystem/Attacks.cpp
    Source/ItemsSystem/BaseItem.cpp
    Source/ItemsSystem/Items.cpp
    Source/ItemsSystem/Weapon.cpp

    Source/Stats/StatsContainer.cpp
    Source/Stats/Modificators/StatWithModificators.cpp

    Source/Utils/Common.cpp
    Source/Utils/TMXGenerator.cpp

    Source/Player/Player.cpp
    Source/Player/PlayerInput.cpp
    )


target_link_libraries(test_base  PRIVATE  GTest::gtest_main  cocos2d)
target_link_libraries(test_pathfinder  PUBLIC  GTest::gtest_main  cocos2d)
target_link_libraries(test_stateMachine  PRIVATE  GTest::gtest_main  cocos2d)
target_link_libraries(test_generator  PRIVATE  GTest::gtest_main  cocos2d)

if(WINDOWS)
    cocos_copy_target_dll(test_pathfinder)
    cocos_copy_target_dll(test_stateMachine)
    cocos_copy_target_dll(test_generator)
endif()

