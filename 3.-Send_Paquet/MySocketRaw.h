/**
 * @file MySocketRaw.h
 * @author Edwin Bernardo Cruz Villalba (edwbcruzv)
 * @brief Este modulo ofrece las operaciones basicas de un socket crudo y pueda ser usado en un cliente-servidor
 * y una forma comoda de usar hilos
 * @version 2.0
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


//********************Modulos propio, ya estan incluidos los paquetes mas comunes*******************************
#include "Interface.h"
//****************************Modulos requeridos por sus respectivas documentaciones****************************
#include <sys/socket.h>          // man socket https://man7.org/linux/man-pages/man2/socket.2.html
#include <linux/if_packet.h>    // https://man7.org/linux/man-pages/man7/packet.7.html
#include <net/ethernet.h>       /* the L2 protocols */
/***************************************************************************************************************
 * Los sockets de paquetes se utilizan para recibir o enviar paquetes sin formato en el nivel del 
 * controlador de dispositivo (OSI Layer 2). Permiten al usuario implementar módulos de protocolo en el espacio
 * del usuario sobre la capa física 
 **************************************************************************************************************/
#include <arpa/inet.h>          //https://linux.die.net/man/3/htons 
#include <unistd.h>             // cerrar un file descriptor close() https://man7.org/linux/man-pages/man2/close.2.html#DESCRIPTION
//**************************************************************************************************************

/**
 * @brief MySocketRaw es una estructura dinamica que contiene el
 * descriptor de socket y una direccion
 * 
 */

typedef struct my_socket_raw{
    int fd_socket; //descriptor de socket 
    struct sockaddr_ll *dir; //direccion, esta sera dinamica
    Data datagram; //datagrama
}*MySocketRaw;

MySocketRaw newMySocketRaw();
void configAddr(MySocketRaw mysocketraw,Interface interface);
int deleteMySocketRaw(MySocketRaw mysocketraw);
int sendToMySocketRaw(MySocketRaw mysocketraw);


/**
 * @brief funcion constructora, al momento de su llamada abre un socket crudo.
 * 
 * @return MySocketRaw 
 */
MySocketRaw newMySocketRaw(){
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
        return NULL;
    }else{
        MySocketRaw temp_mysocket=(MySocketRaw)malloc(sizeof(struct my_socket_raw));
        temp_mysocket->fd_socket=packet_socket;
        temp_mysocket->datagram=NULL;
        temp_mysocket->dir=NULL;
        return temp_mysocket;
    }
}


/**
 * -----------------------------------------------------------------
 * struct sockaddr {
            ushort  sa_family;
            char    sa_data[14];
    };
 * -----------------------------------------------------------------
 * https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
 * struct sockaddr_in {
        short            sin_family;   // e.g. AF_INET
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };

    struct in_addr {
        unsigned long s_addr;  // load with inet_aton()
    };
 * -----------------------------------------------------------------
 * https://man7.org/linux/man-pages/man7/packet.7.html
 * struct sockaddr_ll {
               unsigned short sll_family;   // Always AF_PACKET 
               unsigned short sll_protocol; // Physical-layer protocol 
               int            sll_ifindex;  // Interface number 
               unsigned short sll_hatype;   // ARP hardware type 
               unsigned char  sll_pkttype;  // Packet type 
               unsigned char  sll_halen;    // Length of address 
               unsigned char  sll_addr[8];  // Physical-layer address 
           };
 * ------------------------------------------------------------------
 */

/**
 * @brief se llena el apartado de la direccion
 * 
 * @param mysocketraw 
 * @param interface 
 */
void configAddr(MySocketRaw mysocketraw,Interface interface){
    struct sockaddr_ll* capa_enlace=(struct sockaddr_ll*)malloc(sizeof(struct sockaddr_ll));
    memset(capa_enlace,0x00,sizeof(capa_enlace));
    capa_enlace->sll_family=AF_PACKET;
    capa_enlace->sll_protocol=htons(ETH_P_ALL);
    capa_enlace->sll_ifindex=interface->index;

    mysocketraw->dir=capa_enlace;

}

/**
 * @brief Cierra el socket y libera la memoria de la direccion
 * 
 * @param mysocketraw 
 * @return int 
 */
int deleteMySocketRaw(MySocketRaw mysocketraw){
    int fd=mysocketraw->fd_socket;
    free(mysocketraw->dir);
    return close(fd);
}

/**
 * @brief Envia un paquete de datos, el cual ya debio de ser armado por el usuario y cumplir
 * con los requisitos correctos.
 * 
 * @param mysocketraw 
 * @return int 
 */
int sendToMySocketRaw(MySocketRaw mysocketraw){

    int tam=sendto(mysocketraw->fd_socket,mysocketraw->datagram->data,mysocketraw->datagram->lenght,0,(struct sockaddr*)mysocketraw->dir,sizeof(*(mysocketraw->dir)));
    if(tam==-1){
        perror("Error en enviar.");
        return -1;
    }
    else{
        perror("Exito en enviar.");
        return 0;
    }

}

// MySocketRaw recvFromMySocketRaw(){

//     recvfrom()
// }