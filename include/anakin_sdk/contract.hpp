#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
#include <unique_identifier_msgs/msg/uuid.hpp>
namespace anakin {
inline constexpr uint32_t schema_version = 3;
inline constexpr const char* schema_id = "anakin.g1.canonical29.v3";
inline constexpr const char* legacy_gain_profile="g1_asap_sim_v1";
inline constexpr const char* merged_gain_profile="g1_asap_merged_sim_v1";
inline bool known_gain_profile(const std::string& profile) {
 return profile==legacy_gain_profile || profile==merged_gain_profile;
}
inline constexpr uint8_t mechanical_position=0, pd_reference=1;
inline constexpr const char* strict_execution_profile="g1_position_strict_sim_v1";
inline constexpr const char* project_execution_profile="g1_pd_reference_project_sim_v1";
inline constexpr const char* equivalent_execution_profile="g1_pd_reference_equivalent_sim_v1";
inline constexpr const char* sdk_pd_execution_profile="g1_sdk_pd_v1";
enum class ExecutionEncoding { StrictPosition, ProjectReference, EquivalentReference, SdkPdReference };
inline ExecutionEncoding execution_encoding(const std::string& profile,uint8_t semantics) {
 if(profile==strict_execution_profile && semantics==mechanical_position)return ExecutionEncoding::StrictPosition;
 if(profile==project_execution_profile && semantics==pd_reference)return ExecutionEncoding::ProjectReference;
 if(profile==equivalent_execution_profile && semantics==pd_reference)return ExecutionEncoding::EquivalentReference;
 if(profile==sdk_pd_execution_profile && semantics==pd_reference)return ExecutionEncoding::SdkPdReference;
 throw std::invalid_argument("Unknown or mismatched execution profile/command semantics");
}
inline uint8_t profile_semantics(const std::string& profile) {
 if(profile==strict_execution_profile)return mechanical_position;
 if(profile==project_execution_profile || profile==equivalent_execution_profile || profile==sdk_pd_execution_profile)return pd_reference;
 throw std::invalid_argument("Unknown execution profile");
}
inline bool known_execution_contract(const std::string& profile,uint8_t semantics,const std::string& gain) {
 try {return known_gain_profile(gain) &&
  (execution_encoding(profile,semantics)==ExecutionEncoding::StrictPosition ||
   execution_encoding(profile,semantics)==ExecutionEncoding::SdkPdReference || gain==merged_gain_profile);}
 catch(const std::invalid_argument&) {return false;}
}
using Uuid = unique_identifier_msgs::msg::UUID;
inline bool zero(const Uuid& id) { return std::all_of(id.uuid.begin(),id.uuid.end(),[](auto c){ return c==0; }); }
inline Uuid new_uuid() { Uuid id; std::random_device r; for(auto& c:id.uuid)c=static_cast<uint8_t>(r()); id.uuid[6]=(id.uuid[6]&15)|64; id.uuid[8]=(id.uuid[8]&63)|128; return id; }
inline std::string uuid_string(const Uuid& id) { static constexpr char hex[]="0123456789abcdef"; std::string out; for(auto c:id.uuid){out+=hex[c>>4];out+=hex[c&15];} return out; }
inline const std::vector<std::string>& joint_names() {
 static const std::vector<std::string> names={
 "left_hip_pitch_joint","left_hip_roll_joint","left_hip_yaw_joint","left_knee_joint","left_ankle_pitch_joint","left_ankle_roll_joint",
 "right_hip_pitch_joint","right_hip_roll_joint","right_hip_yaw_joint","right_knee_joint","right_ankle_pitch_joint","right_ankle_roll_joint",
 "waist_yaw_joint","waist_roll_joint","waist_pitch_joint",
 "left_shoulder_pitch_joint","left_shoulder_roll_joint","left_shoulder_yaw_joint","left_elbow_joint","left_wrist_roll_joint","left_wrist_pitch_joint","left_wrist_yaw_joint",
 "right_shoulder_pitch_joint","right_shoulder_roll_joint","right_shoulder_yaw_joint","right_elbow_joint","right_wrist_roll_joint","right_wrist_pitch_joint","right_wrist_yaw_joint"}; return names;
}
// A strict named map also handles asymmetric permutations and 23/12-joint subsets.
inline std::vector<size_t> named_indices(const std::vector<std::string>& names) {
 std::vector<size_t> out; for(const auto& name:names) {
  auto p=std::find(joint_names().begin(),joint_names().end(),name);
  if(p==joint_names().end()) throw std::invalid_argument("Unknown joint: "+name);
  auto index=static_cast<size_t>(p-joint_names().begin());
  if(std::find(out.begin(),out.end(),index)!=out.end())throw std::invalid_argument("Duplicate joint: "+name);
  out.push_back(index);
 } return out;
}
}
