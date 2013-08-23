setBounds()

sensor(3.0,5.0, 0.5, FINISH);
sensor(4.6,4.0, 0.3, FORCE, -PI/6+PI);
sensor(6.6,3.0,0.25, PUNCH);
		
block(3.0, 3.0, 2.0, 0.2 , PI/4);
block(6.0, 4.0, 3.0, 0.05,-PI/4);
block(1.0, 2.0, 3.0, 0.1 ,-PI/4);

setBallPos(3.0, 1.1);

setPunchCount(1);
