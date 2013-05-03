

var jsonroute  = [{"loc":{"latitude":42.406001,"longitude":-71.119944},"isMarker":true,"name":"Upper Campus Center"},
				 {"loc":{"latitude":42.405654,"longitude":-71.117447},"isMarker":false},
				 {"loc":{"latitude":42.401257,"longitude":-71.117029},"isMarker":false},
				 {"loc":{"latitude":42.400687,"longitude":-71.117125},"isMarker":false},
				 {"loc":{"latitude":42.396872,"longitude":-71.122806},"isMarker":true,"name":"Davis Square"}];

var without = 

/*
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.405276, -71.120443), isMarker:true, name:"Lower Campus Center"},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.409641, -71.122264), isMarker:true, name:"Carmichael"}
			 {Location:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(42.408292, -71.120864), isMarker:true, name:"Olin"}];
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
			 {loc:new Microsoft.Maps.Location(), isMarker:false},
*/

$(document).ready(function (){
	var loader = new ModuleLoader(['Microsoft.Maps.Directions'], /* 'Microsoft.Maps.Themes.BingTheme'], */{callback: initialize});
});

function initialize () {
	// init code for side bar pages
	$.get('http://50.63.172.74:9898/joey/bing/default', function(data) {
  		
	});
	$('a[href="#sidebar-about"]').click(function (e) { e.preventDefault(); $(this).tab('show'); });
	$('a[href="#sidebar-route"]').click(function (e) { e.preventDefault(); $(this).tab('show'); });
	
	// make map
	var map = new Microsoft.Maps.Map(document.getElementById("map"), {
		center: new Microsoft.Maps.Location(42.403274, -71.123214),
        mapTypeId: Microsoft.Maps.MapTypeId.road,
        credentials: "AqUvlUQLaxxqadKCETv2A2oa46236Nca_Pke4MQ5iXEcaD-_2NsL3duxDZhTVyoM",
        zoom: 15
    });

	var route = new JRoute(map, jsonroute);
	renderSideBar(route, map);

	window.onkeydown = function (e) {
		if (e.metaKey) {
		 	if (e.keyCode == 90) {
				if (e.shiftKey) {
					console.log("redo");
					route.redo();
				}
				else {
					console.log("undo");				
					route.undo();
				}
				e.stopPropagation();
				e.preventDefault();
			}
			if (e.keyCode == 83) {
				console.log("saving!");

				// do some cool ass saving shit here yeah fuck yeah dickos. 

				e.stopPropagation();
				e.preventDefault();
			}
		}
	};
}

// Takens a list of stops and creates the html elements to go on the side bar
// also handles events on the side bar for destroying stops and changing names
function renderSideBar (route, map) {
	// makes the html for a side bar elem with the given name
	var makeSideBarElement = function (name) {
		var s = "<li class='stop-listitem center-vert'>" +
					"<div class='name-input hidden'>" +
						"<input type='text' maxlength='30'>" +  
						"<button class='btn btn-link pull-right'><i class='icon-remove'></i></button>" +
					"</div>" +
					"<div class='name-display'>" + name + "</div><br/>" +
				"</li>";
		return $(s);	// <-- this tells jquery to make the DOM element, so its not just a string of html
	};

	// clear old stuff in the side bar:
	$('#route-list').empty();

	// extract the list of stops from the route
	var jstops = route.getAllStops();

	// for each stop, make its corresponding html, and set touch events to link the two
	_.each(jstops, function (stop) {
		// make html
		var sideBarElem = makeSideBarElement(stop.getName());

		// handle mouseover events by displaying the text input and hiding the name
		sideBarElem.mouseover(function (e) {
			$(this).find('.name-input').toggleClass("hidden", false).find('input').val(stop.getName());
			$(this).find('.name-display').toggleClass("hidden", true);
		});

		// when mouse leaves the area, hide the text input, show the name
		sideBarElem.mouseout(function (e) {
			$(this).find('.name-input').toggleClass("hidden", true);
			$(this).find('.name-display').toggleClass("hidden", false);
		});

		sideBarElem.find('button').click(function (e) {
			console.log('you clicked the exit');
			// renderSideBar(route.removeStop(stop));
			route.removeStop(stop);
		});

		// when the user types in the text box, update the name of the stop
		sideBarElem.find('input').change(function (e) {
			var newName = $(this).val();
			stop.setName(newName);
			sideBarElem.find('.name-display').html(newName);
		});

		// actually append this newly created element to the DOM
		$('#route-list').append(sideBarElem);
	});	
	 
	 
	 $('#createbutton').click( function(e) {
	 alert("Please click on the map where you want this new stop to be.");
     var newStopMaker = Microsoft.Maps.Events.addHandler(map, 'click', function (e)
    {
      if(e.targetType == "map") {
		console.log("New Stop Protocol Activated!");
		var point = new Microsoft.Maps.Point(e.getX(), e.getY());
        var loc = e.target.tryPixelToLocation(point);
        newStop = new JStop(map, {loc:new Microsoft.Maps.Location(loc.latitude, loc.longitude), isMarker:true, name:"New Stop"});

        var sideBarElem = makeSideBarElement(newStop.getName());

        // handle mouseover events by displaying the text input and hiding the name
        sideBarElem.mouseover(function (e) {
        $(this).find('.name-input').toggleClass("hidden", false).find('input').val(newStop.getName());
        $(this).find('.name-display').toggleClass("hidden", true);
        });

        // when mouse leaves the area, hide the text input, show the name
        sideBarElem.mouseout(function (e) {
        $(this).find('.name-input').toggleClass("hidden", true);
        $(this).find('.name-display').toggleClass("hidden", false);
        });

        console.log("buttons found: " + sideBarElem.find('button').length);
        sideBarElem.find('button').click(function (e) {
          console.log('you clicked the exit');
          // delete the stop!
        });

        // when the user types in the text box, update the name of the stop
        sideBarElem.find('input').change(function (e) {
          var newName = $(this).val();
          newStop.setName(newName);
				/*
					If we end up posting new stops, should we add that post code over here?
						This way, these new stops can be permanent. Perhaps we can even make it so that
						whenever a stop's name is changed, a prompt appears, asking if we want those
						changes to be permanent?
				*/
          sideBarElem.find('.name-display').html(newName);
        });

          // actually append this newly created element to the DOM
          $('#route-list').append(sideBarElem);
		  Microsoft.Maps.Events.removeHandler(newStopMaker);
      }});
    
  });
  
  
}
