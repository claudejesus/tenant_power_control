<?php
// DB config
$host = "localhost";
$user = "root";
$password = "";
$dbname = "electricity_system";

$conn = new mysqli($host, $user, $password, $dbname);

if ($conn->connect_error) {
    http_response_code(500);
    echo json_encode(["error" => "DB connection failed"]);
    exit;
}

// Run query
$sql = "SELECT id, tenant_id, current_kw, status, updated_at FROM tenant_power ORDER BY updated_at DESC";
$result = $conn->query($sql);

$data = [];
if ($result) {
    while ($row = $result->fetch_assoc()) {
        $data[] = $row;
    }
}

header('Content-Type: application/json');
echo json_encode($data);

$conn->close();
?>
