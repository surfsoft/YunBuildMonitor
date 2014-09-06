use <neoMatrixMount-v1.scad>
use <legendMount-v1.scad>

module buildMonitorDisplay(buildCount) {

	neoMatrixMount(buildCount, 1);
	translate([0, 36.2 + 3 + 12, 0]) legendMount(buildCount);
	
	translate([-30, -43.7, 0])
		difference() {
			cube([18, 9, 3], center=true);
			translate([0, 2, 0]) cube([10, 5, 3], center=true);
		};

	translate([30, -43.7, 0])
		difference() {
			cube([18, 9, 3], center=true);
			translate([0, 2, 0]) cube([10, 5, 3], center=true);
		};

}

buildMonitorDisplay(1);