
var stops = [{id:"Upper Campus Center", loc:new google.maps.LatLng(42.406001, -71.119944)}, 
             {id:"Davis Square",        loc:new google.maps.LatLng(42.396872, -71.122806)},
             {id:"Lower Campus Center", loc:new google.maps.LatLng(42.405276, -71.120443)},
         	 {id:"Carmichael",          loc:new google.maps.LatLng(42.409641, -71.122264)},
         	 {id:"Olin",                loc:new google.maps.LatLng(42.408292, -71.120864)}];

var waypoints = [];
for (var i=1;i<stops.length;i++) {
	waypoints.push({location:stops[i].loc});
}



var route = [{loc:new google.maps.LatLng(42.406001, -71.119944), isMarker:true, name:"Upper Campus Center"},
			 {loc:new google.maps.LatLng(42.405654, -71.117447), isMarker:false},
			 {loc:new google.maps.LatLng(42.401257, -71.117029), isMarker:false},
			 {loc:new google.maps.LatLng(42.400687, -71.117125), isMarker:false},
			 {loc:new google.maps.LatLng(42.396872, -71.122806), isMarker:true, name:"Davis Square"}];
/*
			 {loc:new google.maps.LatLng(), isMarker:false},
			 {loc:new google.maps.LatLng(), isMarker:false},
			 {loc:new google.maps.LatLng(42.405276, -71.120443), isMarker:true, name:"Lower Campus Center"},
			 {loc:new google.maps.LatLng(), isMarker:false},
			 {loc:new google.maps.LatLng(), isMarker:false},
			 {loc:new google.maps.LatLng(42.409641, -71.122264), isMarker:true, name:"Carmichael"}
			 {loc:new google.maps.LatLng(), isMarker:false},
			 {loc:new google.maps.LatLng(), isMarker:false},
			 {loc:new google.maps.LatLng(42.408292, -71.120864), isMarker:true, name:"Olin"}];
			 {loc:new google.maps.LatLng(), isMarker:false},
			 {loc:new google.maps.LatLng(), isMarker:false},
*/

// Represents a point along a route
function RoutePoint(latLng, isMarker, name) {
	this.latLng = latLng;
	if (isMarker) {
		this.isMarker = isMarker;
		this.name = name;
	}
	else {
		this.isMarker = false;
		this.name = "Untitled";
	}
}

/* Directions are rendered as a POLYLINE of pts. Some of the points will be MARKERS as well.
   When we wish to calculate the timetables, this should be converted to a DIRECTIONS object. */

function initialize() {
	var mapOptions = {
          center: new google.maps.LatLng(42.4069, -71.1198),
          zoom: 14,
          mapTypeId: google.maps.MapTypeId.ROADMAP
    };
	var map = new google.maps.Map(document.getElementById("mapcanvas"), mapOptions);

	renderer = new google.maps.DirectionsRenderer({map: map, draggable: true});
	var waypts = route.map(function (p) {
		return {location:p.loc, stopover:false};
	});
	var directionsReq = {
		travelMode: google.maps.TravelMode.DRIVING,
		provideRouteAlternatives: false,
		origin: route[0].loc,
		destination: route[0].loc,
		waypoints: waypts
	};

	var dirService = new google.maps.DirectionsService();
	dirService.route(directionsReq, function (result, status) {
		console.log("request status: " + status);
		if (status === google.maps.DirectionsStatus.OK) {
			renderer.setDirections(result);
		}
	});
}




/*
	TRIED TO USE DIRECTIONS REQUESTING -- ALL JUMBLED UP, NOT IN THE ORDER WE WANT

		var directionsReq = {
		travelMode: google.maps.TravelMode.DRIVING,
		provideRouteAlternatives: false,
		origin: stops[0].loc,
		destination: stops[0].loc,
		waypoints: waypoints
	};

	var dirService = new google.maps.DirectionsService();
	dirService.route(directionsReq, function (result, status) {
		console.log("request status: " + status);
		if (status === google.maps.DirectionsStatus.OK)
		var options = {
			directions:result,
			map: map,
			draggable: true,
		};
		var renderer = new google.maps.DirectionsRenderer(options);
	});
*/


