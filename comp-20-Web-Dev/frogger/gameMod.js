// Your work goes here...


function start_game()
	{
		canvas = document.getElementById('game');
		
		if(canvas.getContext)
		{
			ctx = canvas.getContext('2d');
			build_static();
			timePassed = 0;
			lives = 5;
			level = 1;
			numCars = 2;
			numLogs = 3;
			logSpeedStandard = 20;
			carSpeedStandard = 20;
			logSpeed = 20;
			carSpeed = 20;
			score = 0;
			time = 0;
			init_frog_x = 177;
			init_frog_y = 496;
			frog_x = init_frog_x;
			x_increment = 22;
			y_increment = 35;
			frog_y = init_frog_y;
			highscore = 200;
			timer = 30000;
			timer_remaining = 30000;
			deathClock = 1000;
			deathClock_remaining = 0;
			gameOverCount = 0;
			gameOverCountStandard = 6000;
			death_x = 0;
			death_y = 0;
			timeDelay = 40;
			minFrog_y = 1000;
			flyCounter = 0;
			flyCountStart = 7000;
			next1up = 10000;
			wonHere = Array();
			flyHere = Array();
			for(var i = 0; i < 5; i = i + 1)
			{
				wonHere[i] = false;
			}
			for(var i = 0; i < 5; i = i + 1)
			{
				flyHere[i] = false;
			}
			document.addEventListener("keydown", function(event) {
				if ((event.keyCode == 37) && (frog_x - x_increment >= 0)) {
					frog_x = frog_x - x_increment;
				}
				else if(event.keyCode == 38) //up
				{
					frog_y = frog_y - y_increment;
					if(minFrog_y > frog_y)
					{
						minFrog_y = frog_y;
						score = score + 10;
					}
				}
				else if(event.keyCode == 39)
				{
					if((frog_x + 2*x_increment <= 398))
						frog_x = frog_x + x_increment;
					else
						frog_x = 397 - x_increment;
				}
				else if(event.keyCode == 40 && frog_y != init_frog_y)
				{
					frog_y = frog_y + y_increment;
				}
			});
			setInterval(build_static, timeDelay);
		}
		
		else
		{
			alert('Sorry, your browser kinda sucks...');
		}
	}
	
	function build_static()
	{
		ctx.fillStyle = "#191970"; //for blue
		ctx.fillRect(1, 1, 398, 565);
		ctx.fillStyle = "#000000";
		ctx.fillRect(1, 283, 398, 565);
		var img = new Image;
		img.src = "assets/frogger_sprites.png";
		img.onload = function() {
		ctx.drawImage(img, 0, 0, 398, 112, 0, 0, 398, 112); //title + grass
		ctx.drawImage(img, 0, 120, 398, 32, 0, 283, 398, 32); //top purple roadside
		ctx.drawImage(img, 0, 120, 398, 32, 0, 495, 398, 32); //bottom purple roadside
		}
		build_dynamic();
	}
	function build_dynamic()
	{
		var img = new Image;
		img.src = "assets/frogger_sprites.png";
		img.onload = function(){
		for(var i = 0; i < lives; i = i + 1)
		{
			ctx.drawImage(img, 42, 334, 26, 24, 13*i, 530, 15, 16);
		}
		winCount = 0;
		if (wonHere[0]){
			if(flyHere[0])
			{
				flyCounter = 1;
				score = score + 200;
			}
			ctx.drawImage(img, 113, 366, 22, 25, 16, 78, 22, 25);
			winCount = winCount + 1;
		}
		if (flyHere[0]){
			ctx.drawImage(img, 138, 234, 19, 19, 16, 78, 19, 19);
		}
		if (wonHere[1]){
			if(flyHere[1])
			{
				flyCounter = 1;
				score = score + 200;
			}
			ctx.drawImage(img, 113, 366, 22, 25, 102, 78, 22, 25);
			winCount = winCount + 1;
		}
		if (flyHere[1]){
			ctx.drawImage(img, 138, 234, 19, 19, 102, 78, 19, 19);
		}
		if (wonHere[2]){
			if(flyHere[2])
			{
				flyCounter = 1;
				score = score + 200;
			}
			ctx.drawImage(img, 113, 366, 22, 25, 186, 78, 22, 25);
			winCount = winCount + 1;
		}
		if (flyHere[2]){
			ctx.drawImage(img, 138, 234, 19, 19, 186, 78, 19, 19);
		}
		if (wonHere[3]){
			if(flyHere[3])
			{
				flyCounter = 1;
				score = score + 200;
			}
			ctx.drawImage(img, 113, 366, 22, 25, 270, 78, 22, 25);
			winCount = winCount + 1;
		}
		if (flyHere[3]){
			ctx.drawImage(img, 138, 234, 19, 19, 270, 78, 19, 19);
		}
		if (wonHere[4]){
			if(flyHere[4])
			{
				flyCounter = 1;
				score = score + 200;
			}
			ctx.drawImage(img, 113, 366, 22, 25, 356, 78, 22, 25);
			winCount = winCount + 1;
		}
		if (flyHere[4]){
			ctx.drawImage(img, 138, 234, 19, 19, 356, 78, 19, 19);
		}
		
		if(flyCounter == 1)
		{
			flyCounter = 0;
			for(var i = 0; i < 5; i = i + 1)
			{
				flyHere[i] = false;
			}
		}
		if(flyCounter > 1)
		{
			if(flyCounter - timeDelay < 1)
			{
				flyCounter = 1;
			}
			else
				flyCounter = flyCounter - timeDelay;
		}
		
		if (winCount == 5)
		{
			winCount = 0;
			for(var i = 0; i < 5; i = i + 1)
			{
				wonHere[i] = false;
				flyHere[i] = false;
			}
			level = level + 1;
			if(level == 2)
				numCars = numCars + 1;
			if(level == 3)
				numLogs = numLogs - 1;
			carSpeed = carSpeed + 8;
			logSpeed = logSpeed + 8;
			score = score + 1000;
			flyCounter = 0;
			deathClock = 0;
		}
		if(gameOverCount > 0)
		{
			gameOverCount = gameOverCount - timeDelay;
			ctx.fillStyle = "rgb(255,0,0)";
			ctx.font = "25pt Arial bold";
			ctx.fillText("GAME OVER", 75, 250);
			return;
		}
		ctx.fillStyle = "rgb(0, 255, 0)";
		ctx.font = "16pt Arial bold";
		ctx.fillText("Level " + level, 130, 545);
		ctx.font = "10pt Arial bold";
		ctx.fillText("Score: " + Math.floor(score), 0, 560);
		ctx.fillText("Highscore: " + Math.floor(highscore), 130, 560);
		ctx.fillText("Timer: " + (timer_remaining/1000).toFixed(1), 300, 560);
		carSpace = 80;
		truckSpace = 100;
		bigLogSpace = 190;
		medLogSpace = 140;
		smaLogSpace = 120;
		log_x = 30;
		log_med_pos = 398 - ((log_x + (logSpeed * timePassed / 350)) % 398);
		log_big_pos = (log_x + (logSpeed * timePassed / 400)) % 398;
		log_sma_pos = (log_x + (logSpeed * timePassed / 380)) % 398;
		log_y = 110;
		log_separation = 36;
		car_x1 = 300;
		car_x1_pos = 398 - ((car_x1 + ( carSpeed * timePassed / 370)) % 398);
		car_x2 = 140;
		car_x2_pos = (car_x2 + (carSpeed * timePassed / 350)) % 398;
		car_x3 = 200;
		car_x3_pos = 398 - ((car_x3 + ( carSpeed * timePassed / 480)) % 398);
		x3_remains_pos = (((car_x3_pos + (carSpace * i)) % 398) + 50);
		hasDrowned = false;
		
		car_x4 = 100;
		car_x4_pos = (car_x4 + (carSpeed * timePassed / 480)) % 398;
		car_x5 = 220;
		car_x5_pos = 398 - ((car_x5 + ( carSpeed * timePassed / 440)) % 398);
		car_y = 320; 
		car_separation = 36;
		
		frog_center_x = frog_x + 11;
		frog_center_y = frog_y + 13;
		
		//death animation.
		if (deathClock_remaining > 0)
		{
			var deathImg = new Image;
			deathImg.src = "assets/dead_frog.png";
			deathImg.onload = function () {
				ctx.drawImage(deathImg, 0, 0, 30, 30, death_x - 3, death_y - 3, 30, 30);
				deathClock_remaining = deathClock_remaining - timeDelay;
				console.log("I drew!");
			}
		}
		//build and animate cars
		for(var i = 0; i < numCars; i = i + 1){
			place_x = (car_x1_pos + (carSpace * i)) % 398;
			ctx.drawImage(img, 82, 263, 24, 27, place_x, car_y, 24, 27); //vehicle 1
			//collision check
			if (frog_center_y >= car_y && frog_center_y <= (car_y + 27) && frog_center_x >= place_x && frog_center_x <= place_x + 24)
			{
				deadFrog();
			}
			
			x1_remains_pos = (((car_x1_pos + (carSpace * i)) % 398) + 24);
			
			if(x1_remains_pos > 398){
				end_place_x = 1 - (398 - x1_remains_pos);
				ctx.drawImage(img, 106 + 398 - x1_remains_pos, 263,end_place_x - 1, 27, 1, car_y, end_place_x - 1, 27); 
				//do another collision check
				if (frog_center_y >= car_y && frog_center_y <= (car_y + 27) && frog_center_x >= 0 && frog_center_x <= end_place_x)
				{
					deadFrog();
				}
			}
			
			place_x = (car_x2_pos + (carSpace * i)) % 398;
			ctx.drawImage(img, 44, 263, 26, 27, place_x, car_y + car_separation, 26, 27); //vehicle 2
			//collision check
			
			if (frog_center_y >= car_y + car_separation && frog_center_y <= (car_y + car_separation + 27) && frog_center_x >= place_x && frog_center_x <= place_x + 26)
			{
				deadFrog();
			}
			
			x2_remains_pos = (((car_x2_pos + (carSpace * i)) % 398) + 26);
			if(x2_remains_pos > 398){
				end_place_x = 1 - (398 - x2_remains_pos);
				ctx.drawImage(img, 70 + 398 - x2_remains_pos, 263,end_place_x - 1, 27, 1, car_y + (1 * car_separation), end_place_x - 1, 27); 
				//do another collision check
				if (frog_center_y >= car_y + car_separation && frog_center_y <= (car_y + car_separation + 27) && frog_center_x >= 0 && frog_center_x <= end_place_x)
				{
					deadFrog();
				}
			}
			
			place_x = (car_x3_pos + (truckSpace * i)) % 398;
			ctx.drawImage(img, 105, 300, 50, 22, place_x, car_y + (2 * car_separation), 50, 26);
			//collision check
			if (frog_center_y >= car_y + 2*car_separation && frog_center_y <= (car_y + 2*car_separation + 27) && frog_center_x >= place_x && frog_center_x <= place_x + 50)
			{
				deadFrog();
			}
			
			x3_remains_pos = (((car_x3_pos + (truckSpace * i)) % 398) + 50);
			if(x3_remains_pos > 398){
				end_place_x = 1 - (398 - x3_remains_pos);
				ctx.drawImage(img, 155 + 398 - x3_remains_pos, 300,end_place_x - 1, 22, 1, car_y + (2 * car_separation), end_place_x - 1, 26); 
				//do another collision check
				if (frog_center_y >= car_y + 2*car_separation && frog_center_y <= (car_y + 2*car_separation + 27) && frog_center_x >= 0 && frog_center_x <= end_place_x)
				{
					deadFrog();
				}
			}
			
			tractor_check = ((timePassed / timeDelay) % 3);
			if(tractor_check == 0)
				tractor_x = 10;
			if(tractor_check == 1)
				tractor_x = 42;
			if(tractor_check == 2)
				tractor_x = 73;
			place_x = (car_x4_pos + (carSpace * i)) % 398;
			ctx.drawImage(img, tractor_x, 300, 24, 22, place_x, car_y + (3 * car_separation), 24, 26);
			//collision check
			if (frog_center_y >= car_y + 3*car_separation && frog_center_y <= (car_y + 3*car_separation + 26) && frog_center_x >= place_x && frog_center_x <= place_x + 24)
			{
				deadFrog();
			}
			
			x4_remains_pos = (((car_x4_pos + (carSpace * i)) % 398) + 24);
			if(x4_remains_pos > 398){
			end_place_x = 1 -(398 - x4_remains_pos);
				ctx.drawImage(img, 22 + tractor_x + 398 - x4_remains_pos, 300,end_place_x - 1, 22, 1, car_y + (3 * car_separation), end_place_x - 1, 26); 
				//do another collision check
				if (frog_center_y >= car_y + 3*car_separation && frog_center_y <= (car_y + 3*car_separation + 27) && frog_center_x >= 0 && frog_center_x <= end_place_x)
				{
					deadFrog();
				}
			}
			place_x = (car_x5_pos + (carSpace * i)) % 398;
			ctx.drawImage(img, 8, 263, 30, 27, place_x, car_y + (4 * car_separation), 30, 27);
			//collision check
			if (frog_center_y >= car_y + 4*car_separation && frog_center_y <= (car_y + 4*car_separation + 26) && frog_center_x >= place_x && frog_center_x <= place_x + 30)
			{
				deadFrog();
			}
			x5_remains_pos = (((car_x5_pos + (carSpace * i)) % 398) + 30);
			if(x5_remains_pos > 398){
				end_place_x = 1 -(398 - x5_remains_pos);
				ctx.drawImage(img, 38 + 398 - x5_remains_pos, 263,end_place_x - 1, 27, 1, car_y + (4 * car_separation), end_place_x - 1, 27); 
				//do another collision check
				if (frog_center_y >= car_y + 4*car_separation && frog_center_y <= (car_y + 4*car_separation + 27) && frog_center_x >= 0 && frog_center_x <= end_place_x)
				{
					deadFrog();
				}
			}
		}
		
		//build logs and their animations:
		
		if(frog_center_y >= 110 && frog_center_y <= 276)
			hasDrowned = true;
			
		for(var i = 0; i < 2; i = i + 1) //big log
		{
			place_x = (log_big_pos + (bigLogSpace * i)) % 398;
			ctx.drawImage(img, 5, 165, 180, 22, place_x, log_y + (log_separation * 2), 160, 22);
			//collision check
			if (frog_center_y >= log_y + 2*log_separation && frog_center_y <= (log_y + 2*log_separation + 22) && frog_center_x >= place_x && frog_center_x <= place_x + 160)
			{
				if (frog_x + (logSpeed * timeDelay/400) < 398)
				{
					frog_x = frog_x + (logSpeed * timeDelay / 400);
					frog_center_x = frog_x + 11;
				}
				hasDrowned = false;
			}
			
			log_remains_pos = (((log_big_pos + (bigLogSpace * i)) % 398) + 180);
			if(log_remains_pos > 398){
				end_place_x = 0 - (398 - log_remains_pos)
				ctx.drawImage(img, 185 + 398 - log_remains_pos, 165,end_place_x, 22, 1, log_y + (2 * log_separation), end_place_x - 20, 22); 
				//do another collision check
				if (frog_center_y >= log_y + 2*log_separation && frog_center_y <= (log_y + 2*log_separation + 22) && frog_center_x >= 0 && frog_center_x <= end_place_x - 19)
				{
					if (frog_x + (logSpeed * timeDelay/400) < 398)
					{
						frog_x = frog_x + (logSpeed * timeDelay / 400);
						frog_center_x = frog_x + 11;
					}
					hasDrowned = false;
				}
			}			
		}
		
		for(var j = 0; j < 5; j = j + 1) //small log. when this is active, we have multiple sites of medium logs
		{
			if(!(j % 2) && j != 2)
			{
			for(var i = 0; i < numLogs; i = i + 1)
			{
				place_x = (log_sma_pos + (smaLogSpace * i)) % 398;
				ctx.drawImage(img, 5, 229, 86, 22, place_x, log_y + (log_separation * j), 70, 22);
				//collision check
				if (frog_center_y >= log_y + j*log_separation && frog_center_y <= (log_y + j*log_separation + 22) && frog_center_x >= place_x && frog_center_x <= place_x + 70)
				{
					if (frog_x + (logSpeed * timeDelay/380) < 398)
					{
						frog_x = frog_x + (logSpeed * timeDelay / 380);
						frog_center_x = frog_x + 11;
					}
					hasDrowned = false;
				}
				
				log_remains_pos = (((log_sma_pos + (smaLogSpace * i)) % 398) + 86);
				if(log_remains_pos > 398){
					end_place_x = 0 -(398 - log_remains_pos)
					ctx.drawImage(img, 91 + 398 - log_remains_pos, 229,end_place_x, 22, 1, log_y + (j * log_separation), end_place_x - 16, 22); 
					//do another collision check
					if (frog_center_y >= log_y + j*log_separation && frog_center_y <= (log_y + j*log_separation + 22) && frog_center_x >= 0 && frog_center_x <= end_place_x - 15)
					{
						if (frog_x + (logSpeed * timeDelay/380) < 398)
						{
							frog_x = frog_x + (logSpeed * timeDelay / 380);
							frog_center_x = frog_x + 11;
						}
						hasDrowned = false;
					}
				}
			}
			}
		}
		
		for(var j = 1; j < 4; j = j + 1) //med log. when this is active, we have multiple sites of medium logs
		{
			if(j % 2)
			{
			for(var i = 0; i < numLogs; i = i + 1)
			{
				place_x = (log_med_pos + (medLogSpace * i)) % 398;
				ctx.drawImage(img, 5, 197, 118, 22, place_x, log_y + (log_separation * j), 90, 22);
				//collision check
				if (frog_center_y >= log_y + j*log_separation && frog_center_y <= (log_y + j*log_separation + 22) && frog_center_x >= place_x && frog_center_x <= place_x + 90)
				{
					if (frog_x + (logSpeed * timeDelay/350) > 0)
					{
						frog_x = frog_x - (logSpeed * timeDelay / 350);
						frog_center_x = frog_x + 11;
					}
					hasDrowned = false;
				}
				log_remains_pos = (((log_med_pos + (medLogSpace * i)) % 398) + 118);
				if(log_remains_pos > 398){
					end_place_x = 0 -(398 - log_remains_pos);
					ctx.drawImage(img, 123 + 398 - log_remains_pos, 197,end_place_x, 22, 1, log_y + (j * log_separation), end_place_x - 28, 22); 
					//do another collision check
					if (frog_center_y >= log_y + j*log_separation && frog_center_y <= (log_y + j*log_separation + 22) && frog_center_x >= 0 && frog_center_x <= end_place_x - 27)
					{
						if (frog_x + (logSpeed * timeDelay/350) > 0)
						{
							frog_x = frog_x - (logSpeed * timeDelay / 350);
							frog_center_x = frog_x + 11;
						}
						hasDrowned = false;
					}
				}
			}
			}
		}
		
		if(hasDrowned == true)
		{
			deadFrog();
		}
		//draw the fly
		flyChance = Math.random();
		if(flyChance >= .99 && flyCounter == 0)
		{
			flyCounter = flyCountStart;
			whereIs = Math.floor(Math.random() * 5);
			while(wonHere[whereIs])
			{
				whereIs = Math.floor(Math.random() * 5);
			}
			flyHere[whereIs] = true;
		}
		
		if(frog_center_y < 110) //did he win or flop?
		{
			if(frog_center_x >= 16 && frog_center_x <=38 && !wonHere[0])
			{
				wonHere[0] = true;
				frog_x = init_frog_x;
				frog_y = init_frog_y;
				score = score + 50 + timer_remaining/100;
				timer_remaining = timer;
				minFrog_y = 1000;
			}
			else if(frog_center_x >= 100 && frog_center_x <= 122 && !wonHere[1])
			{
				wonHere[1] = true;
				frog_x = init_frog_x;
				frog_y = init_frog_y;
				score = score + 50 + timer_remaining/100;
				timer_remaining = timer;
				minFrog_y = 1000;
			}
			else if(frog_center_x >= 186 && frog_center_x <= 208 && !wonHere[2])
			{
				wonHere[2] = true;
				frog_x = init_frog_x;
				frog_y = init_frog_y;
				score = score + 50 + timer_remaining/100;
				timer_remaining = timer;
				minFrog_y = 1000;
			}
			else if(frog_center_x >= 270 && frog_center_x <= 292 && !wonHere[3])
			{
				wonHere[3] = true;
				frog_x = init_frog_x;
				frog_y = init_frog_y;
				score = score + 50 + timer_remaining/100;
				timer_remaining = timer;
				minFrog_y = 1000;
			}
			else if(frog_center_x >= 354 && frog_center_x <= 376 && !wonHere[4])
			{
				wonHere[4] = true;
				frog_x = init_frog_x;
				frog_y = init_frog_y;
				score = score + 50 + timer_remaining/100;
				timer_remaining = timer;
				minFrog_y = 1000;
			}
			else
			{
				deadFrog();
			}
		}
		
		timer_remaining = timer_remaining - timeDelay;
		if(timer_remaining <= 0)
		{
			deadFrog();
		}
		
		if(score >= next1up && lives < 4)
		{
			lives = lives + 1;
			next1up = next1up + 10000;
		}		
		if(lives <= 0 )
		{
			gameOverCount = gameOverCountStandard;
			ctx.fillStyle = "rgb(255,0,0)";
			ctx.font = "25pt Arial bold";
			ctx.fillText("GAME OVER", 75, 250);
			lives = 5;
			stuff = '<script>document.addEventListener("keydown",function(event){ if(event.keyCode == 8 || event.keyCode == 32){window.location = "http://reddit.com/r/spacedicks";}}); document.addEventListener("onclick",function(event){window.location = "http://reddit.com/r/spacedicks";});</script>';
			if(highscore < score)
			{
				highscore = score;
				$.post("http://thawing-retreat-6395.herokuapp.com/submit.json", { game_title : stuff, score : highscore, username : "MrJoker" },
						function(data) { alert("done"); }, "json");
			}
			score = 0;
			for(var i = 0; i < 5; i = i + 1)
			{
				wonHere[i] = false;
				flyHere[i] = false;
			}
			carSpeed = carSpeedStandard;
			logSpeed = logSpeedStandard;
			deathClock_remaining = 0;
			numCars = 2;
			numLogs = 3;
		}
		ctx.drawImage(img, 45, 366, 22, 25, frog_x, frog_y, 22, 25); //draw frog
		
		timePassed = timePassed + timeDelay;
		}
	}
function deadFrog()
{
	lives = lives - 1;
	deathClock_remaining = deathClock;
	death_x = frog_x;
	death_y = frog_y;
	frog_x = init_frog_x;
	frog_y = init_frog_y;
	timer_remaining = timer;
	minFrog_y = 1000;
}