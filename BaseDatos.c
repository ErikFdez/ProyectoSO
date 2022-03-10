//Incluir esta libreria para poder hacer las llamadas en shiva2.upc.es
//#include <my_global.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

int main(int argc, char *argv[]) {
	
	MYSQL *conn;
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
	
	//inicializar la conexion, entrando nuestras claves de acceso y
	//el nombre de la base de datos a la que queremos acceder 
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "BaseDatos",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	char nombre[20];
	// Pregunto los nombre de los dos jugadores
	printf ("Dame el nombre de un jugador\n"); 
	scanf ("%s", nombre);
	
	char consulta [80];
	strcpy (consulta,"SELECT Relacion.victorias FROM Jugador,Partida,Relacion WHERE Jugador.username = '");
	strcat (consulta, nombre);
	strcat (consulta,"'AND Jugador.id = Relacion.idJ AND Relacion.idP = Partida.id");
	
	err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//recogemos el resultado de la consulta. El resultado de la
	//consulta se devuelve en una variable del tipo puntero a
	//MYSQL_RES tal y como hemos declarado anteriormente.
	//Se trata de una tabla virtual en memoria que es la copia
	//de la tabla real en disco.
	resultado = mysql_store_result (conn);
	// El resultado es una estructura matricial en memoria
	// en la que cada fila contiene los datos de una persona.
	
	// Ahora obtenemos la primera fila que se almacena en una
	// variable de tipo MYSQL_ROW
	row = mysql_fetch_row (resultado);
	
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else
		while (row !=NULL) {
			// la columna 3 contiene las victorias
			printf ("%s\n", row[3]);
			// obtenemos la siguiente fila
			row = mysql_fetch_row (resultado);
	}
		
		mysql_close (conn);
		exit(0);
}

