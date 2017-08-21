pcb_height=31;
pcb_width=43.18;
pcb_thick=2;
grove=1;
walls=2.4;
bottom_offset=2;
chamfer_top=0.7;//[0.0:1.0]
chamfer_bottom=0.3;//[0.0:1.0]


difference(){
//box
translate([walls/2+0.1,0,pcb_thick/2+bottom_offset/2]){
cube([pcb_height+walls,pcb_width+walls*2,pcb_thick+walls*2+bottom_offset],center=true);
}

//pcb
color("green")
translate([0,0,pcb_thick/2+bottom_offset]){
cube([pcb_height,pcb_width,pcb_thick],center=true);
}

//grove
color("blue")
translate([-grove/2,0,pcb_thick+walls/2+bottom_offset]){
cube([pcb_height-grove,pcb_width-grove*2,walls+0.01],center=true);
}


//chamfer top
color("red")
translate([-grove/2,0,pcb_thick+walls/2+bottom_offset-0.01]){
linear_extrude(height = walls+0.1, center = true, convexity = 3, scale = chamfer_top){
square(size = [pcb_height, pcb_width], center = true);
}
}

//bottom offset
color("Gray")
translate([-grove/2,0,bottom_offset/2]){
cube([pcb_height-grove,pcb_width-grove*2,bottom_offset+0.01],center=true);
}

//chamfer bottom
color("Purple")
translate([-grove/2,0,bottom_offset/2+0.01]){
mirror([0,0,1]) 
linear_extrude(height = bottom_offset, center = true, convexity = 3, scale = chamfer_bottom){
square(size = [pcb_height, pcb_width], center = true);
}
}

}

