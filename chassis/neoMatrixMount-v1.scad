module neoMatrixMount(sizeX, sizeY) {

    unitLength = 72.4;
	unitWidth = 72.4;
	length = unitLength * sizeX;
	width = unitWidth * sizeY;
	depth = 3;
	mountWidth = 5;

	translate([ - length / 2 + mountWidth / 2, - 5.5, 0]) 
	  cube([mountWidth, width - 11, depth], center=true);
	translate([ - length / 2 - depth / 2, 0, (mountWidth - depth) / 2]) 
	  cube([depth, width + (depth * 2), mountWidth], center=true);

	translate([0, - width / 2 + (mountWidth / 2), 0]) 
	  cube([length, mountWidth, depth], center=true);
	translate([0, - width / 2 - depth / 2, (mountWidth - depth) / 2]) 
	  cube([length, depth, mountWidth], center=true);

	translate([0, width / 2 - (mountWidth / 2), 0]) 
	  cube([length, mountWidth, depth], center=true);
	translate([0, width / 2 + depth / 2, (mountWidth - depth) / 2]) 
	  cube([length, depth, mountWidth], center=true);

	translate([ length / 2 - mountWidth / 2, 0, 0]) 
	  cube([mountWidth, width, depth], center=true);
	translate([ length / 2 + depth / 2, 0, (mountWidth - depth) / 2]) 
	  cube([depth, width + (depth * 2), mountWidth], center=true);


	for (panelNo = [ 0 : sizeX - 1 ]) {
		translate([ (((panelNo + 1) * unitLength) - (unitLength / 2)) - (length / 2), - width / 2, mountWidth - 0.5])
		  cube([10, 4, 2], center=true);
		translate([ (((panelNo + 1) * unitLength) - (unitLength / 2)) - (length / 2), width / 2, mountWidth - 0.5])
		  cube([10, 4, 2], center=true);
	}



	if (sizeX > 1) {
		for (panelNo = [ 1 : sizeX - 1 ]) {
			translate([((panelNo - 1) * unitLength) , 0, 0])
			  cube([mountWidth, width, depth], center=true);
		}
	}

}