#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Puerto sobre el cual va a estar escuchando
const int PORT = 8080;

int main(void) {
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port= htons(PORT);
	
	int servidor = socket(AF_INET, SOCK_STREAM, 0);
	
	// Las siguientes dos lineas sirven para no lockear el address
	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

	if (bind(servidor, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("Falló el bind");
		return 1;
	}

	if (listen(servidor,100) == -1) {
		perror("Falló el listen");
		return 1;
	}
	
	printf("Estoy escuchando\n");


	//-------------------

	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion;
	int cliente = accept(servidor, (void*) &direccionCliente, &tamanioDireccion);
	if (cliente == -1) {
		perror("Falló el accept");
		return 1;
	}

	printf("Recibí una conexion en %d!! \n", cliente);
 	send(cliente, "Hola Instancia!\n", 16, 0);
	send(cliente, "I see you! :D\n", 15, 0);

	//------------

	/*char* buffer = malloc(5);

	//	while(1) {
		int bytesRecibidos = recv(cliente, buffer, 4, 0);
		printf("%d", bytesRecibidos);

		if (bytesRecibidos < 0) {
			perror("La Instancia se desconectó o algo sucedio..");
			return 1;
		}

		buffer[bytesRecibidos] = '\0';
		printf("Me llegaron %d bytes con %s\n", bytesRecibidos, buffer);
//}
	free(buffer);

/*
	uint32_t tamanioPaquete;

	recv(cliente, &tamanioPaquete, 4, 0);

	char* buffer = malloc(tamanioPaquete);
	recv(cliente, buffer, tamanioPaquete, 0); //MSG_WAITALL
*/

	for(;;);

	return 0;
}
