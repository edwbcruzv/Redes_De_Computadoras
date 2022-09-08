/**
 * @file Interface.h
 * @author Edwin Bernardo Cruz Villalba (edwbcruzv)
 * @brief 
 * @version 0.1
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Data.h"

#include <sys/ioctl.h>  // https://man7.org/linux/man-pages/man7/netdevice.7.html https://linux.die.net/man/7/netdevice
#include <net/if.h>
typedef struct ifreq *Interfaz_Red;

typedef struct interface_struct{
    int index;
    char * name;
    Data netmask;
    Data IP;
    Data broadcast;
    Data MAC;
}*Interface;

Interface newInterface();
Interface getInterface(int fd,char* name_interface);
void printInterface(Interface interface);


Interface newInterface(){
    return (Interface)malloc(sizeof(struct interface_struct));
}


Interface getInterface(int fd,char* name_interface){

    Interface temp_i=(Interface)malloc(sizeof(struct interface_struct));

    Interfaz_Red temp_interface=(Interfaz_Red)malloc(sizeof(struct ifreq));
    strncpy(temp_interface->ifr_name,name_interface,strlen(name_interface)+1);

	// INDICE DE LA INTERFAZ
    // SIOCGIFINDEX:Recupere el índice de interfaz de la interfaz en ifr_ifindex .
	if(ioctl(fd,SIOCGIFINDEX,temp_interface)==-1){ 
        perror("Error al obtener el INDICE DE LA INTERFAZ");
        return NULL;
	}

    temp_i->index=temp_interface->ifr_ifindex;

    // NOMBRE DE LA INTERFAZ
    // SIOCGIFNAME:Dado el ifr_ifindex , devuelve el nombre de la interfaz en ifr_name.
    // Este es el único ioctl que devuelve su resultado en ifr_name .
    if(ioctl(fd,SIOCGIFNAME,temp_interface)==-1){ 
        perror("Error al obtener el NOMBRE DE LA INTERFAZ");
        return NULL;
	}

    temp_i->name=(char*)malloc(sizeof(char)*strlen(temp_interface->ifr_name)+1);
    strncpy(temp_i->name,temp_interface->ifr_name,strlen(temp_interface->ifr_name));


    // MASCARA DE RED
    // SIOCGIFNETMASK, SIOCSIFNETMASK
    // Obtenga o configure la máscara de red para un dispositivo mediante ifr_netmask.
    // Por compatibilidad, solo se aceptan o devuelven direcciones AF_INET.
    // La configuración de la máscara de red es una operación privilegiada.
    if(ioctl(fd,SIOCGIFNETMASK,temp_interface)==-1){ 
        perror("Error al obtener la MASCARA DE RED");
        return NULL;
	}

    temp_i->netmask=newData(temp_interface->ifr_netmask.sa_data+2,4);
    

    // DIRECCION MAC
	// SIOCGIFHWADDR, SIOCSIFHWADDR
    // Obtenga o configure la dirección de hardware de un dispositivo mediante ifr_hwaddr.
    // La dirección de hardware se especifica en una estructura sockaddr.
    // sa_family contiene el tipo de dispositivo ARPHRD_*, sa_data la dirección de hardware L2 a partir del byte 0.
    // Establecer la dirección de hardware es una operación privilegiada.
	if(ioctl(fd,SIOCGIFHWADDR,temp_interface)==-1)
	{ perror("Error al obtener la DIRECCION MAC");
		return NULL;
	}

    temp_i->MAC=newData(temp_interface->ifr_hwaddr.sa_data,6);
    

    // DIRECCION DE BROADCAST
    // SIOCGIFBRDADDR , SIOCSIFBRDADDR
    // Obtenga o configure la dirección de transmisión para un dispositivo mediante ifr_brdaddr.
    // Por compatibilidad, solo se aceptan o devuelven direcciones AF_INET.
    // Establecer la dirección de transmisión es una operación privilegiada
    if(ioctl(fd,SIOCGIFBRDADDR,temp_interface)==-1)
	{ perror("Error al obtener la DIRECCION DE BROADCAST");
		return NULL;
	}

    temp_i->broadcast=newData(temp_interface->ifr_broadaddr.sa_data+2,4);
    

    // DIRECCION IP
	// SIOCGIFADDR, SIOCSIFADDR
    // Obtenga o configure la dirección del dispositivo mediante ifr_addr.
    // Establecer la dirección de la interfaz es una operación privilegiada.
    // Por compatibilidad, solo se aceptan o devuelven direcciones AF_INET .
	if(ioctl(fd,SIOCGIFADDR,temp_interface)==-1)
	{ perror("Error al obtener la DIRECCION IP");
		return NULL;
	}

    temp_i->IP=newData(temp_interface->ifr_addr.sa_data+2,4);
    
	return temp_i;
}



void printInterface(Interface interface){
    printf("\n\n\tIndex: %d\n",interface->index);
    printf("\tName: ");puts(interface->name);
    printf("\tNetmask:");printDataInt(interface->netmask);
    printf("\tMAC:");printDataHex(interface->MAC);
    printf("\tIP:");printDataInt(interface->IP);
    printf("\tBroadcast:");printDataInt(interface->broadcast);
    putchar('\n');
}