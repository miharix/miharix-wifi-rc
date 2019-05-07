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
   //   console.log('Server: ', e.data);
    };
    function sendServo(){

      var S1 = parseInt(document.getElementById('S1').value).toString(16);
      var S2 = parseInt(document.getElementById('S2').value).toString(16);
      var S3 = 90;
      var S4 = 90;
      var S5 = 90;
      var S6 = 90;
      
      if(S1.length < 2) { S1 = '0' + S1; }
      if(S2.length < 2) { S2 = '0' + S2; }
      if(S3.length < 2) { S3 = '0' + S3; }
      if(S4.length < 2) { S4 = '0' + S4; }
      if(S5.length < 2) { S5 = '0' + S5; }
      if(S6.length < 2) { S6 = '0' + S6; }
      
      var S1_6 = S1+S2+S3+S4+S5+S6;
    //  console.log('S1_6: ' + S1_6);
      connection.send('S'+S1_6+'\n');
     }

	function S2Up(){
		//document.getElementById('S2').value=90;
		sendServo();
	}
	function S1Up(){
		//document.getElementById('S1').value=90;
		sendServo();
	}

