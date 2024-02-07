CREATE DATABASE  IF NOT EXISTS `stockmarket` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `stockmarket`;
-- MySQL dump 10.13  Distrib 8.0.36, for Linux (x86_64)
--
-- Host: localhost    Database: stockmarket
-- ------------------------------------------------------
-- Server version	8.0.36-0ubuntu0.22.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `transactions`
--

DROP TABLE IF EXISTS `transactions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `transactions` (
  `transactionId` int NOT NULL AUTO_INCREMENT,
  `email` varchar(50) NOT NULL,
  `type` varchar(5) NOT NULL,
  `stockCompany` varchar(45) NOT NULL,
  `price` double NOT NULL,
  `datetime` datetime NOT NULL,
  PRIMARY KEY (`transactionId`),
  KEY `email_transactions_idx` (`email`),
  CONSTRAINT `email_transactions` FOREIGN KEY (`email`) REFERENCES `users` (`email`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transactions`
--

LOCK TABLES `transactions` WRITE;
/*!40000 ALTER TABLE `transactions` DISABLE KEYS */;
INSERT INTO `transactions` VALUES (1,'saramohamed','buy','AAPL',120.34,'2024-02-06 11:55:24'),(2,'saraahmed','buy','AAPL',0,'2024-02-06 12:11:37'),(3,'saraahmed','buy','FB',0.92,'2024-02-06 12:13:11'),(4,'saraahmed','sell','FB',0.93,'2024-02-06 12:15:34'),(5,'saraahmed','sell','FB',0.05,'2024-02-06 12:16:32'),(6,'saraahmed','buy','AAPL',0.09,'2024-02-06 12:30:32'),(7,'saraahmed','sell','AAPL',0.08,'2024-02-06 16:18:14'),(8,'saraahmed','buy','FB',0.12,'2024-02-06 16:20:11'),(9,'saraahmed','sell','FB',0.02,'2024-02-06 20:55:43'),(10,'saraahmed','buy','FB',0.02,'2024-02-07 17:09:35'),(11,'saraahmed','sell','FB',0.02,'2024-02-07 17:09:55'),(12,'saraahmed','sell','FB',0.02,'2024-02-07 17:12:48'),(13,'saraahmed','buy','BABA',43.81,'2024-02-07 17:53:17');
/*!40000 ALTER TABLE `transactions` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2024-02-07 17:56:01
