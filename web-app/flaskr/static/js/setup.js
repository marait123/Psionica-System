// this is the file where the setup of everything happens
// and the initial configurations are made
var PHONE_IP = "http://192.168.1.160:12345";
var CAR_IP = "http://192.168.1.159:12345";
var SIMULATION_INTERVAL = 3000;
// PHONE_IP = "http://192.168.4.2:12345"
// CAR_IP = "http://192.168.4.1:12345"

INTENDED_IP = CAR_IP;
console.log(PHONE_IP);
$(function () {
  console.log("hello we are comming soon");
  // setInterval(function () {
  //   // alert("hi");
  //   $(".timeNow").text(new Date().toLocaleTimeString());
  // }, 10 / 00);

  // $(window).on("hashchange", TabChanged);
  // $(".control").on("keydown", function (e) {
  //   console.log(e);
  //   console.log("you pressed", e.key);
  // });

  // $.ajax({
  //   type: "GET",
  //   url: "/simulation", //TODO: update request URL
  //   success: (result) => {
  //     // simulation_status = result.simulation_status;
  //     const { simulation_status, last_action } = result;
  //     console.log("get_simulation_status result is ", {
  //       simulation_status,
  //       last_action,
  //     });

  //   },
  //   error: (error) => {
  //     console.error(error);
  //     alert(error);
  //   },
  // });
});

// function discoverip(){
//     for(let i = 1; i < 254;i++){
//         $.ajax({
//             type: "GET",
//             url: ``, //TODO: update request URL
//             success: (result) => {
//               console.log("result is ", result);
//             },
//             error: (error) => {
//               console.error(error);
//             },
//           });
//     }

// }
