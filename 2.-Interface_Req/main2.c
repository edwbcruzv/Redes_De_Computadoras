/**
 * @file main2.c
 * @author Edwin Bernardo Cruz Villalba (edwbcruzv)
 * @brief Se obtiene informacion de una interfaz de red usando un socket crudo.
 * @version 0.1
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "MySocketRaw.h"
#include "Data.h"

#include <sys/ioctl.h>  // https://man7.org/linux/man-pages/man7/netdevice.7.html https://linux.die.net/man/7/netdevice
#include <net/if.h>

/**
 * @brief Renombrando la estructura de la interfaz, importante crear la instancia,
 * sino da error aunque el socket este creado no tendra en donde guardar los datos.
 * 
 */
typedef struct ifreq *Interfaz_Red;

/**
 * @brief por medio de i control y de la documentacion:
 * ttps://man7.org/linux/man-pages/man7/netdevice.7.html https://linux.die.net/man/7/netdevice
 * Se obtiene los parametros para obtener la informacion que necesitemos.
 * Siempre y cuando empecemos por el indice.
 * 
 * @param socket_raw 
 * @return int 
 */
int obtenerDatos(MySocketRaw socket_raw);

int main(int argc, char const *argv[]){

    MySocketRaw mysocketraw=newMySocketRaw();

    if (mysocketraw==NULL){
        perror("Error al abrir el socket: ");
        exit(0);
    }else{
        perror("Exito al abrir el socket.");
    }
    
    if (obtenerDatos(mysocketraw)==-1){
        perror("Error al obtener datos");
    }else{
        deleteMySocketRaw(mysocketraw);
        perror("Socket cerrado.");
    }
        
    return 0;
}

int obtenerDatos(MySocketRaw socket_raw){

    int fd=socket_raw->fd_socket;

    Interfaz_Red interface=(Interfaz_Red)malloc(sizeof(struct ifreq));

	int index;
	char* name;
    char name_aux[30];
    Data netmask,IP,broadcast,MAC;
    
	printf("Inserta el nombre de la red (Ver las interfaces usando 'ifconfig' en otra terminal): ");
	scanf("%s",interface->ifr_name);
	// INDICE DE LA INTERFAZ
    // SIOCGIFINDEX:Recupere el índice de interfaz de la interfaz en ifr_ifindex .
	if(ioctl(fd,SIOCGIFINDEX,interface)==-1){ 
        perror("Error al obtener el INDICE DE LA INTERFAZ");
        return -1;
	}

    index=interface->ifr_ifindex;
    printf("Indice de la interfaz:%d\n",index);

    // NOMBRE DE LA INTERFAZ
    // SIOCGIFNAME:Dado el ifr_ifindex , devuelve el nombre de la interfaz en ifr_name.
    // Este es el único ioctl que devuelve su resultado en ifr_name .
    if(ioctl(fd,SIOCGIFNAME,interface)==-1){ 
        perror("Error al obtener el NOMBRE DE LA INTERFAZ");
        return -1;
	}

    name=(char*)malloc(sizeof(char)*strlen(interface->ifr_name));
    strncpy(name,interface->ifr_name,strlen(interface->ifr_name));
    printf("Nombre de la interfaz:");
    puts(name);


    // MASCARA DE RED
    // SIOCGIFNETMASK, SIOCSIFNETMASK
    // Obtenga o configure la máscara de red para un dispositivo mediante ifr_netmask.
    // Por compatibilidad, solo se aceptan o devuelven direcciones AF_INET.
    // La configuración de la máscara de red es una operación privilegiada.
    if(ioctl(fd,SIOCGIFNETMASK,interface)==-1){ 
        perror("Error al obtener la MASCARA DE RED");
        return -1;
	}

    netmask=newData(interface->ifr_netmask.sa_data+2,4);
    printf("Mascara de subred:");
    printDataInt(netmask);

    // DIRECCION MAC
	// SIOCGIFHWADDR, SIOCSIFHWADDR
    // Obtenga o configure la dirección de hardware de un dispositivo mediante ifr_hwaddr.
    // La dirección de hardware se especifica en una estructura sockaddr.
    // sa_family contiene el tipo de dispositivo ARPHRD_*, sa_data la dirección de hardware L2 a partir del byte 0.
    // Establecer la dirección de hardware es una operación privilegiada.
	if(ioctl(fd,SIOCGIFHWADDR,interface)==-1)
	{ perror("Error al obtener la DIRECCION MAC");
		return -1;
	}

    MAC=newData(interface->ifr_hwaddr.sa_data,6);
    printf("MAC:");
    printDataHex(MAC);

    // DIRECCION DE BROADCAST
    // SIOCGIFBRDADDR , SIOCSIFBRDADDR
    // Obtenga o configure la dirección de transmisión para un dispositivo mediante ifr_brdaddr.
    // Por compatibilidad, solo se aceptan o devuelven direcciones AF_INET.
    // Establecer la dirección de transmisión es una operación privilegiada
    if(ioctl(fd,SIOCGIFBRDADDR,interface)==-1)
	{ perror("Error al obtener la DIRECCION DE BROADCAST");
		return -1;
	}

    broadcast=newData(interface->ifr_broadaddr.sa_data+2,4);
    printf("Broadcast:");
    printDataInt(broadcast);

    // DIRECCION IP
	// SIOCGIFADDR, SIOCSIFADDR
    // Obtenga o configure la dirección del dispositivo mediante ifr_addr.
    // Establecer la dirección de la interfaz es una operación privilegiada.
    // Por compatibilidad, solo se aceptan o devuelven direcciones AF_INET .
	if(ioctl(fd,SIOCGIFADDR,interface)==-1)
	{ perror("Error al obtener la DIRECCION IP");
		return -1;
	}

    IP=newData(interface->ifr_addr.sa_data+2,4);
    printf("IP:");
    printDataInt(IP);
	return 1;
}