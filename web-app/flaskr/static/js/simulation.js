// const SimulationButton = {
//   update: function () {
//     $.ajax({
//       type: "GET",
//       url: "/simulation",
//       success: (result) => {
//         const { simulation_status } = result;
//         console.log("result is ", result);
//         let simBtn = document.getElementById("sim-btn");

//         if (simulation_status == false) {
//           simBtn.style.backgroundColor = "#17a2b8";
//           simBtn.innerHTML = "start simulation";
//         } else {
//           simBtn.style.backgroundColor = "red";
//           simBtn.innerHTML = "stop simulation";
//         }
//       },
//       error: (error) => {
//         console.error(error);
//         alert(error);
//       },
//     });
//   },
// };
let action_uiclass_map = {
  L: "#left-hand-imagine",
  R: "#right-hand-imagine",
  F: "#tongue-imagine",
  B: "#leg-imagine",
  I: "#idle-imagine",
};

let pred_action_uiclass_map = {
  L: ".pred-left-hand-imagine",
  R: ".pred-right-hand-imagine",
  F: ".pred-tongue-imagine",
  B: ".pred-leg-imagine",
  I: ".pred-idle-imagine",
};

// animations area
let timeout_id = null;

function hide_animations() {
  for (const key in action_uiclass_map) {
    if (Object.hasOwnProperty.call(action_uiclass_map, key)) {
      const element = action_uiclass_map[key];
      $(element).hide();
    }
  }

  for (const key in pred_action_uiclass_map) {
    if (Object.hasOwnProperty.call(pred_action_uiclass_map, key)) {
      const element = pred_action_uiclass_map[key];
      $(element).hide();
    }
  }

  timeout_id = null;
}
/*
calling update ui with no paramters hides everything
*/

function update_ui(true_action) {
  if (timeout_id) {
    clearTimeout(timeout_id);
    timeout_id = null;
  }
  try {
    if (true_action) {
      hide_animations();
      $(action_uiclass_map[true_action]).show();
      timeout_id = setTimeout(hide_animations, SIMULATION_INTERVAL);
    }
  } catch (error) {
    console.log("error while updating ui");
    console.error(error);
  }
}

function send_command(action) {
  // alert("hi");
  update_ui(action);
  $.ajax({
    type: "GET",
    url: `prediction?action=${action}`, //TODO: update request URL
    success: (result) => {
      console.log("result is ", result);
      predicted_action = result.prediction;
      let endpoint = `${INTENDED_IP}/${predicted_action}`;
      console.log("we are sending to ", endpoint, predicted_action);
      $(pred_action_uiclass_map[predicted_action]).show();
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

// the idle included partz

let Time_To_Trigger = SIMULATION_INTERVAL ;
$(function () {
  setInterval(function () {
    if(!timeout_id){
      send_command("I");
    }
  }, Time_To_Trigger);
});
