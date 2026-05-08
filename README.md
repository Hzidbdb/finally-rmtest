基于ros1

首先在demo01下分别打开三个端口并执行source ./devel/setup.bash

第一步启动gazebo仿真：运行roslaunch urdf02 gazebo.launch

第二步启动rviz可视化：运行roslaunch nav_demo nav07.launch

接着即可使用rviz的2D navgoal进行导航

第三步保存地图：运行roslaunch nav_demo save_map.launch

第四步使用保存的地图：运行roslaunch nav_demo nav06.launch

DLC内容：使用send_goal节点发布坐标使机器人自动去goal

第四步之后：在demo01下打开一个端口并执行source ./devel/setup.bash

接着运行：rosrun nav_demo send_goal即可

踩坑点：变量名要规范，版本选择要正确

