

PHONE_IP = "http://192.168.1.160:12345"
CAR_IP = "http://192.168.1.159:12345"

// PHONE_IP = "http://192.168.4.2:12345"
// CAR_IP = "http://192.168.4.1:12345"
// PHONE_IP = CAR_IP
console.log(PHONE_IP)

function discoverip(){
    for(let i = 1; i < 254;i++){
        $.ajax({
            type: "GET",
            url: ``, //TODO: update request URL
            success: (result) => {
              console.log("result is ", result);
            },
            error: (error) => {
              console.error(error);
            },
          });   
    }
    
}