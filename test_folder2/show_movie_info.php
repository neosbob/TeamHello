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
				<h2><strong>Movie Information Page</strong></h2>
				<hr>
				<!-- Input form -->
						<?php
							if(isset($_REQUEST["identifier"])){
								//Connect to the database
								$db_connection = mysqli_connect("localhost", "root", "dudebob71");
							if (!$db_connection){
								echo "Failed to connect to MySQL: " . mysqli_connect_error();
							}
							//Select database "CS143"
							mysqli_select_db($db_connection, "CS143");
							//Look at user input for query.
							$id = $_REQUEST["identifier"];
							
							if ($id == null) {
								$result = mysqli_query($db_connection,"SELECT * FROM Actor") or die(mysqli_error());
								$result_movie = mysqli_query($db_connection,"SELECT * FROM Movie") or die(mysqli_error());
							} 
							else {
								$result = mysqli_query($db_connection,"SELECT Actor.first, Actor.last, MovieActor.role, Actor.id FROM Actor, MovieActor WHERE MovieActor.mid = $id AND MovieActor.aid = Actor.id") or die(mysqli_error());
								$result_movie = mysqli_query($db_connection,"SELECT * FROM Movie WHERE id = $id") or die(mysqli_error());
								$result_director = mysqli_query($db_connection,"SELECT Director.first, Director.last FROM MovieDirector, Director WHERE mid = $id AND MovieDirector.did = Director.id") or die(mysqli_error());
								$result_genre = mysqli_query($db_connection,"SELECT genre FROM MovieGenre WHERE mid = $id") or die(mysqli_error());
								$result_review = mysqli_query($db_connection,"SELECT AVG(rating), COUNT(rating) FROM Review WHERE mid = $id") or die(mysqli_error());
								$result_comment = mysqli_query($db_connection,"SELECT name, rating, time, comment FROM Review WHERE mid = $id") or die(mysqli_error());	
							}
							//Print out information associated with movie.
							echo "<h4><b>Movie Information is : </b></h4>";
							$movie = mysqli_fetch_row($result_movie);
							echo "Title: $movie[1] <br>";
							echo "Producer: $movie[4]<br>";
							echo "MPAA Rating: $movie[3]<br>";
							//print out all directors involved in movie.
							$director = mysqli_fetch_row($result_director);
							if ($director == null) {
								echo "Director: N/A";
							} 
							else {
								echo "Director: $director[0] $director[1]";
								while ($director = mysqli_fetch_row($result_director)) {
									echo ",$director[0] $director[1]";
								}	
							}
							echo "<br>";
							//print out all genres the movie belongs in.
							$genre = mysqli_fetch_row($result_genre);
							if ($genre == null) {
								echo "Genre: N/A";
							} 
							else {
								echo "Genre: $genre[0]";
								while ($genre = mysqli_fetch_row($result_genre)) {
									echo ",$genre[0]";
								}
							}
							echo "<br>";
							//Actors information
							echo "<h4><b>Actors in this Movie : </b></h4>";
							echo "<table class = 'table table-bordered table-condensed table-hover'>\n<tr align=center>";
							echo "<br>";
							echo "<td><b>Name</b></td>";
							echo "<td><b>Role</b></td>";
							
							echo "</tr>\n";
							while ($row = mysqli_fetch_row($result)) {
								echo "<tr align=center>";
								echo '<td><a href="show_actor_info.php?identifier='."$row[3]".'">'."$row[0] $row[1]".'</a></td>';
								echo "<td>$row[2]</td>";
								echo "</tr>\n";
							}
							echo "</table><hr>";
							//Link to page for user to leave review.
							$review = mysqli_fetch_row($result_review);
							echo "<h4><b>User Review : </b></h4>";
							echo "Average score for this Movie is $review[0]/5 based on $review[1] people's reviews.<br>";
							echo '<a href= "add_comment.php?MovieID='."$movie[0]".'">leave your comment as well!</a><hr>';
							//print out all the reviews left by users.
							echo "<h4><b>Comment details shown below : </b></h4>";
							echo "<hr>";
							while ($row_comment = mysqli_fetch_row($result_comment)) {
								echo "$row_comment[0] rates the this movie with score $row_comment[1] and left a review at $row_comment[2]<br>comment:<br>$row_comment[3]<br>";
							}
							
							mysqli_free_result($result);
							mysqli_free_result($result_movie);
							mysqli_free_result($result_genre);
							mysqli_free_result($result_director);
							mysqli_free_result($result_review);
							mysqli_free_result($result_comment);
							mysqli_close($db_connection);
						}
					?>
					<br><br><br>
					<label for="search_input">Search:</label>
					<form class="form-group" method ="GET" id="usrform" action="search_page.php">
						<input type="text" class="form-control" placeholder="Search..." name="search_input"><br>
						<input type="submit" value="submit" class="btn btn-primary"/>
					</form>
				</div>
			</div>
		</div>
		
	 

	

	</body>

</html>
