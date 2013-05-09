

/*var stops = [{id:"Upper Campus Center", loc:new Microsoft.Maps.Location(42.406001, -71.119944)}, 
             {id:"Davis Square",        loc:new Microsoft.Maps.Location(42.396872, -71.122806)},
             {id:"Lower Campus Center", loc:new Microsoft.Maps.Location(42.405276, -71.120443)},
         	 {id:"Carmichael",          loc:new Microsoft.Maps.Location(42.409641, -71.122264)},
         	 {id:"Olin",                loc:new Microsoft.Maps.Location(42.408292, -71.120864)}];

var route = [{loc:new Microsoft.Maps.Location(42.406001, -71.119944), isMarker:true, name:"Upper Campus Center"},
			 {loc:new Microsoft.Maps.Location(42.405654, -71.117447), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.401257, -71.117029), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.400687, -71.117125), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.396872, -71.122806), isMarker:true, name:"Davis Square"}];

			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.405276, -71.120443), isMarker:true, name:"Lower Campus Center"},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.409641, -71.122264), isMarker:true, name:"Carmichael"}
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.408292, -71.120864), isMarker:true, name:"Olin"}];
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
*/

var locs = [{lat: 42.405999, lon: -71.119944, isMarker:true, name:"Campus Center"},
 			{lat: 42.405649000000004, lon: -71.117447, isMarker:false},
 			{lat: 42.401261, lon: -71.11700699999999, isMarker:false},
			{lat: 42.40066, lon: -71.117157, isMarker:false},
 			{lat: 42.39686300000001, lon: -71.12281600000001, isMarker:true, name:"Davis"}];

$(document).ready(function () {
	map = L.map('map').setView([42.406001, -71.119944], 13);

	L.tileLayer("http://{s}.tile.cloudmade.com/858d8bfe931743af80d65eb9e0aaafb0/997/256/{z}/{x}/{y}.png", {
	    attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="http://cloudmade.com">CloudMade</a>',
	    maxZoom: 14
	}).addTo(map);

	displayRoute(locs, map);
});

function displayRoute (route, map) {
	// draw the poly line
	var polyline = L.polyline(_.map(route, function (pt) { return new L.LatLng(pt.lat, pt.lon); }), {color: 'blue'}).addTo(map);

	// for each stop, add a marker and popup.
	_.each(_.filter(route, function (pt) { return pt.isMarker; }), function (stop) {
		var marker = L.marker([stop.lat, stop.lon], {draggable:true, title:stop.name}).addTo(map);
		marker.bindPopup(stop.name).openPopup();
	});
}



