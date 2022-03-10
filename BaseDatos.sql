DROP DATABASE IF EXISTS BaseDatos;
CREATE DATABASE BaseDatos;
USE Basedatos;

CREATE TABLE Jugador (
	id INT,
	username VARCHAR(20) NOT NULL,
	password VARCHAR(20) NOT NULL
)ENGINE=InnoDB;

CREATE TABLE Partida (
	id INT,
	fecha VARCHAR(20),
	hora VARCHAR(20),
	duracion INT,
	ganador VARCHAR(20)
)ENGINE=InnoDB;

CREATE TABLE Relacion (
	idJ INT,
	idP INT,
	cantidad INT,
	victorias INT,
FOREIGN KEY (idJ) REFERENCES Jugador(id),
FOREIGN KEY (idP) REFERENCES Partida(id)
)ENGINE=InnoDB;

INSERT INTO Jugador VALUES (1,'Tania','soytania');
INSERT INTO Jugador VALUES (2,'Andrei','andreipipo');
INSERT INTO Jugador VALUES (3,'Toni','tonibote');
INSERT INTO Jugador VALUES (4,'Miguel','soyelprofe');
INSERT INTO Jugador VALUES (5,'Erik','erikfm');

INSERT INTO Partida VALUES (1,'08/03/2022','12:15',3600,'Miguel');
INSERT INTO Partida VALUES (2,'10/03/2022','10:00',3200,'Erik');
INSERT INTO Partida VALUES (3,'12/04/2022','22:30',3100,'Erik');
INSERT INTO Partida VALUES (4,'12/04/2022','23:00',3600,'Toni');

INSERT INTO Relacion VALUES (1,1,3,0);
INSERT INTO Relacion VALUES (5,2,4,1);
INSERT INTO Relacion VALUES (5,3,5,1);
INSERT INTO Relacion VALUES (4,1,5,1);
INSERT INTO Relacion VALUES (3,4,4,1);
INSERT INTO Relacion VALUES (4,4,5,0);


