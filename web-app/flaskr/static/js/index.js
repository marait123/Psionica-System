function send_command(endpoint) {
  // alert("hi");
  $.ajax({
    type: "GET",
    url: endpoint, //TODO: update request URL
    success: (result) => {
      console.log("result is ", result);
    },
    error: (error) => {
      console.error(error);
      alert(error);
    },
  });
}
let simulation_status = false;

function toggle_simulation() {
  $.ajax({
    type: "PUT",
    url: "/simulation", //TODO: update request URL
    success: (result) => {
      console.log("result is ", result);
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
      console.log("get_simulation_status result is ", result);
      simulation_status = result.simulation_status;
    },
    error: (error) => {
      console.error(error);
      alert(error);
    },
  });
});
