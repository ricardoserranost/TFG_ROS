// Datos a enviar al crab_maestro_controller
// Ricardo Serrano Santa Teresa

#include "data_to_send.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#define PI 3.141593

using namespace std;

//Para coger los valores desde un "genoma.csv":
void leerValores(float f[6][3], float d[6][3], float a[6][3], float p[6][3]){
	ifstream archivo("/home/ricardo/catkin_rsst_TFG/src/data_to_send/src/genoma.csv");
	if(!archivo.is_open()) cout << "Error al abrir archivo" << endl;
	string linea, dato;
	int pata = 0, artic = 0;
	while(getline(archivo, linea)){
		stringstream str(linea);
		while(getline(str, dato, ';')){
			f[pata][artic] = stof(dato);
			artic++;
		}
		artic = 0;
		
		getline(archivo, linea);
		stringstream str2(linea);
		while(getline(str2, dato, ';')){
			d[pata][artic] = stof(dato);
			artic++;
		}
		artic = 0;

		getline(archivo, linea);
		stringstream str3(linea);
		while(getline(str3, dato, ';')){
			a[pata][artic] = stof(dato);
			artic++;
		}
		artic = 0;

		getline(archivo, linea);
		stringstream str4(linea);
		while(getline(str4, dato, ';')){
			p[pata][artic] = stof(dato);
			artic++;
		}
		artic = 0;
		
		pata++;
		
	}
	
	archivo.close();
}

//Para que el orden de las articulaciones sea el mismo que el de la simulación
void ajustarValores(float f[6][3], float d[6][3], float a[6][3], float p[6][3]){
	float aux;
	int i;
	// Cambia la pata posición 0 por la de posición 3 (lf por rf)
	for(i = 0; i < 3; i++){
		aux = f[0][i];
		f[0][i] = f[3][i];
		f[3][i] = aux;
		aux = d[0][i];
		d[0][i] = d[3][i];
		d[3][i] = aux;
		aux = a[0][i];
		a[0][i] = a[3][i];
		a[3][i] = aux;
		aux = p[0][i];
		p[0][i] = p[3][i];
		p[3][i] = aux;
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
	int w = 1;	//Uso esto para cambiar la frecuencia que envío, por si quiero ralentizarlo o acelerarlo
	int i, j;
	
	/*Parametros de cada articulacion*/
	float fr[6][3], de[6][3], am[6][3], po[6][3];

	//Matriz de sentidos, 1 si se mantiene como en el robot
	int sentidos[6][3] = {{1,-1,1},{1,1,-1},{1,1,-1},{1,-1,1},{1,1,-1},{1,1,-1}};
	
	leerValores(fr, de, am, po);
	ajustarValores(fr, de, am, po);

	
	/*-------------------------------*/
	
	for(i=0; i<6; i++){
		for(j=0; j<3; j++){
			legs.joints_state[i].velocity[j] = 0.0;		
		}	
	}

	while(node.ok()){
		
		for(i=0; i<6; i++){
			for(j=0; j<3; j++){
				//La linea queda un poco larga
				legs.joints_state[i].joint[j] = (po[i][j] + am[i][j]*sin(w*ros::Time::now().toSec()*2*PI*fr[i][j]+de[i][j]))*(4*PI)/360;
				// Multiplico por 4*PI porque el robot parece que toma los ángulos divididos por la mitad
				
				//legs.joints_state[i].velocity[j] = (po[i][j] + am[i][j]*cos(w*ros::Time::now().toSec()*2*PI*fr[i][j]+de[i][j]))*(2*PI)/360;
				
				//Los sentidos no coinciden con los del robot, por lo que:
				dataToSend::legs.joints_state[i].joint[j] *= sentidos[i][j];
			}	
		}
		
		//Para que siga una función de seno: (el movimiento completo es de la amplitud que pones)	
		//legs.joints_state[i].joint[j] = (PI/4)*sin(w*ros::Time::now().toSec());
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

