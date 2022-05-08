
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mysql.h>
#include <pthread.h>

int contservicios;
MYSQL *conn;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
	char nombre[100];
	int socket;
} JugadorConectado;
typedef struct{
	JugadorConectado Lista[100];
	int num;
}ListaJugadoresConectados;

ListaJugadoresConectados miLista;


//Funcion Login de un jugador que quiere hacer una partida

void Login(char contra[100], char nombre[100], char respuesta[100],int socket){
	
	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[100];
	
	strcpy (consulta,"Select jugador.ID FROM (jugador) WHERE jugador.Username= '"); 
	strcat (consulta, nombre);
	strcat (consulta,"'");
	strcat (consulta," AND jugador.Pass='");
	strcat (consulta, contra);
	strcat (consulta,"'");
	// hacemos la consulta 
	err=mysql_query (conn, consulta); 
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta 
	resultado = mysql_store_result (conn); 
	row = mysql_fetch_row (resultado);
	if (row == NULL)
		sprintf(respuesta,"NO");
	else{
		sprintf(respuesta,"%s",row[0]);
	}
}

	
//Funcion para Resgistrar un nuevo usuario que no existe

void Registrar(char contra[100], char nombre[100], char respuesta[100])
{
		
		int err;
		MYSQL_RES *resultado;
		MYSQL_ROW row;
		char consulta[100];
		
		strcpy (consulta,"Select jugador.Username,jugador.Pass FROM (jugador) WHERE jugador.Username= '"); 
		strcat (consulta, nombre);
		strcat (consulta,"'");
		
		
		// Hacemos la consulta 
		err = mysql_query (conn, consulta); 
		if (err!=0) {
			printf ("Error al consultar datos de la base %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		
		//Recogemos el resultado de la consulta 
		resultado = mysql_store_result (conn); 
		row = mysql_fetch_row (resultado);
		if (row == NULL){
			char select[100];
			strcpy(select,"Select MAX(jugador.ID) FROM (jugador)");
			err=mysql_query (conn, select); 
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			//recogemos el resultado de la consulta 
			resultado = mysql_store_result (conn); 
			row = mysql_fetch_row (resultado);
			int ID_jugador = atoi(row[0])+1;
			char insert[100];
			strcpy (insert,"Insert INTO jugador(ID,Username,Pass) values(");
			sprintf(insert,"%s%d",insert,ID_jugador);
			strcat (insert,",'");
			strcat (insert, nombre);
			strcat (insert,"','");
			strcat (insert, contra);
			strcat (insert,"')");
			pthread_mutex_lock(&mutex);
			err=mysql_query (conn, insert); 
			if (err!=0) {
				printf ("Error al insertar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
				sprintf(respuesta,"NO");
			}
			else 
				sprintf(respuesta,"SI");
			pthread_mutex_unlock(&mutex);
		}
		else{
			sprintf(respuesta,"NO");
		}
	}

void PonerJugadorEnListaConectados(char nombre[100], int socket)
{
	strcpy(miLista.Lista[miLista.num].nombre,nombre);
	miLista.Lista[miLista.num].socket=socket;
	miLista.num++;
}
//Funcion para eliminar un jugador que estaba en la lista de conectados
void ElimimarJugadorQueEstabaListaConectados(char* nombre)
{
	int encontrado = 0;
	int i=0;
	while((encontrado==0) && (i < miLista.num))
	{
		if(strcmp(nombre,miLista.Lista[i].nombre)==0)
		{
			encontrado==1;
			for(int j = 0;j<miLista.num;j++)
			{
				strcpy(miLista.Lista[j].nombre,miLista.Lista[j+1].nombre);
				miLista.Lista[j].socket = miLista.Lista[j+1].socket;
			}
			miLista.num--;
		}
		i++;
	}
}

void NumeroDeConectadosYUsernames(char respuesta[512])
{
	pthread_mutex_lock(&mutex);
	sprintf(respuesta,"%d,",miLista.num);
	
	for (int i = 0; i<miLista.num;i++)
	{
		sprintf(respuesta,"%s%s,",respuesta,miLista.Lista[i].nombre);
	}
	respuesta[strlen(respuesta)-1] = '\0';
	pthread_mutex_unlock(&mutex);
}

void *AtenderCliente (void *socket)
{
	//Bucle para atención al cliente
	int terminar = 0;
	int sock_conn, ret;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int err;
	contservicios=0;
	char consulta[100];
	int *s;
	s = (int *) socket;
	sock_conn = *s;
	
	char peticion[512];
	char respuesta[512];
	
	//Entramos en un bucle para atender todas la peticiones que quiere este cliente
	//Hasta que se desconecte
	
	while (terminar == 0){
		strcpy(respuesta,"");
		
		printf ("Escuchando\n");
		// Ahora recibimos la petici?n
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		
		// Tenemos que anadirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		printf ("Peticion: %s\n",peticion);
		
		// vamos a ver que quieren
		char *p = strtok( peticion, "/");
		int codigo =  atoi (p);
		// Ya tenemos el codigo de la peticion
		char nombre[20];
		char fecha[20];
		char contrasena[100];
		char id_j[100];
		char consulta [200];
		int max;
	    p = strtok(NULL,"/");
			
	    if((p!=NULL) && (codigo!=20)){
				strcpy (nombre, p);
				// Ya tenemos el nombre
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
		}
		
	
		if (codigo == 0){//peticion de desconexion
			if(nombre!=NULL){
				pthread_mutex_lock(&mutex);
				ElimimarJugadorQueEstabaListaConectados(nombre);
				pthread_mutex_unlock(&mutex);
			}
			terminar=1;
		}
		
		else if (codigo == 7 ){ //LOGIN
			p = strtok(NULL, "/");
			strcpy(contrasena,p);
			Login(contrasena,nombre,respuesta,*s);
			pthread_mutex_lock(&mutex);
			PonerJugadorEnListaConectados(nombre,*s);
			pthread_mutex_unlock(&mutex);
		}
		else if (codigo == 8 ){ //Registrar
			char consulta[150];
			p=strtok(NULL,"/");
			strcpy(contrasena,p);
			Registrar(contrasena,nombre,respuesta);
		}
		
		else if (codigo == 9 ){ //Dime nombre ganador y duracion partida jugada tal dia
			
			//Construimos la consulta SQL
			strcpy(consulta,"SELECT jugador.Username, partida.Duracion FROM jugador, partida, relacion WHERE partida.Fecha = '");
			strcat (consulta, fecha);
			strcat (consulta,"'");
			strcat (consulta, " AND partida.ID = relacion.ID_P AND relacion.ID_J = jugador.ID");
			
			//Hacemos la consulta
			err = mysql_query (conn, consulta);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			
			//Recogemos el resultado de la consulta 
			resultado = mysql_store_result (conn); 
			row = mysql_fetch_row (resultado);
			
			if (row == NULL){
				printf ("No se han obtenido datos en la consulta\n");
				sprintf(respuesta,"mal");
			}
			else
			{
				while (row != NULL) 
				{
					sprintf(respuesta,"%s%s",row[0],row[1]);
					row = mysql_fetch_row(resultado);
				}
			}
		}
		
		
		else if (codigo == 10 ){ ////Dame las partidas ganadas de tal username
			
			//Construimos la consulta SQL
			strcpy(consulta,"SELECT relacion.Victorias FROM jugador, partida, relacion WHERE jugador.Username = '");
			strcat (consulta, nombre);
			strcat (consulta,"'");
			strcat (consulta, " AND partida.ID = relacion.ID_P AND relacion.ID_J = jugador.ID");
			
			//Hacemos la consulta
			err = mysql_query (conn, consulta);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			
			//Recogemos el resultado de la consulta 
			resultado = mysql_store_result (conn); 
			row = mysql_fetch_row (resultado);
			
			if (row == NULL){
				printf ("No se han obtenido datos en la consulta\n");
				sprintf(respuesta,"mal");
			}
			else{
				int victorias=0;
				int n=0;
				while(row!=NULL){
					if (strcmp(row[0],nombre)==0){
						victorias = 1;
						n++;
					}
					else{
						victorias = 0;
						n=0;
					}
					if((victorias==1)&&(n>max)){
						max = n;
					}
					sprintf(respuesta,"%s",row[0]);
					row = mysql_fetch_row (resultado);
				}
			}
		}
		else if (codigo == 11 ){ //Dame cantidad de partidas jugadas de tal username
			
			//Construimos la consulta SQL
			sprintf(consulta,"SELECT relacion.Cantidad FROM jugador, partida, relacion WHERE partida.ID = relacion.ID_P AND relacion.ID_J = jugador.ID AND jugador.Username='%s'",nombre);
			
			//Hacemos la consulta
			err=mysql_query (conn, consulta); 
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			
			//Recogemos el resultado de la consulta 
			resultado = mysql_store_result (conn); 
			row = mysql_fetch_row (resultado);
			if((row == NULL)||(row[0] == NULL))
			{
				printf("No se han obtenido datos en la consulta/n");
				sprintf(respuesta,"mal");
			}
			else
			{
				while (row != NULL) 
				{
					//int c = atoi(row[0]);
					sprintf(respuesta,"%s",row[0]);
					row = mysql_fetch_row(resultado);
				}
			}
		}
			
		else if (codigo == 12 ){ //Funcion dame ID de tal username
			
			//Construimos la consulta SQL
			sprintf(consulta,"SELECT jugador.ID FROM jugador WHERE jugador.Username = '%s'", nombre);
			
			//Hacemos la consulta
			err=mysql_query (conn, consulta); 
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			
			//Recogemos el resultado de la consulta
			resultado = mysql_store_result (conn);
			row = mysql_fetch_row (resultado);
			if (row == NULL){
				printf("No se han obtenido datos en la consulta/n");
				sprintf(respuesta,"mal");
			}
			else{
				while (row != NULL) 
				{
					sprintf(respuesta,"%s",row[0]);
					row = mysql_fetch_row(resultado);
				}
			}
		}
		else if (codigo == 13) //Dime los usuarios que estan conectados
		{
			NumeroDeConectadosYUsernames(respuesta);
		}
		else if (codigo == 20)  //Dime cuantos servicios llevo realizados
		{
			sprintf(respuesta,"%d",contservicios);
		}
		if (codigo != 0)
		{
			printf ("Respuesta: %s\n", respuesta);
			// Enviamos respuesta
			write (sock_conn,respuesta, strlen(respuesta));
		}
		if (codigo==9 || codigo==10|| codigo==11 ||codigo==12)
		{
			pthread_mutex_lock(&mutex); //No me interrumpas ahora
			contservicios=contservicios+1;
			pthread_mutex_unlock(&mutex); //Ya puedes interrumpirme
		}
	}
	// Se acabo el servicio para este cliente
	close(sock_conn); 
}
		
int main(int argc, char *argv[]) {
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	miLista.num=0;
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la maquina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9070); //9070 en lh o 50000 en shiva
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind\n");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen\n");
	
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	//Creamos una conexion al servidor MYSQL 
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn,"localhost", "root", "mysql", "M1_BBDD",0, NULL, 0);
	// conn = mysql_real_connect (conn,"shiva2.upc.es", "root", "mysql", "M1_BBDD",0, NULL, 0);
	if (conn==NULL) 
	{
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	int i=0;
	int socket[100];
	pthread_t thread[100];
	
	//Bucle infinito
	for(;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		socket[i]=sock_conn;
		
		//La variable sock_conn es el socket que utilizamos para este cliente
		pthread_create(&thread[i], NULL, AtenderCliente, &socket[i]);
		i = i+1;		
	}
}

