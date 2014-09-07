pcbWidth = 54;
pcbHeight = 76.8;
border = 5;
thickness = 3;
inset = 1.4;
pcbDepth = 1.6;
powerCordGapDepth = 1.7;
retainerWidth = 3;
retainerDepth = 2;

spacerWidth = 7;
spacerDepth = 7;
spacerHeight = 25;


difference() {
	difference() {
    		cube([pcbWidth + (border * 2), pcbHeight + (border * 2), thickness], center = true);
	    cube([pcbWidth, pcbHeight, thickness], center = true);
	};
	translate([ - pcbWidth / 2 + 20, (border + pcbHeight) / 2, (thickness / 2) - (powerCordGapDepth / 2)]) cube([12, border, powerCordGapDepth], center=true);
};

translate([(pcbWidth / 2) - (inset / 2), 0, - (thickness / 2) + (inset / 2)]) cube([inset, pcbHeight, inset], center=true);
translate([- (pcbWidth / 2) + (inset / 2), 0, - (thickness / 2) + (inset / 2)]) cube([inset, pcbHeight, inset], center=true);

translate([0.5 + pcbWidth / 2, 0, (thickness / 2) + (retainerDepth / 2)]) cube([retainerWidth, 10, retainerDepth], center=true);
translate([-0.5 - pcbWidth / 2, pcbHeight / 4, (thickness / 2) + (retainerDepth / 2)]) cube([retainerWidth, 10, retainerDepth], center=true);
translate([-0.5 - pcbWidth / 2, -pcbHeight / 4, (thickness / 2) + (retainerDepth / 2)]) cube([retainerWidth, 10, retainerDepth], center=true);

translate([(pcbWidth / 2) + border + (spacerWidth / 2), (pcbHeight / 2) - (spacerDepth / 2) + border - 3.3, (spacerHeight / 2) - thickness / 2]) 
	cube([spacerWidth, spacerDepth, spacerHeight], center=true);

translate([- (pcbWidth / 2) - border - (spacerWidth / 2), (pcbHeight / 2) - (spacerDepth / 2) + border - 3.3, (spacerHeight / 2) - thickness / 2]) 
	cube([spacerWidth, spacerDepth, spacerHeight], center=true);

translate([(pcbWidth / 2) + border + (spacerWidth / 2), - (pcbHeight / 2) + (spacerDepth / 2) - border, (spacerHeight / 2) - thickness / 2]) 
	cube([spacerWidth, spacerDepth, spacerHeight], center=true);

translate([- (pcbWidth / 2) - border - (spacerWidth / 2), - (pcbHeight / 2) + (spacerDepth / 2) - border, (spacerHeight / 2) - thickness / 2]) 
	cube([spacerWidth, spacerDepth, spacerHeight], center=true);

	translate([-30, - (pcbHeight / 2) - border - 4.5, 0])
		difference() {
			cube([18, 9, thickness], center=true);
			translate([0, 2, 0]) cube([10, 5, thickness], center=true);
		};

	translate([30, - (pcbHeight / 2) - border - 4.5, 0])
		difference() {
			cube([18, 9, thickness], center=true);
			translate([0, 2, 0]) cube([10, 5, thickness], center=true);
		};