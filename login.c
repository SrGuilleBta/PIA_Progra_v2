#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "./lib/login.h"

int loginUsuario(void){
    int intento = 0;
    dataU comparar;
    FILE *archUsuario;
    if ((archUsuario = fopen("./usuarios/usuariosData.bin", "rb+"))==NULL){
        printf("\nError al abrir el archivo.");
        return 1;
    }
    while (intento<3)
    {
        printf("\nComercializadora Fuentes");
        printf("\nUsuario: ");
        fflush(stdin);
        fgets(comparar.nombreUsuario, 30, stdin);
        //Quitar caracter de nueva linea
        comparar.nombreUsuario[strcspn(comparar.nombreUsuario, "\n")] = '\0';
        printf("\nContrasena: ");
        fgets(comparar.contra, 30, stdin);
        //Quitar caracter de nueva linea
        comparar.contra[strcspn(comparar.contra, "\n")] = '\0';
        if (confirmarUsuario(&comparar, &intento)==0)
            //Si todo coincide con exito se regresa 0
            return 0;
    }
    printf("\nIntentos agotados, saliendo...");
    Sleep(3);
    return 1;
}

int confirmarUsuario(dataU *comparar, int *intento){
    FILE *archUsuario;
    dataU temp;
    if ((archUsuario = fopen("./usuarios/usuariosData.bin", "rb+"))==NULL){
        printf("\nError al abrir el archivo.");
        return 1;
    }
    while(fread(&temp, sizeof(dataU), 1, archUsuario)!=0){
        //Se compara el nombre de usuario proporcionado con los encontrados en el archivo de usuarios
        //Ademas se compreueba que el usuario no se encuentre bloqueado
        if(strcmp(temp.nombreUsuario, comparar->nombreUsuario)==0 && temp.status == 1){
            //Se compara la contrasena del usuario
            if((strcmp(temp.contra, comparar->contra))==0){
                //retorna 0 si todo sale bien y coinciden usuario y contrasena
                fclose(archUsuario);
                return 0;
            }
            else{
                printf("\nNombre de usuario o contrasena invalido, intente de nuevo!!!");
                //Si el tipo de usuario es de tipo admin, no se suman los intentos
                if (temp.tipoUsuario == 1){
                    fclose(archUsuario);
                    return -1;
                }
                //Si el usuario es un usuario normal si se suman intentos
                else if(temp.tipoUsuario == 0)
                    (*intento)++;
                //Si ya se hicieron 3 o mas intentos, se bloquea el usuario
                if ((*intento)>=3){
                    printf("\nUsuario Bloqueado, contactar a su administrador para desbloquear.");
                    temp.status = 0;
                    //Escribiendo al archivo de usuarios
                    fseek(archUsuario, -(long)sizeof(dataU), SEEK_CUR);
                    if((fwrite(&temp, sizeof(dataU), 1, archUsuario))!=1){
                        printf("Error al escribir al archivo.");
                        return 1;
                    }
                }
                //Se retorna -1 en caso de que algo no coincida
                fclose(archUsuario);
                return -1;
            }
        }
        //Si el usuario ingresado ya se encuentra desactivado, se le notifica al usuario
        else if(strcmp(temp.nombreUsuario, comparar->nombreUsuario)==0 && temp.status == 0){
            printf("Usuario se encuentra desactivado.");
            fclose(archUsuario);
            return -1;
        }
    }
    //Si el usuario ingresado no coincide con ninguno en el archivo se cierra el mismo y 
    //se regresa -1
    printf("\nNombre de usuario o contrasena invalido, intente de nuevo!!!");
    fclose(archUsuario);
    return -1;
}