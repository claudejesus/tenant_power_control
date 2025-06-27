<?php
// DB config
$host = "localhost";
$user = "root";
$pass = "";
$db = "tenant_power_control";

// Connect
$conn = new mysqli($host, $user, $pass, $db);
if ($conn->connect_error) {
    die("DB connection failed: " . $conn->connect_error);
}

// Read POST
$tenant_id = $_POST['tenant_id'] ?? '';
$current_kw = $_POST['current_kw'] ?? '';
$status = $_POST['status'] ?? '';
$updated_at = date("Y-m-d H:i:s");

// Check values
if ($tenant_id === '' || $current_kw === '' || $status === '') {
    echo "Missing fields!";
    exit;
}

// Insert or update
$sql = "INSERT INTO tenant_power (tenant_id, current_kw, status, updated_at)
        VALUES ('$tenant_id', '$current_kw', '$status', '$updated_at')
        ON DUPLICATE KEY UPDATE
            current_kw = '$current_kw',
            status = '$status',
            updated_at = '$updated_at'";

if ($conn->query($sql) === TRUE) {
    echo "Updated successfully";
} else {
    echo "Error: " . $conn->error;
}

$conn->close();
?>
