#include <ros/ros.h>
#include <avt_vimba_camera/mono_camera.h>
#include <avt_vimba_camera/status_camera.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "mono_camera_node");
  ros::NodeHandle nh;
  ros::NodeHandle nhp("~");
  avt_vimba_camera::StatusCamera hc;
  ros::Publisher status_pub = nh.advertise<cav_msgs::DriverStatus>("driver_discovery", 1);
  ros::Subscriber alert_sub = nh.subscribe<cav_msgs::SystemAlert>("system_alert",10,&avt_vimba_camera::StatusCamera::alertCallback, &hc);
  hc.pre_camera(status_pub);
  avt_vimba_camera::MonoCamera mc(nh,nhp);
  hc.post_camera();

  ros::Timer status_timer = nh.createTimer(
            ros::Duration(ros::Rate(1.0)),
            [&mc,&hc,&status_pub](const auto&) { 
              mc.updateCameraStatus();
              hc.status_cam=mc.cam_status;
              hc.publish_status();
              status_pub.publish(hc.status_);
            });

  ros::spin();
  return 0;
}


