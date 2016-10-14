/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Main program
 */
 
#include <stdio.h>

#include "hash_table.h"
#include "trace.h"
#include "UI.h"

int32_t main(void)
{
    int32_t ret = SUCCESS;

    initTrace();

    MAIN_DEBUG("\n************************** Program started ******************************\n\n");

    ret = createTable();
    
    if (ret == SUCCESS)
    {
        ret = showUI();
        destroyTable();
    }
    else
    {
        MAIN_ERROR("Hash table could not be created\n");
    }
    
    MAIN_DEBUG("\n******************* Program ended with rc= %d ***************************\n\n", ret);
    
    termTrace();
    
    return ret;
}

