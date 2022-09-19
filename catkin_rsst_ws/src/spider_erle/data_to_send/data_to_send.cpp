// Datos a enviar al crab_maestro_controller
// Ricardo Serrano Santa Teresa

#include "data_to_send.hpp"
#include <fstream>

using namespace std;

//Para coger los valores desde un "genoma.txt":
void leerValores(float f[6][3], float d[6][3], float a[6][3], float p[6][3]){
	string path = "genoma_prueba.txt";
	ifstream archivo(path.c_str());
	string linea;
	while(getline(archivo, linea)){
		
	}
	
}


//Funciones de dataToSend:

dataToSend::dataToSend(){
	//maestro = Polstro::SerialInterface::createSerialInterface(port_name, 115200);
}

bool dataToSend::init(){
	//El nodo debe ser inicializado, ya que simplemente se ha declarado en la clase
	int i, j;
	
	joints_pub = node.advertise<crab_msgs::LegsJointsState>("joints_to_controller", 1);
	
	
	for(i=0; i<6; i++){
		for(j=0; j<3; j++){
			legs.joints_state[i].joint[j] = 0;		
		}	
	}

	return true; //Por ahora siempre devuelve true
}

void dataToSend::send(){
	int w = 2;
	int i, j;
	
	/*Parametros de cada articulacion*/
	float frecuencias[6][3], desfases[6][3], amplitudes[6][3], posCentrales[6][3];
	
	leerValores(frecuencias, desfases, amplitudes, posCentrales);
	
	/*-------------------------------*/
	
	for(i=0; i<6; i++){
		for(j=0; j<3; j++){
			legs.joints_state[i].velocity[j] = 3;		
		}	
	}

	while(node.ok()){
		
		//Para que siga una funci{on de seno: (el movimiento completo es de la amplitud que pones)	
		
		
		for(i=0; i<6; i++){
			for(j=0; j<3; j++){
				legs.joints_state[i].joint[j] = (3.1416/4)*sin(w*ros::Time::now().toSec());		
			}	
		}
		

		//legs.joints_state[0].joint[] = sin(ros::Time::now().toSec());
		joints_pub.publish(legs);
		
		ros::spinOnce();
		ros::Rate(25).sleep();
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "data_to_send");	//La string es el nombre que tiene el nodo
    	dataToSend d;
	if(d.init() < 0){
		ROS_ERROR("No se ha inicializado el nodo dataSend correctamente");
		return -1;	
	}

	d.send();
   	ros::spin();
	
	return 0;
}

