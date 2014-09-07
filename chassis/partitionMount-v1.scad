width = 9.4;
depth = 4.4;

rotate([90, 0, 0]) union() {
cube([depth, width, 15], center=true);
translate([27., 0, 5.3]) cube([50, width, depth], center=true);
translate([12, 0, -10]) cube([depth, width, 35], center=true);
};