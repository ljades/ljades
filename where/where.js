function beginall()
{

	try { drawMap(); }
	catch(err) { alert("GMaps isn't supported with your browser!"); }
	getLocation();
	drawRed();
	whereIs();
}


function drawMap()
{
	myLat = 42.406791;
	myLon = -71.120553;
	TuftsCoords = [myLat, myLon];
	myOptions = {
          center: new google.maps.LatLng(TuftsCoords[0], TuftsCoords[1]),
          zoom: 12,
          mapTypeId: google.maps.MapTypeId.ROADMAP
        };
	map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
}


function drawRed()
{
	var str = '[{"Line": "Red", "PlatformKey": "RALEN", "PlatformName": "ALEWIFE NB", "StationName":"ALEWIFE", "PlatformOrder":17, "Branch":"Trunk", "Direction":"NB", "stop_name":"Alewife Station", "stop_lat":42.395428, "stop_lon":-71.142483}, {"Line": "Red", "PlatformKey": "RDAVN", "PlatformName": "DAVIS NB", "StationName":"DAVIS", "PlatformOrder":16, "Branch":"Trunk", "Direction":"NB", "stop_name":"Davis Station", "stop_lat":42.39674, "stop_lon":-71.121815}, {"Line": "Red", "PlatformKey": "RDAVS", "StationName":"DAVIS", "PlatformOrder":1, "Branch":"Trunk", "Direction":"SB", "stop_name":"Davis Station", "stop_lat":42.39674, "stop_lon":-71.121815 }, {"Line": "Red", "PlatformKey": "RPORN", "stop_name":"Porter Square Station", "stop_lat":42.3884, "stop_lon":-71.119149 }, {"Line": "Red", "PlatformKey": "RPORS", "stop_name":"Porter Square Station", "stop_lat":42.3884, "stop_lon":-71.119149 }, {"Line": "Red", "PlatformKey": "RHARN", "stop_name":"Harvard Square Station", "stop_lat":42.373362, "stop_lon":-71.118956 }, {"Line": "Red", "PlatformKey": "RHARS", "stop_name":"Harvard Square Station", "stop_lat":42.373362, "stop_lon":-71.118956 }, {"Line": "Red", "PlatformKey": "RCENN", "stop_name":"Central Square Station", "stop_lat":42.365486, "stop_lon":-71.103802 }, {"Line": "Red", "PlatformKey": "RCENS", "stop_name":"Central Square Station", "stop_lat":42.365486, "stop_lon":-71.103802 }, {"Line": "Red", "PlatformKey": "RKENN", "stop_name":"Kendall/MIT Station", "stop_lat":42.36249079, "stop_lon":-71.08617653 }, {"Line": "Red", "PlatformKey": "RKENS", "stop_name":"Kendall/MIT Station", "stop_lat":42.36249079, "stop_lon":-71.08617653 }, {"Line": "Red", "PlatformKey": "RMGHN", "stop_name":"Charles/MGH Station", "stop_lat":42.361166, "stop_lon":-71.070628 }, {"Line": "Red", "PlatformKey": "RPRKN", "stop_name":"Park St. Station", "stop_lat":42.35639457, "stop_lon":-71.0624242 }, {"Line": "Red", "PlatformKey": "RPRKS", "stop_name":"Park St. Station", "stop_lat":42.35639457, "stop_lon":-71.0624242 }, {"PlatformKey": "RDTCN", "stop_name":"Downtown Crossing Station", "stop_lat":42.355518, "stop_lon":-71.060225 }, {"PlatformKey": "RDTCS", "stop_name":"Downtown Crossing Station", "stop_lat":42.355518, "stop_lon":-71.060225 }, {"PlatformKey": "RSOUN", "stop_name":"South Station", "stop_lat":42.352271, "stop_lon":-71.055242 }, {"PlatformKey": "RSOUS", "stop_name":"South Station", "stop_lat":42.352271, "stop_lon":-71.055242 }, {"PlatformKey": "RBRON", "stop_name":"Broadway Station", "stop_lat":42.342622, "stop_lon":-71.056967 }, {"PlatformKey": "RBROS", "stop_name":"Broadway Station", "stop_lat":42.342622, "stop_lon":-71.056967 }, {"PlatformKey": "RANDN", "stop_name":"Andrew Station", "stop_lat":42.330154, "stop_lon":-71.057655 }, {"PlatformKey": "RANDS", "stop_name":"Andrew Station", "stop_lat":42.330154, "stop_lon":-71.057655 }, {"PlatformKey": "RJFKN", "stop_name":"JFK/UMass Station", "stop_lat":42.320685, "stop_lon":-71.052391 }, {"PlatformKey": "RJFKS", "stop_name":"JFK/UMass Station", "stop_lat":42.320685, "stop_lon":-71.052391 }, {"PlatformKey": "RSAVN", "stop_name":"Savin Hill Station", "stop_lat":42.31129, "stop_lon":-71.053331 }, {"PlatformKey": "RSAVS", "stop_name":"Savin Hill Station", "stop_lat":42.31129, "stop_lon":-71.053331 }, {"PlatformKey": "RFIEN", "stop_name":"Fields Corner Station", "stop_lat":42.300093, "stop_lon":-71.061667 }, {"PlatformKey": "RFIES", "stop_name":"Fields Corner Station", "stop_lat":42.300093, "stop_lon":-71.061667 }, {"PlatformKey": "RSHAN", "stop_name":"Shawmut Station", "stop_lat":42.29312583, "stop_lon":-71.06573796 }, {"PlatformKey": "RSHAS", "stop_name":"Shawmut Station", "stop_lat":42.29312583, "stop_lon":-71.06573796 }, {"PlatformKey": "RASHS", "stop_name":"Ashmont Station", "stop_lat":42.284652, "stop_lon":-71.064489 }, {"PlatformKey": "RNQUN", "stop_name":"North Quincy Station", "stop_lat":42.275275, "stop_lon":-71.029583 }, {"PlatformKey": "RNQUS", "stop_name":"North Quincy Station", "stop_lat":42.275275, "stop_lon":-71.029583 }, {"PlatformKey": "RWOLN", "stop_name":"Wollaston Station", "stop_lat":42.2665139, "stop_lon":-71.0203369 }, {"PlatformKey": "RWOLS", "stop_name":"Wollaston Station", "stop_lat":42.2665139, "stop_lon":-71.0203369 }, {"PlatformKey": "RQUCN", "stop_name":"Quincy Center Station", "stop_lat":42.251809, "stop_lon":-71.005409 }, {"PlatformKey": "RQUCS", "stop_name":"Quincy Center Station", "stop_lat":42.251809, "stop_lon":-71.005409 }, {"PlatformKey": "RQUAN", "stop_name":"Quincy Adams Station", "stop_lat":42.233391, "stop_lon":-71.007153 }, {"PlatformKey": "RQUAS", "stop_name":"Quincy Adams Station", "stop_lat":42.233391, "stop_lon":-71.007153 }, {"PlatformKey": "RBRAS", "stop_name":"Braintree Station", "stop_lat":42.2078543, "stop_lon":-71.0011385 } ]';
	redLine = JSON.parse(str);

	var len = redLine.length;
	prevStopName = " ";
	redPosition = Array();
	redMarker = Array();
	redInfoWindow = Array();
	redDist = Array();
	minDistance = findDistance(redLine[0].stop_lat, redLine[0].stop_lon, myLat, myLon);
	minIndex = 0;
	for(var i = 0; i < len; i = i + 1) //render markers
	{
		if(prevStopName != redLine[i].stop_name)
		{
			redPosition[i] = new google.maps.LatLng(redLine[i].stop_lat, redLine[i].stop_lon)
			redMarker[i] = new google.maps.Marker({
				position: redPosition[i],
				title: redLine[i].stop_name,
				icon: "icons/tlineIcon.png"
			});
			
			redMarker[i].setMap(map);
			
			var j = i;
			redInfoWindow[j] = new google.maps.InfoWindow();
			redDist[j] = findDistance(redLine[j].stop_lat, redLine[j].stop_lon, myLat, myLon);
			if(redDist[j] < minDistance)
				{
					minDistance = redDist[j];
					minIndex = j;
				}
			infoWindowOp(j);
			prevStopName = redLine[i].stop_name;
		}
		else
		{
			redInfoWindow[i] = 0;
		}
		
	}
	redInfoWindow[minIndex].setContent( redMarker[minIndex].title  + "<br/> This station is closest to you. It is " + redDist[minIndex] + "  miles from you. <br/>");
	redInfoWindow[minIndex].open(map, redMarker[minIndex]);
	trainFeed();
	
	
	redPath = Array(); //creating array of the right locations
	var lenOfPath = 0;
	for(var i = 0; i < len; i = i + 1)
	{
		if(redPosition[i])
		{
			redPath[lenOfPath] = redPosition[i];
			lenOfPath = lenOfPath + 1;
		}
	}
	var oneRedPath = Array();
	for(var i = 0; i < (lenOfPath - 5); i++)
	{
		oneRedPath[i] = redPath[i];
	}
	var twoRedPath = Array();
	var lenOfSecond = 0;
	twoRedPath[lenOfSecond] = redPath[lenOfPath-10];
	lenOfSecond = lenOfSecond + 1;
	for(var i = (lenOfPath - 5); i < lenOfPath; i++)
	{
		twoRedPath[lenOfSecond] = redPath[i];
		lenOfSecond = lenOfSecond + 1;
	}
	
	var TPath1 = new google.maps.Polyline({
		path: oneRedPath,
		strokeColor: "red",
		strokeOpacity: 1.0,
		strokeWeight: 5
	});
	TPath1.setMap(map);

	var TPath2 = new google.maps.Polyline({
		path: twoRedPath,
		strokeColor: "red",
		strokeOpacity: 1.0,
		strokeWeight: 5
	});
	TPath2.setMap(map);
}

function infoWindowOp(indexforInfo)
{
	redInfoWindow[indexforInfo].setContent( redMarker[indexforInfo].title  + "<br/> This stop is " + redDist[indexforInfo] + "  miles from you. <br/>");
new google.maps.event.addListener(redMarker[indexforInfo], 'click', function() {
	
	redInfoWindow[indexforInfo].open(map, redMarker[indexforInfo]);
	});
}


function getLocation()
{
if(navigator.geolocation)
{
			navigator.geolocation.getCurrentPosition(function(position) {
				myLat = position.coords.latitude;
				myLon = position.coords.longitude;
			});
		myLoc = new google.maps.LatLng(myLat, myLon);
		map.panTo(myLoc);
		myMarker = new google.maps.Marker({
					position: myLoc,
					title: "You are here!",
					icon: "icons/person.png"
				});
				myMarker.setMap(map);

		myInfoWindow = new google.maps.InfoWindow();
				// Open info window on click of marker
		google.maps.event.addListener(myMarker, 'click', function() {
			myInfoWindow.setContent(myMarker.title);
			myInfoWindow.open(map, myMarker);
		});
}
else
{
	alert("geolocation isn't supported. ");
}
}

function findDistance(lat1, lon1, lat2, lon2)
{

var R = 3958.756; // miles
var dLat = (lat2-lat1) * Math.PI/180;
var dLon = (lon2-lon1) * Math.PI/180;
var lat1 = lat1 * Math.PI/180;
var lat2 = lat2 * Math.PI/180;

var a = Math.sin(dLat/2) * Math.sin(dLat/2) +
        Math.sin((dLon/2)) * Math.sin(dLon/2) * Math.cos(lat1) * Math.cos(lat2); 
var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a)); 
var d = R * c;

return d;

}


function trainFeed()
{
	schedule = '[]';
		try
			{	
				request = new XMLHttpRequest();
			}
		catch(ms1) {
			try {
				request = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (ms2) {
				try {
					request = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch (ex) {
					request = null;
				}
			}
		}
		
		if (request == null) {
				alert("Error creating request object --Ajax not supported?");
		}
		
		else {
			try {
				request.open("GET", "http://mbtamap-cedar.herokuapp.com/mapper/redline.json", true);
				request.send(null);
				request.onreadystatechange = function changer() {
					if (request.readyState == 4 && request.status == 200)
					{
						schedule = request.responseText;
						feedofTrains = JSON.parse(schedule);
						sortFeed(feedofTrains);
						
						
						for(var i = 0; i < feedofTrains.length; i = i + 1)
						{
							keyToFind = feedofTrains[i].PlatformKey;
							if(feedofTrains[i].InformationType == "Predicted")
							{
								for(var j = 0; j < redLine.length; j = j + 1)
								{
									if(keyToFind == redLine[j].PlatformKey)
									{
										isSouth = "Northbound";
										if(j == 0) //the first is the only one that only goes southbound
										{
											isSouth = "Southbound";
										}
										if(!redInfoWindow[j])
										{
											isSouth = "Southbound";
											j = j - 1;
										}	
										contentToGet = redInfoWindow[j].content;
										redInfoWindow[j].setContent(contentToGet + "- " + isSouth + " Train " + feedofTrains[i].Trip + " arrives in " + feedofTrains[i].TimeRemaining + ". <br/>");
										j = redLine.length;
									}
								}
							}	
						}
					}
				}
			}
			catch (err)
				{
					alert("Error: " + err + ". Faulty line.");
			}		
		}
		
}

function sortFeed(feedofTrains)
{ //switch sort
	
	for(var i = 0; i < (feedofTrains.length-1); i = i + 1)
	{
		minIndex = i;
		minTime = feedofTrains[i].TimeRemaining;
		for(var j = i + 1; j < feedofTrains.length; j = j + 1)
		{
			if(feedofTrains[j].TimeRemaining < minTime)
			{
				minTime = feedofTrains[j].TimeRemaining;
				minIndex = j;
			}
		}
		temp = feedofTrains[i];
		feedofTrains[i] = feedofTrains[minIndex];
		feedofTrains[minIndex] = temp;
	}
}

function whereIs()
{
		try
			{	
				whereRequest = new XMLHttpRequest();
			}
		catch(ms1) {
			try {
				whereRequest = new ActiveXObject("Msxml2.XMLHTTP");
			}
			catch (ms2) {
				try {
					whereRequest = new ActiveXObject("Microsoft.XMLHTTP");
				}
				catch (ex) {
					whereRequest = null;
				}
			}
		}
		
		if (whereRequest == null) {
				alert("Error creating request object --Ajax not supported?");
		}
		
		else {
			try {
				whereRequest.open("GET", "http://messagehub.herokuapp.com/a3.json", true);
				whereRequest.send(null);
				whereRequest.onreadystatechange = function changer() {
					if (whereRequest.readyState == 4 && whereRequest.status == 200)
					{
						theResponse = whereRequest.responseText;
						theWheres = JSON.parse(theResponse);
						whereNode = document.getElementById("whereInfo");
						for(var j = 0; j < theWheres.length; j = j + 1)
						{
							if(theWheres[j].name == "Waldo")
							{
								WaldoIsMe = theWheres[j];
								WaldoPos = new google.maps.LatLng(WaldoIsMe.loc.latitude, WaldoIsMe.loc.longitude);
								WaldoMark = new google.maps.Marker({
									position: WaldoPos,
									title: WaldoIsMe.name,
									icon: "icons/waldo.png"
								});
								WaldoMark.setMap(map);
								WaldoInfo = new google.maps.InfoWindow();
								new google.maps.event.addListener(WaldoMark, 'click', function() {
								WaldoInfo.setContent(WaldoMark.title + " <br/> " + WaldoIsMe.loc.note + "<br/> " + findDistance(WaldoPos.lat(), WaldoPos.lng(), myLat, myLon) + " miles from you. <br/>");
								WaldoInfo.open(map, WaldoMark); });
								whereNode.innerHTML = whereNode.innerHTML + "Waldo is: " + findDistance(WaldoPos.lat(), WaldoPos.lng(), myLat, myLon) + " miles from you. <br/>";
							}
							
							if(theWheres[j].name == "Carmen Sandiego")
							{
								CarmenIsMe = theWheres[j];
								CarmenPos = new google.maps.LatLng(CarmenIsMe.loc.latitude, CarmenIsMe.loc.longitude);
								CarmenMark = new google.maps.Marker({
									position: CarmenPos,
									title: CarmenIsMe.name,
									icon: "icons/carmen.png"
								});
								CarmenMark.setMap(map);
								CarmenInfo = new google.maps.InfoWindow();
								new google.maps.event.addListener(CarmenMark, 'click', function() {
								CarmenInfo.setContent(CarmenMark.title + " <br/> " + CarmenIsMe.loc.note + " <br/> " + findDistance(CarmenPos.lat(), CarmenPos.lng(), myLat, myLon) + " miles from you. <br/>");
								CarmenInfo.open(map, CarmenMark); });
								whereNode.innerHTML = whereNode.innerHTML + "Carmen is: " + findDistance(CarmenPos.lat(), CarmenPos.lng(), myLat, myLon) + " miles from you. <br/>";
							}
						}
					}
			
					}
				}
			catch (err)
				{
					alert("Error: " + err + ". Faulty line.");
			}		
		}
}

//Things left to do:
//Uncomment myLocation stuff, test on kublai.		