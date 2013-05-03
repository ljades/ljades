
/*
	A JRoute encapsulates the information about a route of the joey. 
	It contains representations of all the points on the route, as well
	as markers and pop ups for the stops.

	The purpose here is to provide a set of functions to call that make
	dealing with the route as a whole easier. This way, you dont have to
	even know the representation in order to make sure of it.
 */

function JRoute (map, route) {
	/**************
		Private
	***************/

	var _manager, _jstops = undefined;


	// checkpoints is a list of past, present and future stringified routes
	// currentCheckPoint is the index of the checkpoint currently displayed.
	// undoing displays the checkpoint BEFORE the current, if it exists
	// redoing displays the checkpoint AFTER  the current, if it exists
	var checkpoints = [route];
	var currentCheckPoint = 0;

	// didnt fucking help
	var destroyManager = function (man) {
		_.each(man.getAllWaypoints(), function (waypt) {
			if (waypt.jstop) {
				waypt.jstop.killPopover();
				waypt.jstop = undefined;
			}

			if (waypt.getPushpin()) {
				console.log("found a pushpin!");
				// map.entities.remove(waypt.getPushpin());
				console.log("removed pp");
				waypt.getPushpin().setOptions({ visible:false });
				waypt.setOptions({pushpin: undefined});x
			}
		});
		// man.dispose();
		console.log("maps's entities: " + map.entities.length);
	}

	/* renders a json route (list of pts, some of which are stops) to the map */
	var _render = function (pts) {
		// remove old to avoid duplicates
		if (_jstops) {
			_.each(_jstops, function (jstop) {
				map.entities.remove(jstop.pin);
			})
		}
		if (_manager) {
			console.log("disposing!");
			destroyManager(_manager);
		}

		console.log("done disposing");

		_manager = new Microsoft.Maps.Directions.DirectionsManager(map);
		_manager.setRenderOptions({
			// hide the viapoint & endpoint pushpins
			viapointPushpinOptions:{visible:false}, 
			waypointPushpinOptions:{visible:false}
		});   

		// add waypoints, returns a list of jstops.
		_jstops = _.reduce(pts, function (memo, waypt, index) {
			// console.log("rendering a pt at index " + index);
			var jstop = null;
			if (waypt.isMarker) {
				jstop = new JStop(map, waypt);
				memo.push(jstop);
			}	

			var waypt = new Microsoft.Maps.Directions.Waypoint({
				location:new Microsoft.Maps.Location(waypt.loc.latitude, waypt.loc.longitude), 
				isViapoint:!waypt.isMarker,
				pushpin:(jstop ? jstop.getPin() : null),
			});
			waypt.jstop = jstop;
			_manager.addWaypoint(waypt);

			return memo;
		}, []);

		// get, and display, directions:
		_manager.calculateDirections();
	} 

	_render(route);

	var _getAllWaypoints = function () {
		return _manager.getAllWaypoints();
	}

	var _getAllPoints = function () {
		return _.map(_manager.getAllWaypoints(), function (waypt) { return waypt.getLocation(); });
	}

	/* converts the internal representation to JSON, for uploading to database or saving */
	var _stringify = function () {
		// go through and build the array of objects
		var pts = _.map(_getAllWaypoints(), function (waypt) {
			var stopName = undefined;

			// make sure the waypt represents a stop that has not been deleted
			if (waypt.jstop && _.contains(_jstops, waypt.jstop) === true) {
				stopName = waypt.jstop.getName();
			}

			var obj = { 
				loc: waypt.getLocation(),
				isMarker: (stopName !== undefined),
				name: stopName
			};
			return obj;
		});

		return JSON.stringify(pts);
	};

	Microsoft.Maps.Events.addHandler(_manager, 'dragDropCompleted', function (p) {
		console.log("dragDropCompleted");
		currentCheckPoint++;
		checkpoints[currentCheckPoint] = _stringify();
	});

	Microsoft.Maps.Events.addHandler(_manager, 'directionsUpdated', function (p) {
		console.log("directionsUpdated");
	});

	var _undo = function () {
		if (currentCheckPoint > 0) {
			currentCheckPoint--;
			_render(checkpoints[currentCheckPoint]);	
		}
	};

	var _redo = function () {
		if (currentCheckPoint < checkpoints.length - 1) {
			currentCheckPoint++;
			_render(checkpoints[currentCheckPoint]);	
		}
	};

	/**************
		Public
	**************/

	this.getAllPoints = function () {
		return _getAllPoints();
	};

	// returns an array of JStops. dont mutate the list you dummy
	this.getAllStops = function () {
		return _jstops;
	};
	var _removeJStop = function(_jstops, jstopToRemove){
		var newStops = [];
		var compare = function(j1, j2){
			if(!(j1.getLocation() === j2.getLocation()))return false;
			if(!(j1.getName() === j2.getName())) return false;
			return true;
		};
		for(var i = 0; i < _jstops.length; i++){
			if(compare(_jstops[i], jstopToRemove)) newStops.push(_jstops[i]);
		}
		return newStops;
	};
	this.removeStop = function (jstopToRemove) {
		map.entities.remove(jstopToRemove.getPin());
		var newjson = _stringify();
		_render(JSON.parse(newjson));
	};

	
	// reverses the direction of the route
	this.reverse = function () {

	};

	this.undo = function () {
		_undo();
	};

	this.redo = function () {
		_redo();
	};

	// converts the route to a string, for saving on back end
	this.stringify = function () {
		return _stringify();
	}
}

/*
		// returns the name of the stop at that location, or undefined if no stop exists 
		var nameForStopAtLocation = function (p) {
			var stop =  _.find(_jstops, function (stop) {
				console.log("comparing " + p + " and " + stop.getLocation().latitude + "," + stop.getLocation().longitude);
				return Microsoft.Maps.Location.areEqual(stop.getLocation(), p);
			});

			console.log("looking for stop at loc, and found " + (stop? "ONE" : "NONE"));

			return (stop ? stop.getName() : undefined);
		};


 */