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
#include "console_utils.h"

//Constanst
#define INSERT_REG            1
#define SEARCH_REG            2
#define REMOVE_REG            3
#define CHANGE_REG            4
#define SHOW_TABLE            5
#define EXIT                  6

#define SHOW_FULL_TABLE       1
#define SHOW_SUMMARY_TABLE    2
#define SEND_TO_CONSOLE       1
#define SEND_TO_FILE          2

#define OUTPUT_FILE_SIZE      20
#define REG_BUF_SIZE          256

#define CHANGE_NAME           1
#define CHANGE_TEL            2
#define CHANGE_ADDRESS        3
#define CHANGE_CITY           4
#define CHANGE_ALL            5

//Definition of static functions
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
static int32_t showTableView(void);
static int32_t printInfoInTableView(char *str);

//Type definitions
typedef struct _TableView
{
    uint32_t    view;
    uint32_t    output;
    char        outputFile[OUTPUT_FILE_SIZE];
}TableView;

//global variables
TableView * g_tableView = NULL;

static void setDefaultValuesOfTableView()
{
    if (g_tableView != NULL)
    {
        g_tableView->view = SHOW_FULL_TABLE;
        g_tableView->output = SEND_TO_CONSOLE;
    }
}

static void createTableView(void)
{
    if (g_tableView == NULL)
    {
        g_tableView = MEMALLOC(sizeof(TableView));

        if (g_tableView != NULL)
        {
            setDefaultValuesOfTableView();
        }
        else
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
            case SHOW_TABLE: ret = showTableView();
                break;
            default: break;
        }
        
        if (optionSelected != EXIT)
        {
            char repeat = 0;

            repeat = getFirstCharFromConsole("\nDeseas realizar otra operacion [s,n]: ");

            if (repeat == 's' || repeat == 'S')
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
    bool    isValidOption = true;

    if (optionSelected != NULL)
    {
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

          *optionSelected = getUint8FromConsole("Opcion Seleccionada: ");
          
          UI_DEBUG("optionSelected=%i\n", *optionSelected);

          isValidOption = validateIntInput(*optionSelected, INSERT_REG, EXIT, true);

        }while (isValidOption == false);
    }
    else
    {
        UI_ERROR("optionSelected is null");
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
    if (pID != NULL)
    {
        *pID = getUInt32FromConsole("ID: "); 
    }
    
    if (pName != NULL)
    {
        getStringFromConsole("Nombre: ", pName, (NAME_SIZE - 1)); 
    }
    
    if (pTel != NULL)
    {
        getStringFromConsole("Telefono: ", pTel, (TEL_SIZE - 1));
    }
    
    if (pAddress != NULL)
    {
        getStringFromConsole("Direccion: ", pAddress, (ADD_SIZE - 1));
    }
    
    if (pCity != NULL)
    {
        getStringFromConsole("Ciudad: ", pCity, (CITY_SIZE - 1)); 
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
    
    UI_DEBUG(" Insert register:\nID: %d \nname: %s\ntel: %s\naddress: %s\ncity: %s\n",
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

static int32_t showTableView(void)
{
    int32_t     ret = SUCCESS;
    uint32_t    optionSelected = 0;
    bool        isValidOption = true;

    if (g_tableView != NULL)
    {
        do
        {
            cleanScreen();

            printf("\n                    ******* Mostrar Tabla *******\n\n");
            printf("Donde deseas que se muestre la tabla: \n\n");
            printf("1.- Mostrar en la Consola \n");
            printf("2.- Enviar a un archivo \n\n");

            optionSelected = getUint8FromConsole("Opcion Seleccionada: ");
            isValidOption = validateIntInput(optionSelected, SEND_TO_CONSOLE, SEND_TO_FILE, true);

            if (isValidOption)
            {
                g_tableView->output = optionSelected;
                UI_DEBUG("Table output option = %d\n", g_tableView->output);
            }
        }while (isValidOption == false);

        if (g_tableView->output == SEND_TO_FILE)
        {
            getStringFromConsole("Nombre del archivo de salida: ",
                                 g_tableView->outputFile,
                                 (OUTPUT_FILE_SIZE - 1)); 
        }

        do
        {
            cleanScreen();

            printf("\n                    ******* Mostrar Tabla *******\n\n");
            printf("Como deseas que se muestre la tabla: \n\n");
            printf("1.- Completa \n");
            printf("2.- Resumida \n\n");

            optionSelected = getUint8FromConsole("Opcion Seleccionada: ");
            isValidOption = validateIntInput(optionSelected, SHOW_FULL_TABLE, SHOW_SUMMARY_TABLE, true);

            if (isValidOption)
            {
                g_tableView->view = optionSelected;
                UI_DEBUG("Table view option = %d\n", g_tableView->output);
            }
        }while (isValidOption == false);

        if (g_tableView->output == SEND_TO_CONSOLE)
        {
            cleanScreen(); 
        }

        if (g_tableView->view == SHOW_SUMMARY_TABLE)
        {
            ret = printInfoInTableView("Indice\tNivel\tID\tNombre\n\n");
        }

        if (ret == SUCCESS)
        {
            ret = displayTable(); 
            setDefaultValuesOfTableView();
        }
    }
    else
    {
        UI_ERROR("Table view is null\n");
        ret = FAIL;
    }


    return ret;
}

static int32_t printInfoInTableView(char *str)
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

int32_t showRegInfo(uint32_t hashValue,
                    uint32_t treeLevel,
                    uint32_t ID,
                    char *pName, 
                    char *pTel, 
                    char *pAddress, 
                    char *pCity)
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
                        "\nIndice #%d\t Nivel en el arbol: %d\nID:%d\nNombre: %s\nTel: %s\nDireccion: %s\nCiudad: %s\n\n",
                        hashValue,
                        treeLevel,
                        ID, 
                        pName,
                        pTel,
                        pAddress,
                        pCity);
            }
            else if (g_tableView->view == SHOW_SUMMARY_TABLE)
            {
                sprintf(regBuf, "%d\t%d\t%d\t%s\n", hashValue, treeLevel, ID, pName);
            }

            printInfoInTableView(regBuf);
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

int32_t changeFieldsOfReg(char *pName,
                          char *pTel,
                          char *pAddress,
                          char *pCity)
{
    int32_t ret = SUCCESS;
    uint8_t optionSelected = 0;
    bool    isValidOption = true;

    if (pName != NULL
        && pTel != NULL
        && pAddress != NULL
        && pCity != NULL)
    {
        UI_DEBUG("Original fields: name:%s tel:%s address:%s city:%s",
                 pName,
                 pTel,
                 pAddress,
                 pCity);

        do 
        {
          printf("\nSelecciona el campo que quieres modificar:\n\n");
          printf("1.- Nombre\n");
          printf("2.- Telefono\n");
          printf("3.- Direccion\n");
          printf("4.- Ciudad\n");

          optionSelected = getUint8FromConsole("\nOpcion Seleccionada: ");
          
          UI_DEBUG("optionSelected=%i\n", optionSelected);

          isValidOption = validateIntInput(optionSelected, CHANGE_NAME, CHANGE_CITY, true);

        }while (isValidOption == false);

        printf("\nEscribe el nuevo valor del\n");

        switch (optionSelected)
        {
            case CHANGE_NAME:    getRegInfo(NULL, pName, NULL, NULL, NULL);
                break;
            case CHANGE_TEL:     getRegInfo(NULL, NULL, pTel, NULL, NULL);
                break;
            case CHANGE_ADDRESS: getRegInfo(NULL, NULL, NULL, pAddress, NULL);
                break;
            case CHANGE_CITY:    getRegInfo(NULL, NULL, NULL, NULL, pCity);
                break;
            case CHANGE_ALL:     getRegInfo(NULL, pName, pTel, pAddress, pCity);
                break;
            default: break;
        }

        printf("\n");

        UI_DEBUG("New fields: name:%s tel:%s address:%s city:%s",
                 pName,
                 pTel,
                 pAddress,
                 pCity);
    }
    else
    {
        UI_ERROR("At least one parameter is null\n");
        ret = FAIL;
    }

    return ret;
}
