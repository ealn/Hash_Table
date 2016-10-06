/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Main program
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash_table.h"
#include "trace.h"

// Macros to select the OS used
#define WINDOWS
//#define UNIX

//Constanst
#define INSERT_REG 1
#define SEARCH_REG 2
#define SHOW_TABLE 3
#define EXIT       4

//Definition of static functions
static void cleanScreen(void);
static int showGui(void);
static int showMainMenu(int *optionSelected);
static int showRegMenu(int action, int *numberOfSteps);
static int showTable(void);

int main(void)
{
    int ret;
    
    initTrace();
    ret = createTable();
    
    if (ret == SUCCESS)
    {
        ret = showGui();
        //destroyTable();
    }
    else
    {
        MAIN_ERROR("main ERROR - Hash table could not be created\n");
    }
    termTrace();

    MAIN_WARNING("main WARNING - program ended with rc= %d", ret);
    
    return ret;
}

static void cleanScreen(void)
{
#if defined(WINDOWS)
    system("cls");
#elif defined(UNIX) 
    system("clear");
#endif
}

static int showGui(void)
{
    int ret = SUCCESS;
    int optionSelected = 0;
    int numberOfSteps = 0;
    int loop = 0;
        
    do
    {
        loop = 0;
        
        ret = showMainMenu(&optionSelected);
    
        switch (optionSelected)
        {
            case INSERT_REG: ret = showRegMenu(INSERT_REG, &numberOfSteps);
                break;
            case SEARCH_REG: ret = showRegMenu(SEARCH_REG, &numberOfSteps);
                break;
            case SHOW_TABLE: ret = showTable();
                break;
            default: break;
        }
        
        if (optionSelected != EXIT)
        {
            char tempValue[256];
            char repeat;
    
            memset(&tempValue, 0, sizeof(char)*256);
    
            printf("Deseas realizar otra operacion [s,n]: ");
            fgets(tempValue, 256, stdin);
            
            repeat = tempValue[0];

            if ((repeat == 's' || repeat == 'S')
                && strlen(tempValue) == 2)   //2 characters = 1 character + \0 null termininator
            {
                loop = 1;
                cleanScreen();
            }
        }
    }while (loop == 1);
    
    
    return ret;
}

static int showMainMenu(int *optionSelected)
{
    int ret = SUCCESS;
    
    if (optionSelected != NULL)
    {
        char tempValue[256];
        
        memset(&tempValue, 0, sizeof(char)*256);
        
        do 
        {
          printf("\n                    ******* HASH TABLE *******\n\n");
          printf("Selecciona la opcion deseada:\n\n");
          printf("1.- Insertar Registro a la hash table\n");
          printf("2.- Buscar un registro\n");
          printf("3.- Mostrar la hash table\n");
          printf("4.- Salir\n\n");
          printf("Opcion Seleccionada: ");
          fgets(tempValue, 256, stdin);
          
          *optionSelected = atoi(tempValue);
           
          if (*optionSelected < 1 || *optionSelected > 4)
          {
              cleanScreen();
          }
        }while (*optionSelected < 1 || *optionSelected > 4);
    }
    else
    {
        ret = FAIL;
    }
    
    return ret;
}

static int showRegMenu(int action, int *numberOfSteps)
{
    int ret = SUCCESS;
    
    if (numberOfSteps != NULL)
    {
       long   ID; 
       char   tempValue[256];
          
       memset(&tempValue, 0, sizeof(char)*256);
          
       if (action == INSERT_REG)
       {
          char   name[NAME_SIZE];
          char   tel[TEL_SIZE];
          char   address[ADD_SIZE];
          char   city[CITY_SIZE];

          printf("\n\n                    ******* Informacion del Registro *******\n\n");
          printf("Por favor intruduce los datos del registro: \n");
          
          //prevent buffer overflows with the variable tempValue
          printf("ID: ");
          fgets(tempValue, 256, stdin);
          ID = atoi(tempValue);
          
          printf("Name: ");
          fgets(tempValue, 256, stdin);
          memcpy(name, tempValue, sizeof(char)*NAME_SIZE);
          
          printf("Telephone: ");
          fgets(tempValue, 256, stdin);
          memcpy(tel, tempValue, sizeof(char)*TEL_SIZE);
          
          printf("Address: ");
          fgets(tempValue, 256, stdin);
          memcpy(address, tempValue, sizeof(char)*ADD_SIZE);
          
          printf("City: ");
          fgets(tempValue, 256, stdin);
          memcpy(city, tempValue, sizeof(char)*CITY_SIZE);
          
          insertReg(ID, name, tel, address, city);
       }
       else if (action == SEARCH_REG)
       {
          printf("\n\n                    ******* Informacion del Registro *******\n\n");
          printf("Por favor intruduce el ID del registro: \n");
          fgets(tempValue, 200, stdin);
          ID = atoi(tempValue);
          
          ret = searchReg(ID, numberOfSteps);
       }
    }
    else
    {
        ret = FAIL;
    }
    
    return ret;
}

static int showTable(void)
{
    int ret = SUCCESS;
    
    displayTable();
    
    return ret;
}
