// this is the file where the setup of everything happens
// and the initial configurations are made
var PHONE_IP = "http://192.168.1.160:12345";
var CAR_IP = "http://192.168.1.159:12345";
var SIMULATION_INTERVAL = 3000;
// PHONE_IP = "http://192.168.4.1:12345"
// CAR_IP = "http://192.168.4.1:12345"

INTENDED_IP = PHONE_IP;
function switch_mode(){
  INTENDED_IP = INTENDED_IP == PHONE_IP ? CAR_IP : PHONE_IP;
 
}



console.log("we are sending to ",INTENDED_IP);
$(function () {
  console.log("hello we are comming soon");
  
});
