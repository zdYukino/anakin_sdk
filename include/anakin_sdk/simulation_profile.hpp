#pragma once
#include <anakin_sdk/contract.hpp>
#include <array>
namespace anakin {
// Read-only simulation execution-profile metadata. Selection/encoding stays in env.
// Merged profiles reproduce Python merge_dof_cfgs(G1_29DoF, policy.action_dof).
struct SimulationGains { std::array<double,29> kp,kd; };
inline const SimulationGains& simulation_gains(const std::string& profile,const std::string& model) {
 if(model!="asap_loco" && model!="asap_cr7")throw std::invalid_argument("Unknown gain-profile model");
  static const SimulationGains legacy{{100.0, 100.0, 100.0, 200.0, 20.0, 20.0, 100.0, 100.0, 100.0, 200.0, 20.0, 20.0, 400.0, 400.0, 400.0, 90.0, 60.0, 20.0, 60.0, 4.0, 4.0, 4.0, 90.0, 60.0, 20.0, 60.0, 4.0, 4.0, 4.0}, {2.5, 2.5, 2.5, 5.0, 0.2, 0.1, 2.5, 2.5, 2.5, 5.0, 0.2, 0.1, 5.0, 5.0, 5.0, 2.0, 1.0, 0.4, 1.0, 0.2, 0.2, 0.2, 2.0, 1.0, 0.4, 1.0, 0.2, 0.2, 0.2}};
  static const SimulationGains loco{{100.0, 100.0, 100.0, 200.0, 20.0, 20.0, 100.0, 100.0, 100.0, 200.0, 20.0, 20.0, 200.0, 200.0, 200.0, 40.0, 40.0, 40.0, 40.0, 20.0, 20.0, 20.0, 40.0, 40.0, 40.0, 40.0, 20.0, 20.0, 20.0}, {2.5, 2.5, 2.5, 5.0, 0.2, 0.1, 2.5, 2.5, 2.5, 5.0, 0.2, 0.1, 6.0, 6.0, 6.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0}};
  static const SimulationGains mimic{{100.0, 100.0, 100.0, 200.0, 20.0, 20.0, 100.0, 100.0, 100.0, 200.0, 20.0, 20.0, 400.0, 400.0, 400.0, 90.0, 60.0, 20.0, 60.0, 20.0, 20.0, 20.0, 90.0, 60.0, 20.0, 60.0, 20.0, 20.0, 20.0}, {2.5, 2.5, 2.5, 5.0, 0.2, 0.1, 2.5, 2.5, 2.5, 5.0, 0.2, 0.1, 5.0, 5.0, 5.0, 2.0, 1.0, 0.4, 1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 0.4, 1.0, 2.0, 2.0, 2.0}};
 if(profile==legacy_gain_profile)return legacy;
 if(profile==merged_gain_profile)return model=="asap_loco"?loco:mimic;
 throw std::invalid_argument("Unknown simulation gain profile");
}
}  // namespace anakin
