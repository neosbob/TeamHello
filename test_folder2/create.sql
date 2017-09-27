-- After creating each table, use ENGINE=InnoDB to support foreign keys.
-- As instructed, Three primary key constraints, Six referential integrity 
-- constraints (foreign keys), and Three CHECK constraints are included.

-- Create Movie table
-- primary key: id
-- Check: the rating of the movie.
-- Check: the year of the movie.
CREATE TABLE Movie(
	id INT NOT NULL,
	title VARCHAR(100) NOT NULL,
	year INT NOT NULL,
	rating VARCHAR(10),
	company VARCHAR(50),
	PRIMARY KEY(id),
	CHECK(rating IS NULL OR rating='G' OR rating='PG' OR rating='PG-13' OR rating='NC-17' OR rating='R'),
	CHECK(year>1500 AND year<3000)
) ENGINE=InnoDB;

-- Create Actor table
-- primary key: id
CREATE TABLE Actor(
	id INT NOT NULL,
	last VARCHAR(20),
	first VARCHAR(20),
	sex VARCHAR(6),
	dob DATE NOT NULL,
	dod DATE,
	PRIMARY KEY(id)
) ENGINE=InnoDB;

-- Create Sales table
-- foreign key constraint: (mid in sales) -> (id of movie in movie table)
-- Check: if tickets or income is greater than 0.
CREATE TABLE Sales(
	mid INT NOT NULL,
	ticketsSold INT,
	totalIncome INT,
	FOREIGN KEY (mid) references Movie(id),
	CHECK(ticketsSold>=0),
	CHECK(totalIncome>=0)
) ENGINE=InnoDB;

-- Create Director table
-- primary key: id
CREATE TABLE Director(
	id INT NOT NULL,
	last VARCHAR(20),
	first VARCHAR(20),
	dob DATE,
	dod DATE,
	PRIMARY KEY(id)
) ENGINE=InnoDB;

-- Create MovieGenre table
-- foreign key constraint: (mid in MovieGenre) -> (id of movie in movie table)
CREATE TABLE MovieGenre(
	mid INT NOT NULL,
	genre VARCHAR(20) NOT NULL,
	FOREIGN KEY (mid) references Movie(id)
) ENGINE=InnoDB;

-- Create MovieDirector table
-- foreign key constraint: (mid in MovieDirector) -> (id of movie in movie table)
-- foreign key constraint: (did in MovieDirector) -> (id of director in director table)
CREATE TABLE MovieDirector(
	mid INT NOT NULL,
	did INT NOT NULL,
	FOREIGN KEY (mid) references Movie(id),
	FOREIGN KEY (did) references Director(id)
) ENGINE=InnoDB;

-- Create MovieActor table
-- foreign key constraint: (mid in MovieActor) -> (id of movie in movie table)
-- foreign key constraint: (aid in MovieActor) -> (id of actor in actor table)
CREATE TABLE MovieActor(
	mid INT NOT NULL,
	aid INT NOT NULL,
	role VARCHAR(50),
	FOREIGN KEY (mid) references Movie(id),
	FOREIGN KEY (aid) references Actor(id)
) ENGINE=InnoDB;

-- Create MovieRating table
-- foreign key constraint: (mid in MovieRating) -> (id of movie in movie table)
CREATE TABLE MovieRating(
	mid INT NOT NULL,
	imdb INT,
	rot INT,
	FOREIGN KEY (mid) references Movie(id)
) ENGINE=InnoDB;

-- Create Review table
-- foreign key constraint: (mid in Review) -> (id of movie in movie table)
-- Check: if rating of an movie is between 0 and 5.
CREATE TABLE Review(
	name VARCHAR(20),
	time TIMESTAMP,
	mid INT NOT NULL,
	rating INT NOT NULL,
	comment VARCHAR(500),
	FOREIGN KEY (mid) references Movie(id),
	CHECK(rating>=0 AND rating<=5)
) ENGINE=InnoDB;

-- Create MaxPersonID table
CREATE TABLE MaxPersonID(
	id INT NOT NULL
) ENGINE=InnoDB;

-- Create MaxMovieID table
CREATE TABLE MaxMovieID(
	id INT NOT NULL
) ENGINE=InnoDB;
