-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 27-05-2023 a las 06:29:33
-- Versión del servidor: 10.4.28-MariaDB
-- Versión de PHP: 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `ingresosegresos`
--

DELIMITER $$
--
-- Procedimientos
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `Sp_EliminarIngreso` (IN `pIngresoEgreso` INT)   DELETE
FROM ingresoegreso
WHERE IngresoEgresoId = pIngresoEgreso$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `Sp_ObtenerIngresoEgreso` (IN `pTipoRegistro` SMALLINT, IN `pIdPersona` INT)   SELECT
  	a.IngresoEgresoId AS IngresoEgresoNo
    , CASE 
    	WHEN a.IngresoEgresoTipo = 1 THEN 'INGRESO'
        WHEN a.IngresoEgresoTipo = 2 THEN 'EGRESO'
       END IngresoEgresoTipo
    , a.IngresoEgresoMonto AS IngresoEgresoMonto
    , CASE 
    	WHEN a.IngresoEgresoTipo = 1 THEN '5%'
        WHEN a.IngresoEgresoTipo = 2 THEN '12%'
    END IngresoEgresoImpuesto
    , CASE 
    	WHEN a.IngresoEgresoTipo = 1
        	THEN CONVERT((a.IngresoEgresoMonto * 5)/ 100, DECIMAL(10,2))
        WHEN a.IngresoEgresoTipo = 2 
        	THEN CONVERT((a.IngresoEgresoMonto * 12)/ 100, DECIMAL(10,2))
    END IngresoEgresoMontoImpuesto
    , CASE 
    	WHEN a.IngresoEgresoTipo = 1
        	THEN CONVERT(a.IngresoEgresoMonto - (a.IngresoEgresoMonto * 5)/ 100, DECIMAL(10,2))
        WHEN a.IngresoEgresoTipo = 2 
        	THEN CONVERT(a.IngresoEgresoMonto - (a.IngresoEgresoMonto * 12)/ 100, DECIMAL(10,2))
    END IngresoEgresoMontoFinal
    , UPPER(DATE_FORMAT(a.IngresoEgresoFecha, '%M de %Y')) AS IngresoEgresoFecha
    , UPPER(b.PersonaNombre) AS Cliente
  FROM ingresoegreso a INNER JOIN persona b
  ON a.IdPersona = b.PersonaId
  WHERE a.IngresoEgresoTipo = pTipoRegistro
AND b.PersonaId = pIdPersona$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `Sp_TotalIngresosEgresos` (IN `pTipoRegistro` SMALLINT, IN `pIdPersona` INT)   SELECT
	CASE
    	WHEN a.IngresoEgresoTipo = 1
        	THEN CONVERT(SUM(a.IngresoEgresoMonto - (a.IngresoEgresoMonto*5)/100), DECIMAL(10,2))
    	WHEN a.IngresoEgresoTipo = 2
        	THEN CONVERT(SUM(a.IngresoEgresoMonto - (a.IngresoEgresoMonto*12)/100), DECIMAL(10,2))
	END TotalIngresoEgresoMonto
FROM ingresoegreso a
WHERE a.IngresoEgresoTipo = pTipoRegistro
AND a.IdPersona = pIdPersona$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `ingresoegreso`
--

CREATE TABLE `ingresoegreso` (
  `IngresoEgresoId` int(11) NOT NULL,
  `IngresoEgresoTipo` smallint(6) NOT NULL,
  `IngresoEgresoMonto` decimal(10,2) NOT NULL,
  `IngresoEgresoFecha` date NOT NULL,
  `IdPersona` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `ingresoegreso`
--

INSERT INTO `ingresoegreso` (`IngresoEgresoId`, `IngresoEgresoTipo`, `IngresoEgresoMonto`, `IngresoEgresoFecha`, `IdPersona`) VALUES
(1, 1, 2500.00, '2023-05-08', 1);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `persona`
--

CREATE TABLE `persona` (
  `PersonaId` int(11) NOT NULL,
  `PersonaNombre` varchar(150) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Volcado de datos para la tabla `persona`
--

INSERT INTO `persona` (`PersonaId`, `PersonaNombre`) VALUES
(1, 'Keyler Fernando Tajtaj'),
(2, 'Prueba Prueba Prueba');

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `ingresoegreso`
--
ALTER TABLE `ingresoegreso`
  ADD PRIMARY KEY (`IngresoEgresoId`),
  ADD UNIQUE KEY `IngresoEgresoFecha` (`IngresoEgresoFecha`),
  ADD KEY `IdPersona` (`IdPersona`);

--
-- Indices de la tabla `persona`
--
ALTER TABLE `persona`
  ADD PRIMARY KEY (`PersonaId`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `ingresoegreso`
--
ALTER TABLE `ingresoegreso`
  MODIFY `IngresoEgresoId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT de la tabla `persona`
--
ALTER TABLE `persona`
  MODIFY `PersonaId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- Restricciones para tablas volcadas
--

--
-- Filtros para la tabla `ingresoegreso`
--
ALTER TABLE `ingresoegreso`
  ADD CONSTRAINT `ingresoegreso_ibfk_1` FOREIGN KEY (`IdPersona`) REFERENCES `persona` (`PersonaId`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
