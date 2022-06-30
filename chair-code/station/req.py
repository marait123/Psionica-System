# listen for incoming requests from headset scoket
# on action move left
# if mode is move car
    # send this action to the endpoint
    http://192.168.4.1:80/F "move forward"
    http://192.168.4.1:80/B "move backward"
    http://192.168.4.1:80/R "turning right"
    http://192.168.4.1:80/L "turning left"
    http://192.168.4.1:80/S "stop moving"
# else if mode is move cursor (pc listens on 8000)
    http://192.168.4.2:8000/F "move forward"
    http://192.168.4.2:8000/B "move backward"
    http://192.168.4.2:8000/R "turning right"
    http://192.168.4.2:8000/L "turning left"
    http://192.168.4.2:8000/S "stop moving"