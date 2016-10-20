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
#include "resources.h"

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
#define REG_BUF_SIZE          512

#define CHANGE_FIRST_NAME     1
#define CHANGE_LAST_NAME      2
#define CHANGE_ADDRESS        3
#define CHANGE_CITY           4
#define CHANGE_TEL1           5
#define CHANGE_TEL2           6
#define CHANGE_ALL            7

//Definition of static functions
static int32_t  showMainMenu(uint8_t *optionSelected);
static void getRegInfo(uint32_t * pID,
                       char * pFirstName,
                       char * pLastName,
                       char * pAddress,
                       char * pCity,
                       char * pTel1,
                       char * pTel2);
static int32_t showInsertRegMenu(void);
static int32_t showSearchRegMenu(void);
static int32_t showRemoveRegMenu(void);
static int32_t showChangeRegMenu(void);
static int32_t showTableView(void);
static int32_t printInfoInTableView(const char *str);

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
        g_tableView = (TableView *)MEMALLOC(sizeof(TableView));

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
    bool repeat = false;
        
    createTableView();

    do
    {
        repeat = false;
        
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
            //waiting for a key input
            getchar();
            repeat = true;
        }
    }while (repeat);
    
    destroyTableView();

    return ret;
}

static int32_t showMainMenu(uint8_t *optionSelected)
{
    int32_t ret = SUCCESS;
    bool    isValidOption = true;

    if (optionSelected != NULL)
    {
        *optionSelected = createMenuWithMultipleOptions(STR_HASH_TAB_TITLE,
                                                        STR_SELECT_OPTION,
                                                        STR_MAIN_MENU_OPTIONS,
                                                        STR_OPTION_SELECTED,
                                                        true,
                                                        INSERT_REG,
                                                        EXIT,
                                                        true);

        UI_DEBUG("optionSelected=%i\n", *optionSelected);
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
        case SUCCESS:         printf(STR_SUCCESS);
            break;
        case FAIL:            printf(STR_FAIL);
            break;
        case REG_NOT_FOUND:   printf(STR_REGISTER_NOT_FOUND);
            break;
        case REG_DUPLICATED:  printf(STR_REGISTER_DUPLICATED);
            break;
        default:
            break;
    }

    printf(STR_NUMBER_OF_STEPS, numberOfSteps);
}

static void getRegInfo(uint32_t * pID,
                       char * pFirstName,
                       char * pLastName,
                       char * pAddress,
                       char * pCity,
                       char * pTel1,
                       char * pTel2)
{
    if (pID != NULL)
    {
        *pID = getUInt32FromConsole(STR_ID); 
    }
    
    if (pFirstName != NULL)
    {
        getStringFromConsole(STR_FIRST_NAME, pFirstName, (FIRST_NAME_SIZE - 1)); 
    }

    if (pLastName != NULL)
    {
        getStringFromConsole(STR_LAST_NAME, pLastName, (LAST_NAME_SIZE - 1)); 
    }
    
    if (pAddress != NULL)
    {
        getStringFromConsole(STR_ADDRESS, pAddress, (ADD_SIZE - 1));
    }
    
    if (pCity != NULL)
    {
        getStringFromConsole(STR_CITY, pCity, (CITY_SIZE - 1)); 
    }

    if (pTel1 != NULL)
    {
        getStringFromConsole(STR_TEL_1, pTel1, (TEL_SIZE - 1));
    }

    if (pTel2 != NULL)
    {
        getStringFromConsole(STR_TEL_2, pTel2, (TEL_SIZE - 1));
    }
}

static int32_t showInsertRegMenu(void)
{
    int32_t    ret = SUCCESS;
    uint32_t   ID = 0; 
    uint32_t   numberOfSteps = 0;
    char       first_name[FIRST_NAME_SIZE];
    char       last_name[LAST_NAME_SIZE];
    char       address[ADD_SIZE];
    char       city[CITY_SIZE];
    char       tel1[TEL_SIZE];
    char       tel2[TEL_SIZE];

    memset(&first_name, 0, sizeof(char)*FIRST_NAME_SIZE);
    memset(&last_name, 0, sizeof(char)*LAST_NAME_SIZE);
    memset(&address, 0, sizeof(char)*ADD_SIZE);
    memset(&city, 0, sizeof(char)*CITY_SIZE);
    memset(&tel1, 0, sizeof(char)*TEL_SIZE);
    memset(&tel2, 0, sizeof(char)*TEL_SIZE);

    cleanScreen();

    printf(STR_REG_INFO);
    printf(STR_INSERT_REG);
    
    getRegInfo(&ID, first_name, last_name, address, city, tel1, tel2);
    
    UI_DEBUG(" Insert register:\nID: %d \nfirst name: %s\nlast name: %s\naddress: %s\ncity: %s\ntel 1: %s\ntel 2: %s\n",
             ID, 
             first_name, 
             last_name, 
             address, 
             city, 
             tel1, 
             tel2);
  
    ret = insertReg(ID, first_name, last_name, address, city, tel1, tel2, &numberOfSteps);
  
    printOutput(ret, numberOfSteps);

    return ret;
}

static int32_t showSearchRegMenu(void)
{
    int32_t    ret = SUCCESS;
    uint32_t   ID = 0; 
    uint32_t   numberOfSteps = 0;
   
    cleanScreen();

    printf(STR_REG_INFO);
    printf(STR_SEARCH_REG);

    getRegInfo(&ID, NULL, NULL, NULL, NULL, NULL, NULL);
    
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

    printf(STR_REG_INFO);
    printf(STR_REMOVE_REG);

    getRegInfo(&ID, NULL, NULL, NULL, NULL, NULL, NULL);
    
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

    printf(STR_REG_INFO);
    printf(STR_CHANGE_REG);

    getRegInfo(&ID, NULL, NULL, NULL, NULL, NULL, NULL);
    
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
        optionSelected = createMenuWithMultipleOptions(STR_SHOW_TAB_TITLE,
                                                       STR_SHOW_TAB_OUTPUT_HEADER,
                                                       STR_SHOW_TAB_OUTPUT_OPTIONS,
                                                       STR_OPTION_SELECTED,
                                                       true,
                                                       SEND_TO_CONSOLE,
                                                       SEND_TO_FILE,
                                                       true);

        g_tableView->output = optionSelected;
        UI_DEBUG("Table output option = %d\n", g_tableView->output);

        if (g_tableView->output == SEND_TO_FILE)
        {
            getStringFromConsole(STR_OUTPUT_FILE,
                                 g_tableView->outputFile,
                                 (OUTPUT_FILE_SIZE - 1)); 
        }

        optionSelected = createMenuWithMultipleOptions(STR_SHOW_TAB_TITLE,
                                                       STR_SHOW_TAB_VIEW_HEADER,
                                                       STR_SHOW_TAB_VIEW_OPTIONS,
                                                       STR_OPTION_SELECTED,
                                                       true,
                                                       SHOW_FULL_TABLE,
                                                       SHOW_SUMMARY_TABLE,
                                                       true);

        g_tableView->view = optionSelected;
        UI_DEBUG("Table view option = %d\n", g_tableView->output);

        if (g_tableView->output == SEND_TO_CONSOLE)
        {
            cleanScreen(); 
        }

        if (g_tableView->view == SHOW_SUMMARY_TABLE)
        {
            ret = printInfoInTableView(STR_SUMMARY_FORMAT_HEADER);
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

static int32_t printInfoInTableView(const char *str)
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
                    char * pFirstName,
                    char * pLastName,
                    char * pAddress,
                    char * pCity,
                    char * pTel1,
                    char * pTel2)
{
    int32_t ret = SUCCESS;
    char regBuf[REG_BUF_SIZE];

    if (pFirstName != NULL
        && pLastName != NULL
        && pAddress != NULL
        && pCity != NULL
        && pTel1 != NULL
        && pTel2 != NULL)
    {
        memset(regBuf, 0, sizeof(char)*REG_BUF_SIZE);

        if (g_tableView != NULL)
        {
            UI_DEBUG("g_tableView->view = %d\n", g_tableView->view);

            if (g_tableView->view == SHOW_FULL_TABLE)
            {
                sprintf(regBuf,
                        STR_OUTPUT_FULL_FORMAT,
                        hashValue,
                        treeLevel,
                        ID,
                        pFirstName,
                        pLastName,
                        pAddress,
                        pCity,
                        pTel1,
                        pTel2);
            }
            else if (g_tableView->view == SHOW_SUMMARY_TABLE)
            {
                sprintf(regBuf, STR_OUTPUT_SUMMARY_FORMAT, hashValue, treeLevel, ID, pFirstName, pLastName);
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

int32_t changeFieldsOfReg(char * pFirstName,
                          char * pLastName,
                          char * pAddress,
                          char * pCity,
                          char * pTel1,
                          char * pTel2)
{
    int32_t ret = SUCCESS;
    uint8_t optionSelected = 0;
    bool    isValidOption = true;
    bool    repeat = false;

    if (pFirstName != NULL
        && pLastName != NULL
        && pAddress != NULL
        && pCity != NULL
        && pTel1 != NULL
        && pTel2 != NULL)
    {
        UI_DEBUG("Original fields: first name:%s last name:%s address:%s city:%s tel #1:%s tel #2:%s",
                 pFirstName,
                 pLastName,
                 pAddress,
                 pCity,
                 pTel1,
                 pTel2);

        do
        {
            repeat = false;

            optionSelected = createMenuWithMultipleOptions(NULL,
                                                           STR_CHANGE_REG_HEADER,
                                                           STR_CHANGE_REG_OPTIONS,
                                                           STR_OPTION_SELECTED,
                                                           true,
                                                           CHANGE_FIRST_NAME,
                                                           CHANGE_ALL,
                                                           false);

            UI_DEBUG("optionSelected=%i\n", optionSelected);

            printf(STR_INSERT_NEW_VALUE);

            switch (optionSelected)
            {
                case CHANGE_FIRST_NAME: getRegInfo(NULL, pFirstName, NULL, NULL, NULL, NULL, NULL);
                    break;
                case CHANGE_LAST_NAME:  getRegInfo(NULL, NULL, pLastName, NULL, NULL, NULL, NULL);
                    break;
                case CHANGE_ADDRESS:    getRegInfo(NULL, NULL, NULL, pAddress, NULL, NULL, NULL);
                    break;
                case CHANGE_CITY:       getRegInfo(NULL, NULL, NULL, NULL, pCity, NULL, NULL);
                    break;
                case CHANGE_TEL1:       getRegInfo(NULL, NULL, NULL, NULL, NULL, pTel1, NULL);
                    break;
                case CHANGE_TEL2:       getRegInfo(NULL, NULL, NULL, NULL, NULL, NULL, pTel2);
                    break;
                case CHANGE_ALL:        getRegInfo(NULL, pFirstName, pLastName, pAddress, pCity, pTel1, pTel2);
                    break;
                default: break;
            }

            repeat = repeatAction(STR_REPEAT_CHANGE_REG);
        }
        while (repeat);

        UI_DEBUG("New fields: first name:%s last name:%s address:%s city:%s tel #1:%s tel #2:%s",
                 pFirstName,
                 pLastName,
                 pAddress,
                 pCity,
                 pTel1,
                 pTel2);
    }
    else
    {
        UI_ERROR("At least one parameter is null\n");
        ret = FAIL;
    }

    return ret;
}
