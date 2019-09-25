#ifndef ACTIVE_3D_PLANNING_ROS_MODULE_FACTORY_ROS_H
#define ACTIVE_3D_PLANNING_ROS_MODULE_FACTORY_ROS_H

#include "active_3d_planning/module/module_factory.h"

#include <string>


namespace active_3d_planning {
    namespace ros {

        // Concrete factory for ros param server
        class ModuleFactoryROS : public ModuleFactory {
            friend ModuleFactory;

        public:
            ~ModuleFactoryROS() = default;

            ModuleFactoryROS();

        protected:
            // Implement virtual methods
            bool getParamMapAndType(Module::ParamMap *map, std::string *type,
                                    std::string args);

            void printVerbose(const Module::ParamMap &map);

            void printError(const std::string &message);
        };

    } // namespace ros
} // namespace active_3d_planning

#endif // ACTIVE_3D_PLANNING_ROS_MODULE_FACTORY_ROS_H
