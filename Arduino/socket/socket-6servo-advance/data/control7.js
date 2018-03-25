   var Servo1=0;
    var Servo2=0;
    var Servo3=0;
    var Servo4=0;
    var Servo5=0;
    var Servo6=0;
    
    var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
    connection.onopen = function () {
   //   connection.send('Connect ' + new Date()); 
    };
    connection.onerror = function (error) {
      console.log('WebSocket Error ', error);
    };
    connection.onmessage = function (e) {
      console.log('Server: ', e.data);
    };

    function sendServo2(){
	var S3 = "0";
	var S4 = "0";    
	var S5 = "0";
	var S6 = "0";

	Servo1=Servo1/90-1
	Servo2=Servo2/90-1
//https://electronics.stackexchange.com/questions/19669/algorithm-for-mixing-2-axis-analog-input-to-control-a-differential-motor-drive
	//convert to polar   
    	r = Math.hypot(Servo2, Servo1)
	t = Math.atan2(Servo1, Servo2)
	
	//rotate by 45 degrees
	t += Math.PI / 4
	//back to cartesian
	Servo1 = r * Math.cos(t)
	Servo2 = r * Math.sin(t)
	//rescale the new coords
	Servo1 = Servo1 * Math.sqrt(2)
	Servo2 = Servo2 * Math.sqrt(2)
	//clamp to -1/+1
	Servo1 = Math.max(-1, Math.min(Servo1, 1))
	Servo2 = Math.max(-1, Math.min(Servo2, 1))

	Servo1=(Servo1+1)*90
	Servo2=(Servo2+1)*90

	S2 =parseInt(Servo1).toString(16);
      	S1 =parseInt(Servo2).toString(16);

	if(S1.length < 2) { S1 = '0' + S1; }
        if(S2.length < 2) { S2 = '0' + S2; }
	if(S3.length < 2) { S3 = '0' + S3; }
	if(S4.length < 2) { S4 = '0' + S4; }
	if(S5.length < 2) { S5 = '0' + S5; }
	if(S6.length < 2) { S6 = '0' + S6; }
	//console.clear();
	//console.log('S1: ' + S1+' S2: ' + S2);
	//console.log('Servo1: ' + Servo1+' Servo2: ' + Servo2);
        
	var S1_6 = S1+S2+S3+S4+S5+S6;
        //console.log('S1_6: ' + S1_6);
        connection.send('S'+S1_6+'\n');

    }

    function sendServo(){
      S1 =parseInt(Servo1).toString(16);
      S2 =parseInt(Servo2).toString(16);
      var S3 = "0";
      var S4 = "0";    
      var S5 = "0";
      var S6 = "0";
      
      if(S1.length < 2) { S1 = '0' + S1; }
      if(S2.length < 2) { S2 = '0' + S2; }
      if(S3.length < 2) { S3 = '0' + S3; }
      if(S4.length < 2) { S4 = '0' + S4; }
      if(S5.length < 2) { S5 = '0' + S5; }
      if(S6.length < 2) { S6 = '0' + S6; }
	//console.clear();
	//console.log('O1: ' + O1+' O2: ' + O2);
      //console.log('Servo1: ' + Servo1+' Servo2: ' + Servo2);
      var S1_6 = S1+S2+S3+S4+S5+S6;

     // console.log('S1_6: ' + S1_6);
      connection.send('S'+S1_6+'\n');
     }
