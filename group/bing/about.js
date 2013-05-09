$(document).ready(function(){
	animatePeople();
});

function animatePeople(){
	var rrand = Math.floor(Math.random()*20-10);
	var lrand = Math.floor(Math.random()*20-10);
	$('.person').animate({top:'+=' + rrand, left:'+=' + lrand}, 500,'linear', animatePeople);
};