<?php
	$con=mysqli_connect("localhost","root", "");
	// Check connection
	if (mysqli_connect_errno())
	{
		echo "Failed to connect to MySQL: " . mysqli_connect_error();	
		exit;
	}
	else
	{
		// Create database
		$sql="CREATE DATABASE my_db";
		if (mysqli_query($con, $sql))
		{
			echo "Database my_db created successfully";
			$table = "CREATE TABLE peoples (myname CHAR[30], myplace CHAR[15], myage INT";
			if (mysqli_query($con, $table))
			{
				echo "Table 'peoples' created successfully.";
			}
			else
			{
				echo "Error creating table: " . mysqli_error($con);
				exit;
			}
		}	
		else
		{
			echo "Error creating database: " . mysqli_error();
			exit;
		}
		$insert = "INSERT INTO peoples (myname, myplace, myage) VALUES ('louis', 'singu', 18)";
		mysqli_query($con, $insert);	 
		mysqli_close($con);
	}

?>