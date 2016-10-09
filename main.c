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

// Macros to select the OS to be used
#define WINDOWS
//#define UNIX

//Constanst
#define INSERT_REG       1
#define SEARCH_REG       2
#define REMOVE_REG       3
#define SHOW_TABLE       4
#define EXIT             5
#define STDIN_BUF_SIZE   256

//Definition of static functions
static void cleanScreen(void);
static int  showGui(void);
static int  showMainMenu(int *optionSelected);
static int  showRegMenu(int action, unsigned int *numberOfSteps);
static int  showTable(void);

int main(void)
{
    int ret = SUCCESS;
    
    initTrace();
    ret = createTable();
    
    if (ret == SUCCESS)
    {
        ret = showGui();
        destroyTable();
    }
    else
    {
        MAIN_ERROR("main() Hash table could not be created\n");
    }
    
    MAIN_DEBUG("main() program ended with rc= %d\n", ret);
    
    termTrace();
    
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
    unsigned int numberOfSteps = 0;
    int loop = 0;
        
    do
    {
        loop = 0;
        
        ret = showMainMenu(&optionSelected);
    
        MAIN_DEBUG("showGui() optionSelected=%i\n", optionSelected);

        switch (optionSelected)
        {
            case INSERT_REG: ret = showRegMenu(INSERT_REG, &numberOfSteps);
                break;
            case SEARCH_REG: ret = showRegMenu(SEARCH_REG, &numberOfSteps);
                break;
            case REMOVE_REG: ret = showRegMenu(REMOVE_REG, &numberOfSteps);
                break;
            case SHOW_TABLE: ret = showTable();
                break;
            default: break;
        }
        
        if (optionSelected != EXIT)
        {
            char stdinBuf[STDIN_BUF_SIZE];
            char repeat;
    
            memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);
    
            printf("Deseas realizar otra operacion [s,n]: ");
            fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
            
            repeat = stdinBuf[0];

            if ((repeat == 's' || repeat == 'S')
                && strlen(stdinBuf) == 2)   //2 characters = 1 character + \0 (null termininator)
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
        char stdinBuf[STDIN_BUF_SIZE];
        
        memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);
        
        do 
        {
          printf("\n                    ******* HASH TABLE *******\n\n");
          printf("Selecciona la opcion deseada:\n\n");
          printf("1.- Insertar Registro a la hash table\n");
          printf("2.- Buscar un registro\n");
          printf("3.- Mostrar la hash table\n");
          printf("4.- Salir\n\n");
          printf("Opcion Seleccionada: ");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          
          *optionSelected = atoi(stdinBuf);
           
          if (*optionSelected < INSERT_REG || *optionSelected > EXIT)
          {
              cleanScreen();
          }
        }while (*optionSelected < INSERT_REG || *optionSelected > EXIT);
    }
    else
    {
        ret = FAIL;
    }
    
    return ret;
}

static int showRegMenu(int action, unsigned int *numberOfSteps)
{
    int ret = SUCCESS;
    
    if (numberOfSteps != NULL)
    {
       long   ID; 
       char   stdinBuf[STDIN_BUF_SIZE];
          
       memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);
          
       if (action == INSERT_REG)
       {
          char   name[NAME_SIZE];
          char   tel[TEL_SIZE];
          char   address[ADD_SIZE];
          char   city[CITY_SIZE];

          printf("\n\n                    ******* Informacion del Registro *******\n\n");
          printf("Por favor intruduce los datos del registro: \n");
          
          //prevent buffer overflows with the variable stdinBuf
          printf("ID: ");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          ID = atoi(stdinBuf);
          
          printf("Name: ");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          memcpy(name, stdinBuf, sizeof(char)*NAME_SIZE);
          
          printf("Telephone: ");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          memcpy(tel, stdinBuf, sizeof(char)*TEL_SIZE);
          
          printf("Address: ");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          memcpy(address, stdinBuf, sizeof(char)*ADD_SIZE);
          
          printf("City: ");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          memcpy(city, stdinBuf, sizeof(char)*CITY_SIZE);
          
          MAIN_DEBUG("showRegMenu() InsertReg \nID: %d \nname: %stel: %saddress: %scity: %s\n",
                     ID,
                     name,
                     tel,
                     address,
                     city);

          insertReg(ID, name, tel, address, city);
       }
       else if (action == SEARCH_REG)
       {
          printf("\n\n                    ******* Informacion del Registro *******\n\n");
          printf("Por favor intruduce el ID del registro a buscar: \n");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          ID = atoi(stdinBuf);
          
          MAIN_DEBUG("showRegMenu() searchReg ID=%d\n", ID);
          ret = searchReg(ID, numberOfSteps);
       }
       else if (action == REMOVE_REG)
       {
          printf("\n\n                    ******* Informacion del Registro *******\n\n");
          printf("Por favor intruduce el ID del registro a eliminar: \n");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          ID = atoi(stdinBuf);
          
          MAIN_DEBUG("showRegMenu() removeReg ID=%d\n", ID);
          ret = removeReg(ID);
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
