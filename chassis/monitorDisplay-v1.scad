use <neoMatrixMount-v1.scad>
use <legendMount-v1.scad>

module buildMonitorDisplay(buildCount) {

	neoMatrixMount(buildCount, 1);
	translate([0, 36.2 + 3 + 12, 0]) legendMount(buildCount);

}

buildMonitorDisplay(2);