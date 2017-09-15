<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<meta name="viewport" content="width=device-width, initial-scale=1">

		<title>Director-Movie Relation</title>
		<link href="./css/bootstrap.min.css" rel="stylesheet">
		<link href="./css/project1b.css" rel="stylesheet">

	<body>

		<!-- Database Connection -->
		<?php
			$db_connection = mysqli_connect("localhost", "cs143", "");
			if (mysqli_connect_errno()){
		  	echo "Failed to connect to MySQL: " . mysqli_connect_error();
		  }
			mysqli_select_db($db_connection, "CS143");
		?>

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
    			<h2><strong>Movie Director Relation</strong></h2>
    			<!-- Input form -->
					<div>
						<form action="<?php $_PHP_SELF?>" method="POST">
						<span class="req"><small><i>* requires input for query result</i></small></span><br><br>
						<!-- This part gives me a scroll down list of all movies -->
						Movie title*: 
						<select name="mtitle" class="form-control" required>
							<option value="" disabled selected>Input Movie Title</option>
							<?php
								$movie_query = "SELECT id, title, year FROM Movie ORDER BY title";
								$movie_result = mysqli_query($db_connection, $movie_query);
								if($movie_result){
									while($row = mysqli_fetch_row($movie_result)){
										echo "<option value=$row[0]>$row[1], ($row[2])</option>";
									}
								}
							?>
						</select>
						<br>
						<!-- This part gives me a scroll down list of all directors -->
						Director name*: 
						<select name="dirname" class="form-control" required>
							<option value="" disabled selected>Input Director Name</option>
							<?php
								$director_query = "SELECT id, last, first FROM Director ORDER BY first";
								$director_result = mysqli_query($db_connection, $director_query);
								if($director_result){
									while($dirRow = mysqli_fetch_row($director_result)){
										echo "<option value=$dirRow[0]>$dirRow[2] $dirRow[1]</option>";
									}
								}
							?>
						</select>
						<br>

						<input type="submit" value="submit" class="btn btn-primary"/>
						<br><br>
						</form>

						<!-- This part adds the relation of director to movie-->
						<?php
							if($_SERVER["REQUEST_METHOD"] == "POST"){
								$mid = $_POST["mtitle"];
								echo "<strong>Movie id chosen: $mid </strong><br><br>";
								$did = $_POST["dirname"];
								echo "<strong>Director id chosen: $did </strong><br><br>";
								$insert_query = "INSERT INTO MovieDirector VALUES ($mid, $did)";
								$insert_result = mysqli_query($db_connection, $insert_query);
								if($insert_result){
									echo "<strong>Director-movie relation inserted.</strong>";
								} else {
									echo 'Invalid query: ' . mysqli_error($db_connection);
								}
								mysqli_close($db_connection);
							}
						?>

					</div>
    		</div>
    	</div>
    </div>

		

		

	</body>

</html>