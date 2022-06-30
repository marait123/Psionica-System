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
    console.log("the action is ", data["action"]);
    // <div id="sim-arrow" onclick="" class="sim-arrow move-right">
    $("#sim-arrow").removeClass("move-left move-right move-up move-down");
    $("#sim-arrow").addClass("move-" + data["action"]);
    let actionMap = {
      right: "R",
      left: "L",
      up: "F",
      down: "B",
    };

    send_command("http://156.223.70.165:12345/" + actionMap[data["action"]]);
  });
  socket.on("end-simulation", async (data) => {
    console.log("a new action arrived ", data);
    console.log("the action is ", data["action"]);
    SimulationButton.update();
    $("#sim-arrow").removeClass("move-left move-right move-up move-down");
  });
});
