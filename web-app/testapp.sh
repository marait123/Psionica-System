source env/bin/activate
export FLASK_APP=flaskr
export FLASK_DEBUG=1
coverage run -m unittest discover
coverage report -m
coverage html