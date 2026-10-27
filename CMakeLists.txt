cmake_minimum_required(VERSION 3.16)
project(anakin_sdk VERSION 0.1.0 LANGUAGES C CXX)
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
foreach(dep builtin_interfaces std_msgs unique_identifier_msgs sensor_msgs geometry_msgs nav_msgs trajectory_msgs diagnostic_msgs)
  find_package(${dep} REQUIRED)
endforeach()
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RobotStateFrame.msg" "msg/ControlTarget.msg" "msg/InferenceStatus.msg"
  "srv/SwitchInferenceMode.srv"
  DEPENDENCIES builtin_interfaces std_msgs unique_identifier_msgs sensor_msgs geometry_msgs nav_msgs trajectory_msgs diagnostic_msgs)
execute_process(COMMAND git rev-parse HEAD WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE ANAKIN_SDK_REVISION OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
configure_file(cmake/revision.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/revision.cmake @ONLY)
install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME})
install(DIRECTORY docs/ DESTINATION share/${PROJECT_NAME}/docs)
ament_export_include_directories(include/${PROJECT_NAME})
ament_export_dependencies(rosidl_default_runtime builtin_interfaces std_msgs unique_identifier_msgs sensor_msgs geometry_msgs nav_msgs trajectory_msgs diagnostic_msgs)
ament_package(CONFIG_EXTRAS ${CMAKE_CURRENT_BINARY_DIR}/revision.cmake)
