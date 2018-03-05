function _GET(identifier) {
    var result = undefined, tmp = [];

    var items = window.location.search.substr(1).split("&");

    for (var index = 0; index < items.length; index++) {
        tmp = items[index].split("=");

        if (tmp[0] === identifier){
            result = decodeURIComponent(tmp[1]);
        }
    }

    return result;
}



	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
        		var myObj = JSON.parse(this.responseText);
			if (_GET("s1t")==null){document.getElementById("s1t").value = myObj.Servo1.trim;}else{document.getElementById("s1t").value = _GET("s1t");}
			if (_GET("s2t")==null){document.getElementById("s2t").value = myObj.Servo2.trim;}else{document.getElementById("s2t").value = _GET("s2t");}
        		if (_GET("s3t")==null){document.getElementById("s3t").value = myObj.Servo3.trim;}else{document.getElementById("s3t").value = _GET("s3t");}
			if (_GET("s4t")==null){document.getElementById("s4t").value = myObj.Servo4.trim;}else{document.getElementById("s4t").value = _GET("s4t");}
			if (_GET("s5t")==null){document.getElementById("s5t").value = myObj.Servo5.trim;}else{document.getElementById("s5t").value = _GET("s5t");}
			if (_GET("s6t")==null){document.getElementById("s6t").value = myObj.Servo6.trim;}else{document.getElementById("s6t").value = _GET("s6t");}

			if (_GET("s1x")==null){document.getElementById("s1x").value = myObj.Servo1.max;}else{document.getElementById("s1x").value = _GET("s1x");}
			if (_GET("s2x")==null){document.getElementById("s2x").value = myObj.Servo2.max;}else{document.getElementById("s2x").value = _GET("s2x");}
			if (_GET("s3x")==null){document.getElementById("s3x").value = myObj.Servo3.max;}else{document.getElementById("s3x").value = _GET("s3x");}
			if (_GET("s4x")==null){document.getElementById("s4x").value = myObj.Servo4.max;}else{document.getElementById("s4x").value = _GET("s4x");}
			if (_GET("s5x")==null){document.getElementById("s5x").value = myObj.Servo5.max;}else{document.getElementById("s5x").value = _GET("s5x");}
			if (_GET("s6x")==null){document.getElementById("s6x").value = myObj.Servo6.max;}else{document.getElementById("s6x").value = _GET("s6x");}

			if (_GET("s1n")==null){document.getElementById("s1n").value = myObj.Servo1.min;}else{document.getElementById("s1n").value = _GET("s1n");}
			if (_GET("s2n")==null){document.getElementById("s2n").value = myObj.Servo2.min;}else{document.getElementById("s2n").value = _GET("s2n");}
			if (_GET("s3n")==null){document.getElementById("s3n").value = myObj.Servo3.min;}else{document.getElementById("s3n").value = _GET("s3n");}
			if (_GET("s4n")==null){document.getElementById("s4n").value = myObj.Servo4.min;}else{document.getElementById("s4n").value = _GET("s4n");}
			if (_GET("s5n")==null){document.getElementById("s5n").value = myObj.Servo5.min;}else{document.getElementById("s5n").value = _GET("s5n");}
			if (_GET("s6n")==null){document.getElementById("s6n").value = myObj.Servo6.min;}else{document.getElementById("s6n").value = _GET("s6n");}

			if (_GET("s1f")==null){document.getElementById("s1f").value = myObj.Servo1.failsave;}else{document.getElementById("s1f").value = _GET("s1f");}
			if (_GET("s2f")==null){document.getElementById("s2f").value = myObj.Servo2.failsave;}else{document.getElementById("s2f").value = _GET("s2f");}
			if (_GET("s3f")==null){document.getElementById("s3f").value = myObj.Servo3.failsave;}else{document.getElementById("s3f").value = _GET("s3f");}
			if (_GET("s4f")==null){document.getElementById("s4f").value = myObj.Servo4.failsave;}else{document.getElementById("s4f").value = _GET("s4f");}
			if (_GET("s5f")==null){document.getElementById("s5f").value = myObj.Servo5.failsave;}else{document.getElementById("s5f").value = _GET("s5f");}
			if (_GET("s6f")==null){document.getElementById("s6f").value = myObj.Servo6.failsave;}else{document.getElementById("s6f").value = _GET("s6f");}

			if (_GET("s1e")==null){document.getElementById("s1e").value = myObj.Servo1.expo;}else{document.getElementById("s1e").value = _GET("s1e");}
			if (_GET("s2e")==null){document.getElementById("s2e").value = myObj.Servo2.expo;}else{document.getElementById("s2e").value = _GET("s2e");}
			if (_GET("s3e")==null){document.getElementById("s3e").value = myObj.Servo3.expo;}else{document.getElementById("s3e").value = _GET("s3e");}
			if (_GET("s4e")==null){document.getElementById("s4e").value = myObj.Servo4.expo;}else{document.getElementById("s4e").value = _GET("s4e");}
			if (_GET("s5e")==null){document.getElementById("s5e").value = myObj.Servo5.expo;}else{document.getElementById("s5e").value = _GET("s5e");}
			if (_GET("s6e")==null){document.getElementById("s6e").value = myObj.Servo6.expo;}else{document.getElementById("s6e").value = _GET("s6e");}

			
		if (_GET("s1r")==null){document.getElementById("s1r").checked = myObj.Servo1.reverse;}else{document.getElementById("s1r").checked = JSON.parse(_GET("s1r"));}
		if (_GET("s2r")==null){document.getElementById("s2r").checked = myObj.Servo2.reverse;}else{document.getElementById("s2r").checked = JSON.parse(_GET("s2r"));}
		if (_GET("s3r")==null){document.getElementById("s3r").checked = myObj.Servo3.reverse;}else{document.getElementById("s3r").checked = JSON.parse(_GET("s3r"));}
		if (_GET("s4r")==null){document.getElementById("s4r").checked = myObj.Servo4.reverse;}else{document.getElementById("s4r").checked = JSON.parse(_GET("s4r"));}
		if (_GET("s5r")==null){document.getElementById("s5r").checked = myObj.Servo5.reverse;}else{document.getElementById("s5r").checked = JSON.parse(_GET("s5r"));}
		if (_GET("s6r")==null){document.getElementById("s6r").checked = myObj.Servo6.reverse;}else{document.getElementById("s6r").checked = JSON.parse(_GET("s6r"));}

			
			if (_GET("Ftime")==null){document.getElementById("Ftime").value = myObj.Ftime;}else{document.getElementById("Ftime").value = _GET("Ftime");}

			if (_GET("s1s")==null){document.getElementById("s1s").value = myObj.Servo1.rate;}else{document.getElementById("s1s").value = _GET("s1s");}
			if (_GET("s2s")==null){document.getElementById("s2s").value = myObj.Servo2.rate;}else{document.getElementById("s2s").value = _GET("s2s");}
			if (_GET("s3s")==null){document.getElementById("s3s").value = myObj.Servo3.rate;}else{document.getElementById("s3s").value = _GET("s3s");}
			if (_GET("s4s")==null){document.getElementById("s4s").value = myObj.Servo4.rate;}else{document.getElementById("s4s").value = _GET("s4s");}
			if (_GET("s5s")==null){document.getElementById("s5s").value = myObj.Servo5.rate;}else{document.getElementById("s5s").value = _GET("s5s");}
			if (_GET("s6s")==null){document.getElementById("s6s").value = myObj.Servo6.rate;}else{document.getElementById("s6s").value = _GET("s6s");}
	
			if (_GET("Mode")==null){document.getElementById("Mode").value = myObj.Mode;}else{document.getElementById("Mode").value = _GET("Mode");}

			if (_GET("rcname")==null){
				document.getElementById("rcname").value = myObj.RCname;
				document.getElementById("rc").innerHTML = myObj.RCname
			}else{
				document.getElementById("rcname").value = _GET("rcname");
				document.getElementById("rc").innerHTML = _GET("rcname");
			}


			if (_GET("RCSSID")==null){document.getElementById("RCSSID").value = myObj.SSID;}else{document.getElementById("RCSSID").value = _GET("RCSSID");}


			if (_GET("RCPASS")==null){document.getElementById("RCPASS").value = myObj.Pass;}else{document.getElementById("RCPASS").value = _GET("RCPASS");}
			
		if (_GET("WiFiAP")==null){document.getElementById("WiFiAP").checked = myObj.WiFiAP;}else{document.getElementById("WiFiAP").checked = JSON.parse(_GET("WiFiAP"));}
			

    }
};
xmlhttp.open("GET", "config.json", true);
xmlhttp.send();
