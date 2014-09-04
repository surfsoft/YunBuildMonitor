module legendMount(buildCount) {

    unitLength = 72.4;
	unitWidth = 72.4;
	length = unitLength * buildCount;
	width = 24;
	depth = 3;
	mountWidth = 5;

	translate([0, (width / 2) - (mountWidth / 2), 0])
      cube([length, mountWidth, depth], center=true);
	translate([0, (width / 2) + (depth / 2), (mountWidth - depth) / 2])
	  cube([length + (depth * 2), depth, mountWidth], center=true);

	translate([0, - (width / 2) + (mountWidth / 2), 0])
      cube([length, mountWidth, depth], center=true);

	translate([(length / 2) - (mountWidth / 2), 0, 0]) 
	  cube([mountWidth, width, depth], center=true);
	translate([(length / 2) + (depth / 2), 0, (mountWidth - depth) / 2]) 
	  cube([depth, width, mountWidth], center=true);

	translate([- (length / 2) + (mountWidth / 2), 0, 0]) 
	  cube([mountWidth, width, depth], center=true);
	translate([- (length / 2) -  + (depth / 2), 0, (mountWidth - depth) / 2]) 
	  cube([depth, width, mountWidth], center=true);

	for (panelNo = [ 0 : buildCount - 1 ]) {
		translate([((panelNo + 0.5) * unitLength) - (length / 2), - width / 2, mountWidth - 0.5])
		  cube([10, 6, 2], center=true);
		translate([((panelNo + 0.5) * unitLength) - (length / 2), width / 2, mountWidth - 0.5])
		  cube([10, 6, 2], center=true);
	}

	if (buildCount > 1) {
		for (panelNo = [ 1 : buildCount - 1 ]) {
			translate([((panelNo - 1) * unitLength) , 0, 0])
			  cube([mountWidth, width, depth], center=true);
		}
	}

}