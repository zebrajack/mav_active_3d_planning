#ifndef ACTIVE_3D_PLANNING_SEGMENT_SELECTOR_RANDOM_RESTRICTED_H
#define ACTIVE_3D_PLANNING_SEGMENT_SELECTOR_RANDOM_RESTRICTED_H

#include "active_3d_planning/module/trajectory_generator.h"

namespace active_3d_planning {
    namespace segment_selector {

        // Select segments at random, weighted with their value
        class RandomRestricted : public SegmentSelector {
        public:
            explicit RandomRestricted(PlannerI &planner);

            // override virtual functions
            bool selectSegment(TrajectorySegment **result,
                               TrajectorySegment *root) override;

            void setupFromParamMap(Module::ParamMap *param_map) override;

            bool checkParamsValid(std::string *error_message) override;

        protected:
            static ModuleFactoryRegistry::Registration<RandomRestricted> registration;

            // params
            int maxdepth_; // maximal look ahead. ==1 will only consider immediate children
        };

    } // namespace segment_selector
} // namespace active_3d_planning
#endif // ACTIVE_3D_PLANNING_SEGMENT_SELECTOR_RANDOM_RESTRICTED_H
