auv-simulator
=============

Simulates the working environment for Kraken-our Autonomous Underwater Vehicle.

To get it running:

1. Move to ROS Workspace: ````$ roscd ````

2. Clone the repository from github: ````$ git clone https://github.com/ankeshanand/auv-simulator.git ````

3. Install UWSim: ````$ apt-get install ros-groovy-uwsim````

4. Be sure roscore is running: ````$ roscore &```` (Runs roscore in background, you can continue in the same terminal session)

5. Run the simulator: ````$ rosrun uwsim uwsim --configfile auv-simulator/UWSim/kraken/scenes/UWSim_scene.xml````

