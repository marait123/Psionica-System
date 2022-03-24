function send_command(endpoint) {
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
