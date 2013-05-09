

/*
	A JStop represents a stop on the joey. The key pieces of data include:
		1) Location
		2) The pin thats put on the map
		3) The name of the stop
		4) The popup that displays the name

	From outside this class, you can only use the public functions under the public section
 */

function JStop(map, stop) {
	/**************
		Private
	**************/

	// removes spaces from given string -- allows for names that have spaces, but one word IDs
	var killSpaces = function (str) { return str.replace(/\s/g, ''); }
	// just appends a # to the given string without spaces
	var idForName = function (name) { return "#" + killSpaces(name); }

	var name, location, pin, $popover, poptions;

	// Get and set the name of the stop
	var getName = function () { return name };
	var setName = function (newName) {
		name = newName
		render();
	};

	// Get and set the location of the stop. Im not sure this is actually used.
	var getLocation = function () { return location };
	var setLocation = function (newLoc) {
		location = newLoc;
		render();
	};

	// draws fresh popover and pin to the given map
	// destroys the old popover and pin and creates them anew.
	var render = function () {
		console.log("rendering jstop!");

		// remove old pin and popover:
		if (pin) { map.entities.remove(pin); }
		if ($popover) { $popover.popover('destroy'); }

		pin = undefined;

		// pin = new Microsoft.Maps.Pushpin(getLocation(), {
		// 	htmlContent: "<div tabindex='0' id='" + killSpaces(getName()) + "'><img src='assets/marker.png'></div>",
		// 	draggable: true
		// });
		//map.entities.push(pin);

		poptions = {
			html: true,
			content: "<div tabindex='0' id='popover-" + getName() + "'>" + getName() + "</div>",
			trigger:'manual',
			placement: 'top'
		};
		$popover = $(idForName(getName())).popover(poptions);
		//$popover.popover('show');
	}

	name = stop.name;
	location = stop.loc;
	render();

	// display popup whenver pin dragging ends (cause it hides while dragging)
	Microsoft.Maps.Events.addHandler(pin, 'dragend', function (p) {
		//$(idForName(name)).popover(poptions).popover('show')
	});

	// display popup whenever the map stops panning
	// This needs more complicated logic to prevent it from blipping off and then on
	Microsoft.Maps.Events.addHandler(map, 'viewchangeend', function (p) {
		console.log("view change ended");
		//$(idForName(name)).popover(poptions).popover('show')
	});

	/**************
		Public
	**************/

	// Export public versions of the helper functions defined above
	this.getLocation = getLocation;
	this.setLocation = setLocation;
	this.getName = getName;
	this.setName = setName;
	this.getPin = function () { return pin; }
	this.getPopover = function () { return $popover; }
	this.killPopover = function () { $popover.popover('destroy'); }
}
/*
<<<<<<< HEAD



	// Debugging statements:
	console.log("# of DOM Elems with ID = " + idForName(stop.name) + " = " + $(idForName(stop.name)).length);
	console.log("id to jquery for: " + id + ". DOM elems with that id: " + $(id).length);
=======
>>>>>>> 757e0e6d84db6607c22b3bcddb2c3fd0e0983916
*/
