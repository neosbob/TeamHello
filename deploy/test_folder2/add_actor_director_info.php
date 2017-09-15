<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<meta name="viewport" content="width=device-width, initial-scale=1">

		<title>Add New Movie</title>
		<link href="./css/bootstrap.min.css" rel="stylesheet">
		<link href="./css/project1b.css" rel="stylesheet">

	<body>
		<nav class="navbar navbar-inverse navbar-fixed-top">
			<div class="container-fluid">
				<div class="navbar-header navbar-defalt">
				<a class="navbar-brand">CS143 Database Query System</a>
				</div>
			</div>
		</nav>


    <br><br>
    <div class="container">
    	<div class="row">
    		<div class="col-sm-3 col-md-2 sidebar">
				<ul class="nav nav-sidebar">
					<p><strong>&nbsp;&nbsp;Add new content</strong></p>
					<li><a href="add_actor_director_info.php">Add Actor/Director</a></li>
					<li><a href="add_movie_info.php">Add Movie Information</a></li>
					<li><a href="add_actor_movie_relation.php">Add Actor to Movie Relation</a></li>
					<li><a href="add_director_movie_relation.php">Add Director to Movie Relation</a></li>
					</ul>
				<ul class="nav nav-sidebar">
					<p><strong>&nbsp;&nbsp;Browsing Content</strong></p>
					<li><a href="show_actor_info.php">Show Actor Information</a></li>
					<li><a href="show_movie_info.php">Show Movie Information</a></li>
				</ul>
				<ul class="nav nav-sidebar">
					<p><strong>&nbsp;&nbsp;Search Interface</strong></p>
					<li><a href="search_page.php">Search/Actor Movie</a></li>
				</ul>
			</div>


	    	<div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main">
				<h2><strong>Actor/Actress or Director Input Page</strong></h2>
				<!-- Input form -->
					<form action="<?php $_PHP_SELF?>" method="POST">
						<span class="req"><small><i>* requires input for query result</i></small></span><br><br>
						Title*:
							<input type="radio" name="title" value="actor"/> Actor
							<input type="radio" name="title" value="director"/> Director
						<br><br>

						Last Name*: <input type="text" class="form-control" name="last_name" placeholder="eg: Wu" required/>
						<br>
						First Name*: <input type="text" class="form-control" name="first_name" placeholder="eg: Bob" required/>
						<br>
						Gender*: <input type="radio" name="gender" value="female" required/> Female
								 <input type="radio" name="gender" value="male"/> Male
						<br><br>
						Date of Birth*: <input class="form-control" type="date" name="dateofbirth" required/>
						<br>
						Date of Death: <input type="date" class="form-control" name="dateofdeath"/>
						<br>
						<input type="submit" value="submit" class="btn btn-primary"/>
					</form>

						<?php
							//Connect to the database
							if($_SERVER["REQUEST_METHOD"] == "POST"){
							$db_connection = mysqli_connect("localhost", "cs143", "");
							if (mysqli_connect_errno()){
				  			echo "Failed to connect to MySQL: " . mysqli_connect_error();
				  		}
							//Select database "CS143"
							mysqli_select_db($db_connection, "CS143");
							//Look for user input.
							$title = trim($_POST['title']);
							$last_name = trim($_POST['last_name']);
							$first_name = trim($_POST['first_name']);
							$gender = trim($_POST['gender']);
							$birth_date = $_POST['dateofbirth'];
							$birth_date = date("Y-m-d", strtotime($birth_date));
							if(!empty($_POST["dateofdeath"])){
								$death_date = $_POST['dateofdeath'];
								$death_date = date("Y-m-d", strtotime($death_date));
							} 
							else {
								$death_date = "NULL";
							}
							
							//find the id of current max
							$idquery = mysqli_query($db_connection, "SELECT id FROM MaxPersonID") or die(mysql_error());
							$result = mysqli_fetch_array($idquery);
							$id = $result[0];
							$id = $id + 1;
							//Update the MaxPersonID
							$updateMaxId = "UPDATE MaxPersonID SET id = $id";
							
							//Generate the query to insert relation.
							if($title=="actor"){
								if($death_date <> "NULL"){
									$addquery = "INSERT INTO Actor VALUES ('$id', '$last_name', '$first_name', '$gender', '$birth_date', '$death_date')";
								} 
								else {
									$addquery = "INSERT INTO Actor VALUES ('$id', '$last_name', '$first_name', '$gender', '$birth_date', NULL)";
								}
								echo "<br>";
								echo "<strong>Requesting addition of an actor with id $id...</strong>";
							} 
							elseif($title=="director"){
								if($death_date <> "NULL"){
									$addquery = "INSERT INTO Director VALUES ('$id', '$last_name', '$first_name', '$birth_date', '$death_date')";
								} 
								else {
									$addquery = "INSERT INTO Director VALUES ('$id', '$last_name', '$first_name', '$birth_date', NULL)";
								}
								echo "<br>";
								echo "<strong>Requesting addition of a director with id $id...</strong>";
							}
							
							//Execute the query to insert relation.
							$result = mysqli_query($db_connection, $addquery);
							if(!$result){
								echo ('Invalid query: ' . mysqli_error($db_connection));
								echo "<br><br>";
							} 
							else{
								mysqli_query($db_connection, $updateMaxId);
								echo "<br><br>";
								echo "<strong>Insertion successful!</strong>";
								echo "<br><br>";
							}
							mysqli_free_result($result);
							mysqli_close($db_connection);
						}			
					?>

				</div>
			</div>
		</div>
		
	 

	

	</body>

</html>