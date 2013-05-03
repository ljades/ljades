/*The modules*/
var express = require('express'),
	port = 9898,
	mongoose = require('mongoose'),
	util = require('util');
	var Schema = mongoose.Schema;

/*Opening the database through mongoosejs*/
mongoose.connect('mongodb://50.63.172.74/joeyplusplus');
var server = express();
server.listen(port);

/*Configure the server*/
server.configure(function(){
	server.use(express.bodyParser());
	server.use(server.router);
});

/*Route schema, defines a path*/
var routeSchema = new Schema({
	path:[{
  		lat: Number,
  		lon: Number,
  		isStop: Boolean,
  		name: String
	}],
});

/*Instance of a schema*/
var Route =  mongoose.model('Route', routeSchema);

/*index page*/
server.get('/joey/bing/default', function(request, response){
	var route = new Route({path:[{lat:42.406001, lon:-71.119944, isStop:true, name:"Upper Campus Center"}, {lat:42.405654, lon:-71.117447, isStop:false, name:""},{lat:42.401257, lon:-71.117029, isStop:false, name:""}, {loc:42.400687, lon:-71.117125, isStop:false, name:""},{loc:42.396872, lon:-71.122806, isStop:true, name:"Davis Square"}]});
	response.send(route);
});

server.get('/joey/bing/about', function(request, response){
	response.sendfile('joey/bing/about.html');
});

/*Requesting a specific route*/
server.get('/route', function(request, response){
	if(request.query._id){
		Route.find({_id:request.query._id}, function(err, obj){
			if(!obj) response.end('No matching id for value ' + request.query._id + ' found');
			else response.end(obj);
		});
	}else{
		response.send('Not a valid request');
	}
});

/*Submit a route*/
server.post('/submit', function(request, response){
	var route = new Route({path:{lat:0, lon:0, isStop:false, name:'Brendan'}});
	route.save(function(err, save){
		if(err || !save) response.end('500 Internal Server Error');
		else response.end('200 OK');
	});
});

server.get('/joey/*', function(request, response){
	response.sendfile('.' + request.url);
});

server.all('/', function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "X-Requested-With");
  next();
});

util.puts("> listening on port " + port);
