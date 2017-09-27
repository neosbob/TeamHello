<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<meta name="viewport" content="width=device-width, initial-scale=1">

		<title>Add New Actor or Director</title>
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
				<h2><strong>Actor/Actress or Movie Search Page</strong></h2>
				<hr>
				<!-- Input form -->
					<label for="search_input">Search:</label>
					<form class="form-group" method ="GET" id="usrform" action="<?php $_SERVER[PHP_SELF]?>">
						<input type="text" class="form-control" placeholder="Search..." name="search_input"><br>
						<input type="submit" value="submit" class="btn btn-primary"/>
					</form>

						<?php
                                                    var_dump($_GET);
                                                    
							//if no http input is inserted, return whole database.
							if(isset($_GET["search_input"])){
								//Connect to the database
								$db_connection = mysqli_connect("localhost", "root", "dudebob71");
							if (!$db_connection){
								echo "Can't connect to MySQL: " . mysqli_connect_error();
							}
							//Select database "CS143"
							mysqli_select_db($db_connection, "CS143");
							//Look at user input for query.
							$input_string = trim($_GET["search_input"]);
							$keywords = preg_split("/[\s,]+/", $input_string);
							if ($input_string == null) {
								$result = mysqli_query($db_connection,"SELECT * FROM Actor") or die(mysqli_error());
								$result_movie = mysqli_query($db_connection,"SELECT * FROM Movie") or die(mysqli_error());
							} 
							else {
								if(count($keywords) == 1) {
									//echo "$keywords[0]";
									$result = mysqli_query($db_connection,"SELECT * FROM Actor WHERE (first like '%{$keywords[0]}%') OR (last like '%{$keywords[0]}%')") or die(mysqli_error());
								} 
								else if (count($keywords) == 2){
									$result = mysqli_query($db_connection,"SELECT * FROM Actor WHERE (first like '$keywords[0]' AND last like '$keywords[1]') OR (first like '$keywords[1]' AND last like '$keywords[0]')") or die(mysqli_error());
								}
								$movie_query = "SELECT * FROM Movie WHERE title like '%{$keywords[0]}%'";
								for ($i = 1; $i < count($keywords); $i++) {
									$movie_query = $movie_query." AND title like '%{$keywords[$i]}%'";
								}
								$result_movie = mysqli_query($db_connection, $movie_query) or die(mysqli_error());
							}
							
							//print actor results
							echo "<h4><b>matching Actors are:</b></h4>";
							echo "<table class = 'table table-bordered table-condensed table-hover'>\n<tr align=center>";
							echo "<br>";
							echo "<td><b>Name</b></td>";
							echo "<td><b>Gender</b></td>";
							echo "<td><b>Date of Birth</b></td>";
							
							echo "</tr>\n";
							while ($row = mysqli_fetch_row($result)) {
								echo "<tr align=center>";
								echo '<td><a href="show_actor_info.php?identifier='."$row[0]".'">'."$row[2] $row[1]".'</a></td>';
								echo "<td>$row[3]</td>";
								echo "<td>$row[4]</td>";
								echo "</tr>\n";
							}
							echo "</table>";
							//print movie results
							echo "<h4><b>matching Movies are:</b></h4>";
							echo "<table class = 'table table-bordered table-condensed table-hover'>\n<tr align=center>";
							echo "<br>";
							echo "<td><b>Title</b></td>";
							echo "<td><b>Year</b></td>";
							
							
							echo "</tr>\n";
							while ($row = mysqli_fetch_row($result_movie)) {
								echo "<tr align=center>";
								echo '<td><a href= "show_movie_info.php?identifier='."$row[0]".'">'."$row[1]".'</a></td>';
								echo "<td>$row[2]</td>";
								echo "</tr>\n";
							}
							echo "</table>";
							mysqli_free_result($result);
							mysqli_free_result($result_movie);
							mysqli_close($db_connection);
						}
					?>

				</div>
			</div>
		</div>
		
	 

	

	</body>

</html>
