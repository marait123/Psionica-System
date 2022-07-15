

testpoint () {
    x=$(curl --location --request GET "$1")
    # echo $x
    if [[ $x == *"$2"* ]]; then
        echo "$2 succeeded"
    else
        >&2 echo "$2 failed"
    fi
    sleep $3

}

testpoint http://192.168.1.159:12345/F "move forward" 2
testpoint http://192.168.1.159:12345/B "move backward" 2
testpoint http://192.168.1.159:12345/R "turning right" 2
testpoint http://192.168.1.159:12345/L "turning left" 2
testpoint http://192.168.1.159:12345/S "stop moving" 2
