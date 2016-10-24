/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all strings used in the User interface
 */

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

#define STR_HASH_TAB_TITLE           "\n                    ******* TABLA HASH *******\n\n"
#define STR_SELECT_OPTION            "Selecciona la opcion deseada:\n\n"
#define STR_MAIN_MENU_OPTIONS        "1.- Insertar un registro\n" \
                                     "2.- Buscar un registro\n"   \
                                     "3.- Borrar un registro\n"   \
                                     "4.- Cambiar un registro\n"  \
                                     "5.- Mostrar la tabla\n"     \
                                     "6.- Salir\n\n"
#define STR_OPTION_SELECTED          "Opcion Seleccionada: "
#define STR_SUCCESS                  "\nOperacion exitosa!!!\n"
#define STR_FAIL                     "\nHubo un ERROR mientras se ejecutaba el programa\n"
#define STR_REGISTER_NOT_FOUND       "\nEl registro no fue encontrado\n"
#define STR_REGISTER_DUPLICATED      "\nEl registro esta duplicado\n"
#define STR_NUMBER_OF_STEPS          "\nNumero de pasos que tomo realizar la accion = %d\n\n"
#define STR_ID                       "ID: "
#define STR_FIRST_NAME               "Nombre(s): "
#define STR_LAST_NAME                "Apellido(s): "
#define STR_ADDRESS                  "Direccion: "
#define STR_CITY                     "Ciudad: "
#define STR_TEL_1                    "Telefono #1: "
#define STR_TEL_2                    "Telefono #2: "
#define STR_REG_INFO                 "\n                    ******* Informacion del Registro *******\n\n"
#define STR_INSERT_REG               "Por favor intruduce los datos del registro\n"
#define STR_SEARCH_REG               "Por favor intruduce el ID del registro a buscar\n"
#define STR_REMOVE_REG               "Por favor intruduce el ID del registro a eliminar\n"
#define STR_CHANGE_REG               "Por favor intruduce el ID del registro a ser cambiado\n"
#define STR_SHOW_TAB_TITLE           "\n                    ******* Mostrar Tabla *******\n\n"
#define STR_SHOW_TAB_OUTPUT_HEADER   "Donde deseas que se muestre la tabla: \n\n"
#define STR_SHOW_TAB_OUTPUT_OPTIONS  "1.- Mostrar en la Consola \n" \
                                     "2.- Enviar a un archivo \n\n"
#define STR_OUTPUT_FILE              "Nombre del archivo de salida: "
#define STR_SHOW_TAB_VIEW_HEADER     "Como deseas que se muestre la tabla: \n\n"
#define STR_SHOW_TAB_VIEW_OPTIONS    "1.- Completa \n"   \
                                     "2.- Resumida \n\n"
#define STR_SUMMARY_FORMAT_HEADER    "Indice\tID\tNivel\tPapa\tLado\tNombre\n\n"
#define STR_OUTPUT_FULL_FORMAT       "\nIndice #%d\t ID:%d\n"  \
                                     "Nivel en el arbol: %d\t ID del padre: %d\t lado: %s\n" \
                                     "Nombre(s): %s\n"                         \
                                     "Apellido(s): %s\n"                       \
                                     "Direccion: %s\n"                         \
                                     "Ciudad: %s\n"                            \
                                     "Tel #1: %s\n"                            \
                                     "Tel #2: %s\n\n"
#define STR_OUTPUT_SUMMARY_FORMAT    "%d\t%d\t%d\t%d\t%s\t%s %s\n"
#define STR_CHANGE_REG_HEADER        "\nSelecciona el campo que quieres modificar:\n\n"
#define STR_CHANGE_REG_OPTIONS       "1.- Nombre(s)\n"   \
                                     "2.- Apellido(s)\n" \
                                     "3.- Direccion\n"   \
                                     "4.- Ciudad\n"      \
                                     "5.- Telefono #1\n" \
                                     "6.- Telefono #2\n" \
                                     "7.- Cambiar todos los campos\n\n"
#define STR_INSERT_NEW_VALUE         "\nEscribe el nuevo valor del\n"
#define STR_REPEAT_CHANGE_REG        "\nDeseas cambiar otro campo [s,n]: "
#define STR_LEFT                     "Izq"
#define STR_RIGHT                    "Der"

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif
