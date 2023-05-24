// Programacionl.Proyecto2.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string>
#include <mysql.h>
#include "Extension.h"

using namespace std;

const char* host = "127.0.0.1";
const char* usuario = "root";
const char* contrasenia = "";
const char* nombreBaseDatos = "ingresosegresos";
unsigned int puerto = 3306;

MYSQL* mysqlConexion;
Extension extension;

bool AbrirConexion()
{
	bool estadoConexion = false;
	mysqlConexion = mysql_init(NULL);

	if (!mysql_real_connect(mysqlConexion, host, usuario, contrasenia, nombreBaseDatos, puerto, NULL, 0))
	{
		printf_s("OCURRIO UN ERROR AL INTENTAR ABRIR LA CONEXION, VERIFICAR SI EL GESTOR DE LA BASE DE DATOS ESTA EJECUTANDOSE.\n");

		estadoConexion = false;
		return estadoConexion;
	}

	estadoConexion = true;
	return estadoConexion;
}

void CerrarConexion()
{
	mysql_close(mysqlConexion);
}

void ObtenerPersona()
{
	if (!AbrirConexion())
	{
		return;
	}

	if (mysql_query(mysqlConexion, "SELECT PersonaId, UPPER(PersonaNombre) FROM Persona"))
	{
		printf_s("OCURRIO UN ERROR AL INTENTAR CONSULTAR LA TABLA DE PERSONAS");
		return;
	}

	MYSQL_RES* result = mysql_store_result(mysqlConexion);
	if (result == NULL || result->row_count <= 0)
	{
		printf_s("\033[31mNO TIENE PERSONAS AGREGADAS EN LA BASE DE DATOS\033[0m\n");
		return;
	}

	extension.EncabezadoPersona();
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{
		printf_s("\t\033[32m| %-*s | %-*s |\033[0m\n"
			, extension.encabezado8_len
			, row[0]
			, extension.encabezado9_len
			, row[1]);
	}

	mysql_free_result(result);

	CerrarConexion();
}

bool ObtenerIngreso(int pIdPersona)
{
	bool estadoObtenerIngreso = false;
	if (!AbrirConexion())
	{
		return estadoObtenerIngreso;
	};

	short int pTipoRegistro = 1;
	char consulta[100];
	sprintf_s(consulta, "CALL Sp_ObtenerIngresos(%d, %d)", pTipoRegistro, pIdPersona);

	if (mysql_query(mysqlConexion, consulta))
	{
		printf_s("\033[31mOCURRIO UN ERROR AL EJECUTAR EL PROCEDIMIENTO QUE OBTIENE INGRESOS, ASEGURESE DE QUE EXISTA EN BASE DE DATOS\033[0m\n");
		return estadoObtenerIngreso;
	}

	MYSQL_RES* result = mysql_store_result(mysqlConexion);
	if (result == NULL || result->row_count <= 0)
	{
		printf_s("\033[31mNO TIENE INGRESOS AGREGADOS EN LA BASE DE DATOS O EL NUMERO DE PERSONA ES INCORRECTO\033[0m\n");
		return estadoObtenerIngreso;
	}

	MYSQL_ROW row;
	extension.Encabezado();
	while ((row = mysql_fetch_row(result)))
	{
		printf_s("\t\033[32m| %-*s | %-*s | %-*s | %-*s | %-*s | %-*s | %-*s | %-*s |\033[0m\n"
			, extension.encabezado0_len
			, row[0]
			, extension.encabezado1_len
			, row[1]
			, extension.encabezado2_len
			, row[2]
			, extension.encabezado3_len
			, row[3]
			, extension.encabezado4_len
			, row[4]
			, extension.encabezado5_len
			, row[5]
			, extension.encabezado6_len
			, row[6]
			, extension.encabezado7_len
			, row[7]);
	}

	estadoObtenerIngreso = true;

	mysql_free_result(result);

	CerrarConexion();

	return estadoObtenerIngreso;
}

bool ObtenerTotalIngreso(int pTipoRegistro, int pIdPersona) 
{
	bool estadoObtenerIngreso = false;
	if (!AbrirConexion())
	{
		return estadoObtenerIngreso;
	};

	char consulta[100];
	sprintf_s(consulta, "CALL Sp_TotalIngresosEgresos(%d, %d)", pTipoRegistro, pIdPersona);

	if (mysql_query(mysqlConexion, consulta))
	{
		printf_s("\033[31mOCURRIO UN ERROR AL EJECUTAR EL PROCEDIMIENTO QUE OBTIENE INGRESOS, ASEGURESE DE QUE EXISTA EN BASE DE DATOS\033[0m\n");
		return estadoObtenerIngreso;
	}

	MYSQL_RES* result = mysql_store_result(mysqlConexion);
	if (result == NULL || result->row_count <= 0)
	{
		printf_s("\033[31mNO TIENE INGRESOS AGREGADOS EN LA BASE DE DATOS O EL NUMERO DE PERSONA ES INCORRECTO\033[0m\n");
		return estadoObtenerIngreso;
	}

	MYSQL_ROW row;

	while ((row = mysql_fetch_row(result)))
	{
		printf_s("\n\t\033[32mTOTAL: %s\033[0m\n", row[0]);
	}

	mysql_free_result(result);

	CerrarConexion();
	
}

bool EliminarIngreso(int pIngresoEgresoId)
{
	bool estadoEliminarIngreso = false;
	if (!AbrirConexion())
	{
		return estadoEliminarIngreso;
	};

	char consulta[100];
	sprintf_s(consulta, "CALL Sp_EliminarIngreso(%d)", pIngresoEgresoId);

	if (mysql_query(mysqlConexion, consulta))
	{
		printf_s("\033[31mOCURRIO UN ERROR AL EJECUTAR EL PROCEDIMIENTO, ASEGURESE DE QUE EXISTA EN BASE DE DATOS\033[0m\n");
		return estadoEliminarIngreso;
	}

	estadoEliminarIngreso = true;

	CerrarConexion();

	return estadoEliminarIngreso;
}

void MostrarSubMenu()
{
	int opcion;

	do
	{
		int numeroIngreso = 0;
		int c = 0;

		printf("Seleccione una opcion:\n");
		printf("1. Eliminar Ingreso por Id\n");
		printf("2. Salir\n");

		scanf_s("%d", &opcion);

		switch (opcion)
		{
		case 1:

			printf_s("INGRESA EL NUMERO DE REGISTRO A ELIMINAR: ");
			scanf_s("%d", &numeroIngreso);

			if (!EliminarIngreso(numeroIngreso))
			{
				printf_s("\nNO SE PUDO ELIMINAR EL NUMERO DE REGISTRO: %d", numeroIngreso);
			}

			printf_s("\nEL REGISTRO: %d SE HA ELIMINADO CORRECTAMENTE", numeroIngreso);

			system("pause>nul");
			printf_s("\nPRESIONE ENTER PARA REFRESCAR\n");
			system("pause>nul");

			c = getchar();

			if (c == '\n')
			{
				//ObtenerIngreso();
			}
			else
			{
				opcion = 2;
			}

			break;
		case 2:
			printf("Ha seleccionado salir.\n");
			break;
		}

	} while (opcion != 2);
}

void MostrarMenu()
{
	int opcion = 0;
	int personaId = 0;
	do
	{
		system("cls");
		
		printf("Seleccione una opcion:\n");
		printf("1. Ver Ingresos Por Persona\n");
		printf("2. Ver Egresos Por Persona\n");
		printf("3. Salir\n");

		scanf_s("%d", &opcion);

		switch (opcion) {
		case 1:
			ObtenerPersona();
			printf_s("\nINGRESE UN NUMERO DE PERSONA: ");

			scanf_s("%d", &personaId);

			if (personaId == 0)
			{
				printf_s("\nEL NUMERO DE PERSONA QUE INGRESO NO ES VALIDO\n");
				system("pause>nul");
				continue;
			}
			else
			{
				ObtenerIngreso(personaId);
				ObtenerTotalIngreso(1, personaId);
			}

			system("pause>nul");
			break;
		case 2:
			ObtenerPersona();
			break;
		case 3:
			printf("Ha seleccionado salir.\n");
			break;
		default:
			printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
			break;
		}
	} while (opcion != 3);
}

int main()
{
	MostrarMenu();
	system("pause");
	return 0;
}