define kinect ranger
(
  sensor
  (
    range_max 6.5
    fov 58.0
    samples 640
  )
  # generic model properties
  color "black"
  size [ 0.06 0.15 0.03 ]
)

define visible camera
(
    resolution [ 320 240 ]
    range [ 0.2 100.0 ]
    fov [ 70.0 40.0 ]
    pantilt [ 0.0 -90.0 ]
  # generic model properties
  update_interval 100
  color "black"
  size [ 0.06 0.15 0.03 ]
)

define turtlebot position
(
  pose [ 0.0 0.0 0.0 0.0 ]
  localization "gps"

  #odom_error [0.03 0.03 999999 999999 999999 0.02]

  #size [ 0.1 0.1 0.20 ]
  size [ 0.2552 0.2552 0.40 ]
  origin [ 0.0 0.0 0.0 0.0 ]
  gui_nose 1
  #drive "diff"
  drive "omni"
  color "gray"
  
#  kinect(pose [ -0.1 0.0 -0.11 0.0 ])
  visible(pose [ -0.1 0.0 0.11 0.0 ])
  velocity_bounds [-10 10 -10 10 -1 1 -90 90]
)

define swarm position
(
  pose [ 0.0 0.0 0.0 0.0 ]
  localization "gps"

  size [ 0.2552 0.2552 0.40 ]
  origin [ 0.0 0.0 0.0 0.0 ]
  gui_nose 1
  drive "omni"
  color "gray"
  
  
)
