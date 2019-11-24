$fn=80;

module trikotnik(stranica,visina){
    polmer=(stranica*sqrt(3))/3;
    translate([visina/2,0,polmer/2])
    rotate([0,-90,0])
    cylinder(h=visina,r=polmer,$fn=3);
}



difference(){

hull(){

/*translate([0,0,0])
rotate([0,90,0])
cylinder(h=43,r=16,center=true);
*/
translate([43,0,0])
rotate([0,90,0])
cylinder(h=43, r1=16, r2=0,center=true);

translate([-43/2,0,0])
sphere(r=16);
}

translate([-10,0,11])
trikotnik(5.5,60);

translate([-10,0,16])
cube([60,3,6],center=true);

translate([10,0,-16])
cube([130,16*2,16*2],center=true);
}