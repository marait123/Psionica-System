const SimulationButton = {
  update: function () {
    $.ajax({
      type: "GET",
      url: "/simulation",
      success: (result) => {
        const { simulation_status } = result;
        console.log("result is ", result);
        let simBtn = document.getElementById("sim-btn");

        if (simulation_status == false) {
          simBtn.style.backgroundColor = "#17a2b8";
          simBtn.innerHTML = "start simulation";
        } else {
          simBtn.style.backgroundColor = "red";
          simBtn.innerHTML = "stop simulation";
        }
      },
      error: (error) => {
        console.error(error);
        alert(error);
      },
    });
  },
};

function send_command( action) {
  // alert("hi");
  $.ajax({
    type: "GET",
    url: `prediction?action=${action}`, //TODO: update request URL
    success: (result) => {
      console.log("result is ", result);
      predicted_action = result.prediction;
      let endpoint= `${PHONE_IP}/${predicted_action}`
      console.log("we are sending to ", endpoint, predicted_action)
      $.ajax({
        type: "GET",
        url: endpoint, //TODO: update request URL
        success: (result) => {
          console.log("result is ", result);
        },
        error: (error) => {
          console.error(error);
          // alert(error);
        },
      });

    },
    error: (error) => {
      console.error(error);
      // alert(error);
    },
  });
 
}
let simulation_status = false;

function toggle_simulation() {
  $.ajax({
    type: "PUT",
    url: "/simulation", //TODO: update request URL
    success: (result) => {
      const { simulation_status } = result;
      console.log("result is ", result);
      SimulationButton.update();
    },
    error: (error) => {
      console.error(error);
      alert(error);
    },
  });
}
// on start code
let currentTab = ".manualTab";
function TabChanged(e = null) {
  // Your Code goes here
  let tabClassName = window.location.hash
    ? "." + window.location.hash.substring(1)
    : currentTab;
  $(currentTab).hide();
  currentTab = tabClassName;
  $(currentTab).show();
}
$(function () {
  TabChanged();
  setInterval(function () {
    // alert("hi");
    $(".timeNow").text(new Date().toLocaleTimeString());
  }, 10 / 00);

  $(window).on("hashchange", TabChanged);
  $(".control").on("keydown", function (e) {
    console.log(e);
    console.log("you pressed", e.key);
  });

  $.ajax({
    type: "GET",
    url: "/simulation", //TODO: update request URL
    success: (result) => {
      // simulation_status = result.simulation_status;
      const { simulation_status, last_action } = result;
      console.log("get_simulation_status result is ", {
        simulation_status,
        last_action,
      });

      SimulationButton.update();
      if (last_action == "stop") {
        $("#sim-arrow").removeClass("move-L move-R move-F move-B");
        // $("#sim-arrow").addClass("no-move");
      } else {
        $("#sim-arrow").removeClass("move-L move-R move-F move-B");
        $("#sim-arrow").addClass("move-" + last_action);
      }
    },
    error: (error) => {
      console.error(error);
      alert(error);
    },
  });
});
