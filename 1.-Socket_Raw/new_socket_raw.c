/**
 * @file new_socket_raw.c
 * @author Edwin Bernardo Cruz Villalba (edwbcruzv)
 * @brief Se Muesra la apertura basica de un socket crudo
 * @version 1.0
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//++++++++++++++Modulos por defecto++++++++++++++
#include <stdio.h>
#include <stdlib.h>
//-----------------------------------------------

//+++++++++++++++++++++Modulos requeridos por sus respectivas documentaciones+++++++++++++++++++++
#include <sys/socket.h>          // man socket https://man7.org/linux/man-pages/man2/socket.2.html
#include <linux/if_packet.h>    // https://man7.org/linux/man-pages/man7/packet.7.html
#include <net/ethernet.h>       /* the L2 protocols */
/************************************************************************************************************
 * Los sockets de paquetes se utilizan para recibir o enviar paquetes sin formato en el nivel del 
 * controlador de dispositivo (OSI Layer 2). Permiten al usuario implementar módulos de protocolo en el espacio
 * del usuario sobre la capa física 
 ************************************************************************************************************/
#include <arpa/inet.h>             //https://linux.die.net/man/3/htons 
#include <unistd.h>             // cerrar un file descriptor close() https://man7.org/linux/man-pages/man2/close.2.html#DESCRIPTION
//------------------------------------------------------------------------------------------------


/**
 * @brief Ejecucion de un soquet crudo basico y despues de 3 segs se cierra,
 * se debe de ejecutra en modo root.
 * 
 * @return int 
 */

int main(){
    /*++++++++++++++++++++++++++++++SOCKETS++++++++++++++++++++++++++++++
    |              int socket(int dominio , int tipo , int protocolo );
    | packet_socket = socket(AF_PACKET, int socket_type, int protocol);
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    // int socket_type = SOCK_DGRAM //incluir el encabezado no incluir la capa de enlace (Encabezado MAC)

    int socket_type = SOCK_RAW; //incluir el encabezado a nivel de capa de enlace (Encabezado MAC)

    // ver tabla de valores permitidos de ethertype de las tramas Ethernet
    int protocol = htons(ETH_P_ALL);
    int packet_socket = socket(AF_PACKET,socket_type,protocol);

    if (packet_socket==-1){
        perror("Error al abrir el socket: ");
        exit(0);
    }else{
        perror("Exito al abrir el socket.");
    }

    sleep(3);

    close(packet_socket); //cerramos el socket

    printf("Socket cerrado.\n");
    
    return 0;
}