// Programacionl.Proyecto2.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <stdio.h>
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

int numeroPersona = 0;

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
		numeroPersona++;
		printf_s("\t\033[32m| %-*s | %-*s |\033[0m\n"
			, extension.encabezado8_len
			, row[0]
			, extension.encabezado9_len
			, row[1]);
	}

	mysql_free_result(result);

	CerrarConexion();
}

bool ObtenerIngresoEgreso(short int pTipoRegistro, int pIdPersona)
{
	bool estadoObtenerIngreso = false;
	if (!AbrirConexion())
	{
		return estadoObtenerIngreso;
	};

	char consulta[100];
	sprintf_s(consulta, "CALL Sp_ObtenerIngresoEgreso(%d, %d)", pTipoRegistro, pIdPersona);

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

void MostrarMenu()
{
	int opcion = 0;
	char entrada[100];
	int personaId = 0;

	do
	{
		system("cls");

		printf("=== PROGRAMACION l - PROYECTO ll ===\n");
		printf("\t1. VER INGRESOS\n");
		printf("\t2. VER EGRESOS\n");
		printf("\t3. SALIR\n");

		scanf_s("%d", &opcion);

		switch (opcion) {
		case 1:
			ObtenerPersona();
			printf_s("\nINGRESE UN NUMERO DE PERSONA: ");

			scanf_s("%d", &personaId);

			if (scanf_s("%d", &personaId) != 0)
			{
				scanf_s("%s", entrada, sizeof(entrada));
				printf_s("\033[31mERROR:SE INGRESO UNA LETRA EN LUGAR DE UN ENTERO\033[0m\n");
				system("pause>nul");
				continue;
			}

			if (personaId <= 0)
			{
				printf_s("\033[31mEL NUMERO DE PERSONA QUE INGRESO NO ES VALIDO\033[0m\n");
				system("pause>nul");
				continue;
			}

			if (personaId > numeroPersona)
			{
				printf_s("\033[31mEL NUMERO DE PERSONA QUE INGRESO NO EXISTE EN EL LISTADO MOSTRADO\033[0m\n");
				system("pause>nul");
				continue;
			}

			if (!ObtenerIngresoEgreso(1, personaId))
			{
				system("pause>nul");
				continue;
			};

			ObtenerTotalIngreso(1, personaId);

			system("pause>nul");
			break;
		case 2:
			ObtenerPersona();
			printf_s("\nINGRESE UN NUMERO DE PERSONA: ");

			scanf_s("%d", &personaId);

			if (scanf_s("%s", entrada, sizeof(entrada)))
			{
				printf_s("\033[31mERROR:SE INGRESO UNA LETRA EN LUGAR DE UN ENTERO\033[0m\n");
				system("pause>nul");
				continue;
			}

			if (personaId <= 0)
			{
				printf_s("\033[31mERROR:EL NUMERO DE PERSONA QUE INGRESO NO ES VALIDO\033[0m\n");
				system("pause>nul");
				continue;
			}

			if (personaId > numeroPersona)
			{
				printf_s("\033[31mEL NUMERO DE PERSONA QUE INGRESO NO EXISTE EN EL LISTADO MOSTRADO\033[0m\n");
				system("pause>nul");
				continue;
			}

			if (!ObtenerIngresoEgreso(2, personaId))
			{
				system("pause>nul");
				continue;
			};

			ObtenerTotalIngreso(2, personaId);

			system("pause>nul");
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