/**
 * @file main.c
 * @author Edwin Bernardo Cruz Villalba (edwbcruzv)
 * @brief 
 * @version 0.1
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "MySocketRaw.h"




int main(int argc, char const *argv[])
{
    MySocketRaw socket=newMySocketRaw();

    if (socket==NULL){
        perror("Error al abrir el socket: ");
        exit(0);
    }else{
        perror("Exito al abrir el socket.");
    }
    
    Interface interfaz_origen=getInterface(socket->fd_socket,"wlp7s0");

    if (interfaz_origen==NULL){
        perror("Error al obtener datos");
    }else{
        printf("\nInformacion de la interfaz:");
        printInterface(interfaz_origen);
        
    }

    unsigned char MACbroadcast[6]={0xff,0xff,0xff,0xff,0xff,0xff}; // que el paquete sea enviato a todos en la red
    unsigned char ethertype[2]={0xaa,0xaa};
    
    char *msg="Esto es un mensaje para todas las maquinas";
    Data mensaje=newData(msg,strlen(msg));
    printf("\nMensaje a enviar:");
    printDataChar(mensaje);

    socket->datagram=EncabezadoMAC(newData(MACbroadcast,6),interfaz_origen->MAC,newData(ethertype,2),mensaje);

    configAddr(socket,interfaz_origen);
    printf("\nPaquete a enviar:");
    printDataGram(socket->datagram);
    
    sendToMySocketRaw(socket);
    deleteMySocketRaw(socket);
    perror("Socket cerrado.");
    return 0;
}
