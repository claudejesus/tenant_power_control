-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 27, 2025 at 01:10 PM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `tenant_power_control`
--

-- --------------------------------------------------------

--
-- Table structure for table `tenant_power`
--

CREATE TABLE `tenant_power` (
  `id` int(11) NOT NULL,
  `tenant_id` int(11) NOT NULL,
  `current_kw` float DEFAULT 0,
  `status` varchar(20) DEFAULT 'disconnected',
  `updated_at` datetime DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `tenant_power`
--

INSERT INTO `tenant_power` (`id`, `tenant_id`, `current_kw`, `status`, `updated_at`) VALUES
(1, 1, 0, 'disconnected', '2025-06-27 13:10:46'),
(224, 2, 0, 'disconnected', '2025-06-27 13:05:56'),
(225, 3, 0, 'disconnected', '2025-06-27 13:05:56');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `tenant_power`
--
ALTER TABLE `tenant_power`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `tenant_id` (`tenant_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `tenant_power`
--
ALTER TABLE `tenant_power`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=285;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
