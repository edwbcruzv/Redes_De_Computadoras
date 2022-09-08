/**
 * @file Data.h
 * @author Edwin Bernardo Cruz Villalba (edwbcruzv)
 * @brief Este modulo ofrece un mejor manejo de las estructuras y arreglos dinamicos que 
 * estan a nivel de bytes (unsigned char), ofreciendo funciones similares a
 * como si usaramos cadenas de caracteres (char).
 * @version 1.0
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief La estructura  'Data'  contiene un apuntador para almacenar los datos y
 * un valor entero donde almacenaremos el tamaño de dicha estructura
 * 
 */

typedef struct data_struct{
    unsigned char* data;
    int lenght;
    }*Data;


Data newData(unsigned char* paq,int len);
Data newDataEmpty(int len);

Data concatData(Data d1,Data d2);
Data zerosData(int len);

Data EncabezadoMAC(Data mac_destino,Data mac_origen,Data ethertype,Data datos);

/**
 * @brief Crea un nuevo Data en base al tamaño que reciba, puede no estar asociado
 * a un  paquete de datos ya que se da la oportunidad de agregar despues, pero recordando la
 * longitud que se le dio al principio
 * 
 * @param paq paquete de datos
 * @param len tamaño del paquete de datos, es asignado por el usuario
 * @return Data 
 */
Data newData(unsigned char* paq,int len){

    Data temp=(Data)malloc(sizeof(struct data_struct));
    temp->data=(unsigned char*)malloc(sizeof(unsigned char)*len);
    temp->lenght=len;

    
    memcpy(temp->data,paq,len);

    return temp;
}

Data newDataEmpty(int len){
    Data temp=(Data)malloc(sizeof(struct data_struct));
    temp->data=(unsigned char*)malloc(sizeof(unsigned char)*len);
    temp->lenght=len;
    return temp;
}

/**
 * @brief Concatena dos estructuras tipo Data, ambas Data a concatenar son eliminadas
 * regresando solo una
 * 
 * @param d1 
 * @param d2 
 * @return Data 
 */
Data concatData(Data d1,Data d2){
    int ptr;

    if (d1->lenght==0){
        return d2;
    }else if (d2->lenght==0)
    {
       return d1;
    }else{
        Data temp=newDataEmpty(d1->lenght+d2->lenght);
        ptr=0;
        memcpy(temp->data+ptr,d1->data,d1->lenght);
        ptr=d1->lenght;
        memcpy(temp->data+ptr,d2->data,d2->lenght);
        // free(d1);
        // free(d2);
        return temp;
    }
}

/**
 * @brief crera una Data de ceros del la longitud que se solicite
 * 
 * @param len 
 * @return Data 
 */
Data zerosData(int len){
    unsigned char temp[len];
    for (int i = 0; i < len; i++){temp[i]=0x00;}
    Data d=newData(temp,len);
    return d;
}

/**
 * @brief Estructura del encabezado mac
 * **********************Estructura del bufer que se envia***************
 *          6 bytes            6 bytes          2 bytes      0-1500 bytes
 *       +----------------------------------------------------------------+
 *       | Direccion MAC |  Direccion MAC  |   Ethertype  |      Datos    |
 *       |  Destino      |   Origen        |              |               |
 *       +----------------------------------------------------------------+
 *index:       [0-5]         [6-11]             [12-13]      [14-1513]
 * Longitud total:1514 bytes
 * 
 * Para que pueda ser enviado el paquete debe de ser minumo de 60 bytes
 * 
 * @param mac_destino 
 * @param mac_origen 
 * @param ethertype 
 * @param paq 
 * @return Data 
 */
Data EncabezadoMAC(Data mac_destino,Data mac_origen,Data ethertype,Data datos){
    // que todos los parametros cumplan con los requisitos
    if (mac_destino->lenght==6 && mac_origen->lenght==6 && ethertype->lenght==2 && datos->lenght<1500 )
    {
        Data temp=concatData(concatData(concatData(mac_destino,mac_origen),ethertype),datos);

        //aseguramos de que el encabezado sea minimo de 60 bytes
        int resto=60-(temp->lenght);
        if (resto > 0){
            return temp=concatData(temp,zerosData(resto));
        }
        
        return temp;
    }
    else{
        return NULL;
    }
}



/**
 * @brief Muestra como esta compuesta el paquete mostrando el 
 * subindice del dato y mostrando en 3 formas diferentes
 * 
 * @param paq 
 */
void printshowIndexData(Data paq){
    for (int i = 0; i < paq->lenght; i++)
    {
        fprintf(stdout,"\n[%d]=%c,%d,%x",i,paq->data[i],paq->data[i],paq->data[i]);
    }
    printf("\n");
}

/**
 * @brief Muestra el paquete en formato hexadecimal, que es la forma mas comun.
 * 
 * @param paq 
 */
void printData(Data paq){
    for (int i = 0; i < paq->lenght; i++)
    {
        fprintf(stdout," %.4x",paq->data[i]);
    }
    printf("\n");
}

/**
 * @brief Muestra el paquete en caracteres
 * 
 * @param paq 
 */

void printDataChar(Data paq){
    for (int i = 0; i < paq->lenght; i++)
    {
        fprintf(stdout," %c",paq->data[i]);
    }
    printf("\n");
}

/**
 * @brief Muestra el paquete en formato de enteros
 * 
 * @param paq 
 */
void printDataInt(Data paq){
    for (int i = 0; i < paq->lenght; i++)
    {
        fprintf(stdout," %d",paq->data[i]);
    }
    printf("\n");
}

/**
 * @brief Muestra el paquete en formato hexadecimal sin procesar
 * 
 * @param paq 
 */
void printDataHex(Data paq){
    for (int i = 0; i < paq->lenght; i++)
    {
        fprintf(stdout," %x",paq->data[i]);
    }
    printf("\n");
}

/**
 * @brief Muestra el paquete en un hexadecimal y en formato de una trama
 * 
 * @param paq 
 */
void printDataGram(Data paq){

    puts("\n");
    for (int i = 0; i < paq->lenght;)
    {
        //printf("  %.2hx[%d]",trama[i],i);
        printf("  %.2hx",paq->data[i]);
        i++;
        if (i%16==0){
            puts("\n");
        }    
    }
    printf("\n\n  lenght:%d\n\n",paq->lenght);
}


