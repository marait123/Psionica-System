

testpoint () {
    x=$(curl --location --request GET "$1")
    # echo $x
    if [[ $x == *"$2"* ]]; then
        echo "$2 succeeded"
    else
        >&2 echo "$2 failed"
    fi

}

testpoint http://192.168.4.1/F "move forward"
testpoint http://192.168.4.1/B "move backward"
testpoint http://192.168.4.1/R "turning right"
testpoint http://192.168.4.1/L "turning left"
testpoint http://192.168.4.1/S "stop moving"
