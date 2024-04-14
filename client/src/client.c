#include "client.h"
#include <readline/readline.h>
int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger, "Soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	valor = config_get_string_value(config, "CLAVE");
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	log_info(logger, "lei la clave %s",valor);
	// Loggeamos el valor de config

	
	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config, "PUERTO");
	
	// Creamos una conexión hacia el servidor
	
	conexion = crear_conexion(ip, puerto);
	enviar_mensaje(valor, conexion);
	
	// Enviamos al servidor el valor de CLAVE como mensaje

	// Armamos y enviamos el paquete
	paquete(conexion, logger);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "tp0",1,LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("./cliente.config");
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	leido = readline("> ");
	do{
		log_info(logger,leido);
		free(leido);
		leido = readline("> ");
	}while (strcmp(leido,"")!=0);
	// ¡No te olvides de liberar las lineas antes de regresar!
}

void paquete(int conexion, t_log* logger)
{
	// Ahora toca lo divertido!
	// Leemos y esta vez agregamos las lineas al paquete
	char* leido;
	leido = readline("> ");

	t_paquete* paquete;
	paquete = crear_paquete();

	// La primera te la dejo de yapa
	do { 
		log_info(logger, leido); 
		int tamanio = strlen(leido) + 1;
		agregar_a_paquete(paquete, leido, tamanio);
		free (leido);
		leido = readline("> ");
		} 
	while (strcmp(leido, "") != 0);

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
	 // ¡No te olvides de liberar las líneas y el paquete antes de regresar!
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	exit(-1);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
