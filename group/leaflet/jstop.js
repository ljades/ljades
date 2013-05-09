
/*
 Given stop:
 0) Make a pin
 1) Make a corresponding info box
 2) Attach infobox to pin
 3) Make pin's movements move the infow window
 */

// Represents a stop on the joey, which contains a location, a pushpin, and an infowindow.
// Returns undefined for non-marker stops
function JStop(stop) {
	if (stop.isMarker !== true) {
		return;
	}

	var infoboxForStop = function (stop) {
		var infobox = new Microsoft.Maps.Infobox(stop.loc, {
			title:stop.name, 
			width:200, 
			height:35
		});
		return infobox;
	};

	var pinForStop = function (stop) {
		var pin =  new Microsoft.Maps.Pushpin(stop.loc, {
			draggable: true,
			//icon: "blackicon.png"
			//htmlContent: $("<div>" + stop.name + "</div>").addClass('pushpin').html()
		});
		return pin;
	};

	var setVisible = function (obj, visible) {
		obj.setOptions({visible: visible});
	};

	var loc = stop.loc;
	var box = infoboxForStop(stop);
	var pin = pinForStop(stop, box);

	map.entities.push(box);
	map.entities.push(pin);

	Microsoft.Maps.Events.addHandler(pin, 'drag', function (p) {
		box.setLocation(pin.getLocation());
	});

	Microsoft.Maps.Events.addHandler(pin, 'click', function (p) {
		setVisible(pin, false);
		setVisible(box, true);
	});

	this.loc = loc;
	this.pin = pin;
	this.box = box;
}

// var html = $("<p>" + stop.name + "</p>").addClass("pushpin");
