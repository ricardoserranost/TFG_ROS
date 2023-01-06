// Datos a enviar al crab_maestro_controller
// Ricardo Serrano Santa Teresa

#ifndef DATA_TO_SEND
#define DATA_TO_SEND

#include <ros/ros.h>

#include <crab_msgs/LegsJointsState.h>
#include <math.h>

//#include "PolstroSerialInterface.h"


class dataToSend {
	public:
	dataToSend();
	bool init();
	void send();
	private:

	crab_msgs::LegsJointsState legs;
	// LegsJointsState tiene	: crab_msgs/LegJointsState[6] joints_state
	// 0: lf
	// 1: rm
	// 2: rb
	// 3: rf
	// 4: lm
	// 5: lb
	// LegJointsState tiene		: float64[3] joint ; float64[3] velocity
	// 0 coxa, 1 femur, 2 tibia
	
	ros::NodeHandle node;	//("nodo_prueba")
	std::string port_name;
	//Polstro::SerialInterface* maestro;
	ros::Publisher joints_pub;	//Lo llamo con el mismo nombre que tenia para no cambiar la lectura
	};



#endif /*DATA_TO_SEND*/
