<?php
$conn = new mysqli("localhost", "root", "", "tenant_power_control");
$result = $conn->query("SELECT * FROM tenant_power");
?>

<!DOCTYPE html>
<html>
<head>
  <title>Live Tenant Power Status</title>
  <style>
    table { border-collapse: collapse; width: 60%; margin: auto; }
    th, td { border: 1px solid #ccc; padding: 10px; text-align: center; }
    th { background-color: #eee; }
  </style>
</head>
<body>
  <h2 style="text-align:center">Live Tenant Power Status</h2>
  <table>
    <tr>
      <th>Tenant ID</th>
      <th>Current (kW)</th>
      <th>Status</th>
      <th>Updated</th>
    </tr>
    <?php while($row = $result->fetch_assoc()): ?>
    <tr>
      <td><?= $row['tenant_id'] ?></td>
      <td><?= number_format($row['current_kw'], 3) ?></td>
      <td><?= $row['status'] ?></td>
      <td><?= $row['updated_at'] ?></td>
    </tr>
    <?php endwhile; ?>
  </table>
</body>
</html>
