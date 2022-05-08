DROP DATABASE IF EXISTS M1_BBDD;
CREATE DATABASE M1_BBDD;

USE M1_BBDD;

CREATE TABLE jugador (

    ID INT NOT NULL,
    Username VARCHAR(20) NOT NULL,
    Pass VARCHAR(20) NOT NULL,
    PRIMARY KEY (ID)

) ENGINE = InnoDB;

CREATE TABLE partida (

    ID INT NOT NULL,
    Fecha VARCHAR(20),
    Hora VARCHAR(20),
    Duracion VARCHAR(20),
    Ganador VARCHAR(20),
    PRIMARY KEY (ID)

) Engine = InnoDB;


CREATE TABLE relacion (

ID_J INT NOT NULL,
ID_P INT NOT NULL,
Cantidad INT,
Victorias INT,
FOREIGN KEY (ID_J) REFERENCES jugador(ID),
FOREIGN KEY (ID_P) REFERENCES partida(ID)

) ENGINE = InnoDB;



INSERT INTO jugador VALUES(1,'Erik','CasaPapel');
INSERT INTO jugador VALUES(2,'Andrei','BreakingBad');
INSERT INTO jugador VALUES(3,'Tania','KUWTK');
INSERT INTO jugador VALUES(4,'Toni','DARK');

INSERT INTO partida VALUES(1,'17-02-2022','12:00','1h','Toni');
INSERT INTO partida VALUES(3,'18-02-2022','12:00','3h','Erik');
INSERT INTO partida VALUES(6,'20-02-2022','18:00','2h','Toni');
INSERT INTO partida VALUES(9,'24-02-2022','17:00','3h 30min','Toni');
INSERT INTO partida VALUES(10,'26-02-2022','21:00','2h','Andrei');


INSERT INTO relacion VALUES(4,1,5,2);
INSERT INTO relacion VALUES(1,3,5,3);
INSERT INTO relacion VALUES(4,6,5,2);
INSERT INTO relacion VALUES(2,10,5,3);


/*DAME EL USERNAME DEL GANADOR Y LA DURACIÓN DE LA PARTIDA QUE SE JUGO EN UNA FECHA CONCRETA

SELECT jugador.Username, partida.Duracion FROM jugador, partida, relacion
WHERE  partida.Fecha = '17-02-2022'
AND    partida.ID = relacion.ID_P
AND    relacion.ID_J = jugador.ID;*/


/*DAME LAS PARTIDAS GANADAS DE TAL USERNAME

SELECT relacion.Victorias FROM jugador, partida, relacion
WHERE jugador.Username = 'Erik'
AND partida.ID = relacion.ID_P
AND relacion.ID_J = jugador.ID;*/


/*DAME CANTIDAD DE PARTIDAS JUGADAS DE TAL USERNAME

SELECT relacion.Cantidad FROM jugador, partida, relacion
WHERE jugador.Username = 'Erik'
AND partida.ID = relacion.ID_P
AND relacion.ID_J = jugador.ID;*/


/*DAME EL ID DE TAL USERNAME
SELECT jugador.ID FROM jugador
WHERE jugador.Username = 'Erik'*/




