<!DOCTYPE html>
<html>
<head>
    <title>Addition</title>
</head>
<body>
    <form method="post" action="">
        <label>Enter the first number:</label>
        <input type="text" name="num1"><br><br>
        <label>Enter the second number:</label>
        <input type="text" name="num2"><br><br>
        <input type="submit" name="submit" value="Add">
    </form>
    <?php
    if(isset($_POST['submit'])){
        // Getting input values from the form
        $num1 = $_POST['num1'];
        $num2 = $_POST['num2'];
        
        // Adding the numbers
        $sum = $num1 + $num2;
        
        // Displaying the sum
        echo "The sum of $num1 and $num2 is: $sum";
    }
    ?>
</body>
</html>
