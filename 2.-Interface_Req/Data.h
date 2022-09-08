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
    if (paq!=NULL){
        memcpy(temp->data,paq,len);
    }
    return temp;
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
    puts("\n");
}


