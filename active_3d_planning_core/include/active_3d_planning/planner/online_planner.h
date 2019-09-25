#ifndef ACTIVE_3D_PLANNING_CORE_PLANNER_ONLINE_PLANNER_H
#define ACTIVE_3D_PLANNING_CORE_PLANNER_ONLINE_PLANNER_H

#include "active_3d_planning/data/trajectory_segment.h"
#include "active_3d_planning/data/visualization_markers.h"
#include "active_3d_planning/data/system_constraints.h"
#include "active_3d_planning/module/module.h"
#include "active_3d_planning/module/back_tracker.h"
#include "active_3d_planning/module/module_factory.h"
#include "active_3d_planning/module/trajectory_evaluator.h"
#include "active_3d_planning/module/trajectory_generator.h"
#include "active_3d_planning/planner/planner_I.h"
#include "active_3d_planning/map/map.h"
#include "active_3d_planning/tools/defaults.h"

#include <ctime>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace active_3d_planning {

    class OnlinePlanner : public PlannerI, public ModuleBase {
    public:
        OnlinePlanner(ModuleFactory *factory, Module::ParamMap *param_map);

        virtual ~OnlinePlanner();

        // Start/Run the planning loop
        virtual void planningLoop();

        // Accessors
        const Eigen::Vector3d &getCurrentPosition() const override {
            return current_position_;
        }

        const Eigen::Quaterniond &getCurrentOrientation() const override {
            return current_orientation_;
        }

        virtual BackTracker &getBackTracker() override {
            return *back_tracker_;
        }

        virtual TrajectoryGenerator &getTrajectoryGenerator() override {
            return *trajectory_generator_;
        }

        virtual TrajectoryEvaluator &getTrajectoryEvaluator() override {
            return *trajectory_evaluator_;
        }

        virtual ModuleFactory &getFactory() override {
            return *factory_;
        }

        virtual Map &getMap() override {
            return *map_;
        }

        virtual SystemConstraints &getSystemConstraints() override {
            return *system_constraints_;
        }

        // logging and printing
        virtual void printInfo(const std::string &text) override;

        virtual void printWarning(const std::string &text) override;

        virtual void printError(const std::string &text) override;

    protected:
        // factory access
        ModuleFactory *factory_;

        // members
        std::unique_ptr<Map> map_;
        std::unique_ptr<TrajectoryGenerator> trajectory_generator_;
        std::unique_ptr<TrajectoryEvaluator> trajectory_evaluator_;
        std::unique_ptr<BackTracker> back_tracker_;
        std::unique_ptr<TrajectorySegment> current_segment_;    // root node of full trajectory tree
        std::unique_ptr<SystemConstraints> system_constraints_;

        // variables
        bool planning_;                     // whether to run the main loop
        bool running_;                      // whether the planner is alive
        Eigen::Vector3d current_position_;  // Current pose of the robot
        Eigen::Quaterniond current_orientation_;
        Eigen::Vector3d target_position_;   // current movement goal
        double target_yaw_;
        bool target_reached_; // whether the goal point was reached, update based on odom input
        int new_segments_; // keep track of min/max tries and segments
        int new_segment_tries_;
        bool min_new_value_reached_;
        std::string logfile_;
        int vis_completed_count_;   // keep track to incrementally add segments

        // Info+performance bookkeeping
        std::clock_t info_timing_;    // Simulated/On-Board time for verbose and perf [s]
        int info_count_;              // num trajectories counter for verbose
        int info_killed_next_;        // number of segments killed during root change
        int info_killed_update_;      // number of segments killed during updating
        std::ofstream perf_log_file_;  // performance file
        std::vector<double> perf_log_data_; // select, expand, gain, cost, value [cpu seconds]
        std::clock_t perf_cpu_timer_; // total time counter

        // params
        bool p_verbose_;
        bool p_visualize_; // Publish visualization of completed path, current gain,
        // new candidates, ...
        bool p_log_performance_;     // Whether to write a performance log file
        int p_max_new_segments_; // After this count is reached no more segments are
        // expanded (0 to ignore)
        int p_min_new_segments_; // Until this count is reached the next segment is
        // not executed (0 to ignore)
        int p_min_new_tries_;    // Until no. expansion calls the next segment is not
        // executed (0 to ignore)
        int p_max_new_tries_; // After no. expansion calls the next segment is forced
        // (0 to ignore)
        double p_min_new_value_; // Until this value is found in the tree, expansion
        // continues (0 to ignore)
        int p_expand_batch_;     // run multiple segment expansions before rechecking
        // conditions
        bool p_visualize_gain_; // true: add a colored sphere according to the gain to
        // every segment
        bool p_highlight_executed_trajectory_;  // true: print executed trajectory in bold red

        // methods
        virtual void initializePlanning();

        virtual void loopIteration();

        virtual bool requestNextTrajectory();

        virtual void expandTrajectories();

        virtual void requestMovement(const EigenTrajectoryPointVector &trajectory) = 0;

        virtual void setupFromParamMap(Module::ParamMap *param_map) override;

        // visualization
        virtual void publishTrajectoryVisualization(const std::vector<TrajectorySegment *> &trajectories);

        virtual void publishCompletedTrajectoryVisualization(const TrajectorySegment &trajectories);

        virtual void publishEvalVisualization(const TrajectorySegment &trajectory);

        // helper functions
        // check whether min value is found
        bool checkMinNewValue(const std::unique_ptr<TrajectorySegment> &segment);

        // Recursively update the tree
        void updateGeneratorStep(TrajectorySegment *target);

        void updateEvaluatorStep(TrajectorySegment *target);

    };
} // namespace active_3d_planning

#endif //ACTIVE_3D_PLANNING_CORE_PLANNER_ONLINE_PLANNER_H
