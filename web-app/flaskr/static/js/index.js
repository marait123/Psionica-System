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
let currentTab = ".manualTab";
function TabChanged(e = null) {
  // Your Code goes here
  let tabClassName = "." + window.location.hash.substring(1) + "Tab";
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
});
