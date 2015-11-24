
	function draw()
	{
		canvas = document.getElementById('simple');
		if (canvas.getContext)
		{
			ctx = canvas.getContext('2d');
			img = new Image();
			img.src = 'pacman10-hp-sprite.png';
			ctx.drawImage(img, 320, 0, 465, 138, 0, 0, 465, 138);
			ctx.drawImage(img, 80, 0, 20, 20, 100, 35, 20, 20);
		}
		else
		{
			alert('Sorry, canvas is not supported here! Please just leave, you are making a mockery of modern internet browsers!');
		}
	}
