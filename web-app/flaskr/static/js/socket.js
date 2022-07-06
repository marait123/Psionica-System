function bold_string(str){
  return `${str}`
}

$(function () {
  console.log("hi people");
  var socket = io.connect(window.location.origin);
  socket.on("connect", async () => {
    let x = await socket.send("user has connected");
  });
  socket.on("message", async (data) => {
    console.log("data received is ", data);
  });
  socket.on("new-action", async (data) => {
    console.log("a new action arrived ", data);
    console.log("the action is ", data["predicted_action"]);
    // <div id="sim-arrow" onclick="" class="sim-arrow move-R">
    $("#sim-arrow").removeClass("move-L move-R move-F move-B");
    $("#sim-arrow").addClass("move-" + data["predicted_action"]);
    let actionMap = {
      R: "right",
      L: "left",
      F: "forward",
      B: "backward",
    };
    // TODO: put the ip in a single place
    console.log("we are here")
    send_command( data["predicted_action"]);
    $(".predVsTrue").text("predicted is ("+bold_string(actionMap[data["predicted_action"]])+") true is ("+ bold_string(actionMap[data["true_action"]])+ ")") ;

    if(data["predicted_action"] == data["true_action"]){

      $(".predVsTrue").css("color","black")
    }
    else{

      $(".predVsTrue").css("color","red")
    }

  });
  socket.on("end-simulation", async (data) => {
    console.log("a new action arrived ", data);
    console.log("the action is ", data["action"]);  //correct_predictions miss_predictions
    SimulationButton.update();
    $("#sim-arrow").removeClass("move-L move-R move-F move-B");
    $(".predVsTrue").text("simulation ended")
    // $(".predVsTrue").text("number of correct prediction: "+ bold_string(data["correct_predictions"])  + " number of missprediction: ", bold_string(data["miss_predictions"]))
  });
});
