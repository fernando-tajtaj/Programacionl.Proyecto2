#pragma once
#include <stdio.h>
#include <string.h>

using namespace std;
class Extension
{
private:
	const char* encabezado0 = "NUMERO DE REGISTRO";
	const char* encabezado1 = "TIPO REGISTRO";
	const char* encabezado2 = "MONTO MENSUAL";
	const char* encabezado3 = "% IMPUESTO";
	const char* encabezado4 = "MONTO IMPUESTO";
	const char* encabezado5 = "MONTO CON IMPUESTO";
	const char* encabezado6 = "FECHA REGISTRO";
	const char* encabezado7 = "NOMBRE COMPLEO CLIENTE";

	const char* encabezado8 = "NUMERO PERSONA";
	const char* encabezado9 = "NOMBRE COMPLETO PERSONA";

	// Se suma 6 por los espacios y las barras verticales
	int row_len = encabezado0_len + encabezado1_len + encabezado2_len + encabezado3_len + encabezado3_len + encabezado4_len + encabezado5_len + encabezado6_len + encabezado7_len + 6;
public:
	int encabezado0_len = strlen(encabezado0);
	int encabezado1_len = strlen(encabezado1);
	int encabezado2_len = strlen(encabezado2);
	int encabezado3_len = strlen(encabezado3);
	int encabezado4_len = strlen(encabezado4);
	int encabezado5_len = strlen(encabezado5);
	int encabezado6_len = strlen(encabezado6);
	int encabezado7_len = strlen(encabezado7);
	int encabezado8_len = strlen(encabezado8);
	int encabezado9_len = strlen(encabezado9);

	void Encabezado()
	{
		printf_s("\033[32mMESES CORRECTOS\033[0m\n");

		printf_s("\t\033[32m| %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s |\033[0m\n"
			, encabezado0_len / 2 + encabezado0_len % 2
			, encabezado0
			, encabezado1_len / 2 + encabezado1_len % 2
			, encabezado1
			, encabezado2_len / 2 + encabezado2_len % 2
			, encabezado2
			, encabezado3_len / 2 + encabezado3_len % 2
			, encabezado3
			, encabezado3_len / 2 + encabezado3_len % 2
			, encabezado4
			, encabezado4_len / 2 + encabezado4_len % 2
			, encabezado5
			, encabezado5_len / 2 + encabezado5_len % 2
			, encabezado6
			, encabezado7_len / 2 + encabezado7_len % 2
			, encabezado7);

	}

	void EncabezadoPersona()
	{
		printf_s("\033[32mPERSONAS REGISTRADAS\033[0m\n");
		printf_s("\t\033[32m| %*s | %*s |\033[0m\n"
			, encabezado8_len / 2 + encabezado8_len % 2
			, encabezado8
			, encabezado9_len / 2 + encabezado9_len % 2
			, encabezado9);
	}
};


