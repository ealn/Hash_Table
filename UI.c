/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Implementation of User interface
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "UI.h"
#include "hash_table.h"
#include "trace.h"
#include "memutils.h"

// Macros to select the OS to be used
#define WINDOWS
//#define UNIX

//Constanst
#define INSERT_REG            1
#define SEARCH_REG            2
#define REMOVE_REG            3
#define CHANGE_REG            4
#define SHOW_TABLE            5
#define EXIT                  6
#define STDIN_BUF_SIZE        256
#define SHOW_FULL_TABLE       1
#define SHOW_SUMMARY_TABLE    2
#define SEND_TO_CONSOLE       1
#define SEND_TO_FILE          2
#define OUTPUT_FILE_SIZE      20
#define REG_BUF_SIZE          256

//Definition of static functions
static void cleanScreen(void);
static int32_t  showMainMenu(uint8_t *optionSelected);
static void getRegInfo(uint32_t * pID,
                       char * pName,
                       char * pTel,
                       char * pAddress,
                       char * pCity);
static int32_t showInsertRegMenu(void);
static int32_t showSearchRegMenu(void);
static int32_t showRemoveRegMenu(void);
static int32_t showChangeRegMenu(void);
static int32_t showTable(void);

//Type definitions
typedef struct _TableView
{
    uint32_t    view;
    uint32_t    output;
    char        outputFile[OUTPUT_FILE_SIZE];
}TableView;

//global variables
TableView * g_tableView = NULL;

static void cleanScreen(void)
{
#if defined(WINDOWS)
    system("cls");
#elif defined(UNIX) 
    system("clear");
#endif
}

static void createTableView(void)
{
    if (g_tableView == NULL)
    {
        g_tableView = MEMALLOC(sizeof(TableView));

        if (g_tableView == NULL)
        {
            UI_ERROR("Table view could not be allocated\n");
        }
    }
}

static void destroyTableView(void)
{
    if (g_tableView != NULL)
    {
        MEMFREE((void *)g_tableView);
        g_tableView = NULL;
    }
    else
    {
        UI_WARNING("Table view is null\n");
    }
}

int32_t showUI(void)
{
    int32_t ret = SUCCESS;
    uint8_t optionSelected = 0;
    bool loop = false;
        
    createTableView();

    do
    {
        loop = false;
        
        ret = showMainMenu(&optionSelected);
    
        UI_DEBUG("optionSelected=%i\n", optionSelected);

        switch (optionSelected)
        {
            case INSERT_REG: ret = showInsertRegMenu();
                break;
            case SEARCH_REG: ret = showSearchRegMenu();
                break;
            case REMOVE_REG: ret = showRemoveRegMenu();
                break;
            case CHANGE_REG: ret = showChangeRegMenu();
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
                loop = true;
                cleanScreen();
            }
        }
    }while (loop);
    
    destroyTableView();

    return ret;
}

static int32_t showMainMenu(uint8_t *optionSelected)
{
    int32_t ret = SUCCESS;
    
    if (optionSelected != NULL)
    {
        char stdinBuf[STDIN_BUF_SIZE];
        
        memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);
        
        do 
        {
          printf("\n                    ******* HASH TABLE *******\n\n");
          printf("Selecciona la opcion deseada:\n\n");
          printf("1.- Insertar un registro\n");
          printf("2.- Buscar un registro\n");
          printf("3.- Borrar un registro\n");
          printf("4.- Cambiar un registro\n");
          printf("5.- Mostrar la tabla\n");
          printf("6.- Salir\n\n");
          printf("Opcion Seleccionada: ");
          fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
          
          *optionSelected = atoi(stdinBuf);
          
          UI_DEBUG("optionSelected=%i\n", *optionSelected);

          if (*optionSelected < INSERT_REG || *optionSelected > EXIT)
          {
              printf("\nOpcion invalida\n");
              getchar();
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

static void printOutput(int32_t ret, uint32_t numberOfSteps)
{
    switch (ret)
    {
        case SUCCESS: printf("\nOperacion exitosa!!!\n");
            break;
        case FAIL: printf("\nHubo un ERROR mientras se ejecutaba el programa\n");
            break;
        case REG_NOT_FOUND: printf("\nEl registro no fue encontrado\n");
            break;
        case REG_DUPLICATED: printf("\nEl registro esta duplicado\n");
            break;
        default:
            break;
    }

    printf("\nNumero de pasos que tomo realizar la accion = %d\n\n", numberOfSteps);
}

static void getRegInfo(uint32_t * pID,
                       char * pName,
                       char * pTel,
                       char * pAddress,
                       char * pCity)
{
    char stdinBuf[STDIN_BUF_SIZE];

    memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);

    //prevent buffer overflows with the variable stdinBuf
    if (pID != NULL)
    {
        printf("ID: "); 
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        *pID = atoi(stdinBuf);
    }
    
    if (pName != NULL)
    {
        printf("Name: "); 
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        memcpy(pName, stdinBuf, sizeof(char)*(NAME_SIZE - 1)); //-1 to keep the '\0' at the end
    }
    
    if (pTel != NULL)
    {
        printf("Telephone: "); 
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        memcpy(pTel, stdinBuf, sizeof(char)*(TEL_SIZE - 1)); //-1 to keep the '\0' at the end
    }
    
    if (pAddress != NULL)
    {
        printf("Address: "); 
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        memcpy(pAddress, stdinBuf, sizeof(char)*(ADD_SIZE - 1)); //-1 to keep the '\0' at the end
    }
    
    if (pCity != NULL)
    {
        printf("City: "); 
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        memcpy(pCity, stdinBuf, sizeof(char)*(CITY_SIZE - 1)); //-1 to keep the '\0' at the end
    }
}

static int32_t showInsertRegMenu(void)
{
    int32_t    ret = SUCCESS;
    uint32_t   ID = 0; 
    uint32_t   numberOfSteps = 0;
    char       name[NAME_SIZE];
    char       tel[TEL_SIZE];
    char       address[ADD_SIZE];
    char       city[CITY_SIZE];

    memset(&name, 0, sizeof(char)*NAME_SIZE);
    memset(&tel, 0, sizeof(char)*TEL_SIZE);
    memset(&address, 0, sizeof(char)*ADD_SIZE);
    memset(&city, 0, sizeof(char)*CITY_SIZE);

    cleanScreen();

    printf("\n                    ******* Informacion del Registro *******\n\n");
    printf("Por favor intruduce los datos del registro\n");
    
    getRegInfo(&ID, name, tel, address, city);
    
    UI_DEBUG(" Insert register:\nID: %d \nname: %stel: %saddress: %scity: %s\n",
             ID,
             name,
             tel,
             address,
             city);
  
    ret = insertReg(ID, name, tel, address, city, &numberOfSteps);
  
    printOutput(ret, numberOfSteps);

    return ret;
}

static int32_t showSearchRegMenu(void)
{
    int32_t    ret = SUCCESS;
    uint32_t   ID = 0; 
    uint32_t   numberOfSteps = 0;
   
    cleanScreen();

    printf("\n                    ******* Informacion del Registro *******\n\n");
    printf("Por favor intruduce el ID del registro a buscar\n");

    getRegInfo(&ID, NULL, NULL, NULL, NULL);
    
    UI_DEBUG("ID=%d\n", ID);
    ret = searchReg(ID, &numberOfSteps);

    printOutput(ret, numberOfSteps);

    return ret;
}

static int32_t showRemoveRegMenu(void)
{
    int32_t    ret = SUCCESS;
    uint32_t   ID = 0; 
    uint32_t   numberOfSteps = 0;

    cleanScreen();

    printf("\n                    ******* Informacion del Registro *******\n\n");
    printf("Por favor intruduce el ID del registro a eliminar\n");

    getRegInfo(&ID, NULL, NULL, NULL, NULL);
    
    UI_DEBUG("ID=%d\n", ID);
    ret = removeReg(ID, &numberOfSteps);

    printOutput(ret, numberOfSteps);

    return ret;
}

static int32_t showChangeRegMenu(void)
{
    int32_t    ret = SUCCESS;
    uint32_t   ID = 0; 
    uint32_t   numberOfSteps = 0;

    cleanScreen();

    printf("\n                    ******* Informacion del Registro *******\n\n");
    printf("Por favor intruduce el ID del registro a ser cambiado\n");

    getRegInfo(&ID, NULL, NULL, NULL, NULL);
    
    UI_DEBUG("ID=%d\n", ID);
    ret = changeReg(ID, &numberOfSteps);

    printOutput(ret, numberOfSteps);

    return ret;
}

static int32_t showTable(void)
{
    int32_t     ret = SUCCESS;
    uint32_t    optionSelected = 0;
    char        stdinBuf[STDIN_BUF_SIZE];

    if (g_tableView != NULL)
    {
        memset(&stdinBuf, 0, sizeof(char) * STDIN_BUF_SIZE); 

        do
        {
            cleanScreen();

            printf("\n                    ******* Mostrar Tabla *******\n\n");
            printf("Donde deseas que se muestre la tabla: \n\n");
            printf("1.- Mostrar en la Consola \n");
            printf("2.- Enviar a un archivo \n\n");
            printf("Opcion Seleccionada: ");
            fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
            optionSelected = atoi(stdinBuf);

            if (optionSelected < SEND_TO_CONSOLE || optionSelected > SEND_TO_FILE)
            {
                printf("\nOpcion invalida\n");
                getchar();
                cleanScreen();
            }
            else
            {
                g_tableView->output = optionSelected;
            }
        }while (optionSelected < SEND_TO_CONSOLE || optionSelected > SEND_TO_FILE);

        if (g_tableView->output == SEND_TO_FILE)
        {
            printf("Nombre del archivo de salida: "); 
            fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
            memcpy(g_tableView->outputFile, stdinBuf, sizeof(char)*(OUTPUT_FILE_SIZE - 1)); //-1 to keep the '\0' at the end
        }

        do
        {
            cleanScreen();

            printf("\n                    ******* Mostrar Tabla *******\n\n");
            printf("Como deseas que se muestre la tabla: \n\n");
            printf("1.- Completa \n");
            printf("2.- Resumida \n\n");
            printf("Opcion Seleccionada: ");
            fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
            optionSelected = atoi(stdinBuf);

            if (optionSelected < SHOW_FULL_TABLE || optionSelected > SHOW_SUMMARY_TABLE)
            {
                printf("\nOpcion invalida\n");
                getchar();
                cleanScreen();
            }
            else
            {
                g_tableView->view = optionSelected;
            }
        }while (optionSelected < SHOW_FULL_TABLE || optionSelected > SHOW_SUMMARY_TABLE);

        ret = displayTable();
    }
    else
    {
        UI_ERROR("Table view is null\n");
        ret = FAIL;
    }


    return ret;
}

static int32_t showReg(char *str)
{
    int32_t  ret = SUCCESS;
    FILE    *pFile = NULL;

    if (g_tableView != NULL
        && str != NULL)
    {
        UI_DEBUG("g_tableView->output = %d g_tableView->outputFile = %s\n", 
                 g_tableView->output, 
                 g_tableView->outputFile);

        if (g_tableView->output == SEND_TO_CONSOLE)
        {
            printf(str);
        }
        else if (g_tableView->output == SEND_TO_FILE)
        {
            pFile = fopen(g_tableView->outputFile, "a");

            if (pFile != NULL)
            {
                fprintf(pFile,str);
                fflush(pFile);
                fclose(pFile);
            }
            else
            {
                UI_ERROR("Output file %s could not be openned rc = %08lx\n", 
                         g_tableView->outputFile,
                         pFile);
                ret = FAIL;
            }
        }
        else
        {
            UI_ERROR("Table view output invalid\n");
            ret = FAIL;
        }
    }
    else
    {
        UI_ERROR("Table view is null or string is null\n");
        ret = FAIL;
    }

    return ret;
}

int32_t showRegInfo(uint32_t ID, char *pName, char *pTel, char *pAddress, char *pCity)
{
    int32_t ret = SUCCESS;
    char regBuf[REG_BUF_SIZE];

    if (pName != NULL
        && pTel != NULL
        && pAddress != NULL
        && pCity != NULL)
    {
        memset(regBuf, 0, sizeof(char)*REG_BUF_SIZE);

        if (g_tableView != NULL)
        {
            UI_DEBUG("g_tableView->view = %d\n", g_tableView->view);

            if (g_tableView->view == SHOW_FULL_TABLE)
            {
                sprintf(regBuf,
                        "ID:%d\nName: %sTel: %sAddress: %sCity: %s\n", 
                        ID, 
                        pName,
                        pTel,
                        pAddress,
                        pCity);
            }
            else if (g_tableView->view == SHOW_SUMMARY_TABLE)
            {
                sprintf(regBuf, "%d\t\t%s", ID, pName);
            }

            showReg(regBuf);
        }
        else
        {
            UI_ERROR("Table view is null\n");
            ret = FAIL;
        }
    }
    else
    {
        UI_ERROR("At least one parameter is null\n");
        ret = FAIL;
    }

    return ret; 
}
