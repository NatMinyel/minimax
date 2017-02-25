var express = require('express'),
	path = require('path'),
	bodyParser = require('body-parser'),
	cons = require('consolidate'),
	dust = require('dustjs-helpers'),
	pg = require('pg'),
	app = express();

var connect = "postgres://postgres:newpassword@localhost/db";

app.engine('dust', cons.dust);

// For views (templates)
// Set .dust files to be parsed as html
app.set('view engine', 'dust');
// Set the views folder as the location for the templates
app.set('views', path.join(__dirname, 'views'));

// For static files
app.use(express.static(path.join(__dirname, 'public')));

// Body parser middleware
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false}));

app.get('/', function(req, res) {
    // PG Connect
	pg.connect(connect, function(err, client, done) {
		if (err) {
			return console.error('error fetching client from pool', err);
		}
		client.query('SELECT a1.username, a1.gamesplayed, a1.successrate, COUNT (a2.successrate) rank ' +
			'FROM users a1, users a2 ' +
			'WHERE a1.successrate < a2.successrate OR ' +
			'(a1.successrate=a2.successrate AND (a1.gamesplayed ' +
			'> a2.gamesplayed OR (a1.gamesplayed=a2.gamesplayed ' + 
			'AND (a1.username = a2.username)))) ' +
			'GROUP BY a1.username, a1.successrate, a1.gamesplayed ' +
			'ORDER BY a1.successrate DESC, a1.gamesplayed ASC, a1.username ASC;', function(err, result) {
			if (err) {
				return console.error('error running query', err);
			}
			// Render template
			res.render('index', {users: result.rows});
			done();
		});
	});
});

//Server
app.listen(3001, function() {
    console.log("Server started at port 3001.")
})
