from argparse import Namespace
# from distutils.core import setup
import imp
from logging import debug
from random import random,randint
import threading
from flask import Flask,  request, jsonify,  render_template

from flask_cors import CORS
from flask_socketio import SocketIO, send,emit
from numpy import broadcast
from time import sleep
import socket
from model.preprocessing import predict, predict_idle,read_file_sim,read_file

test_data, actions = read_file_sim("action_test", "A07")
# print(test_data.shape, actions)
data_hash ={"L":[], "R":[], "B":[], "F":[], "I":[]}
for one_datum, one_label in zip(test_data, actions):
    data_hash[one_label].append(one_datum)

activity_test_data, activity_test_label = read_file("idle_test", "A07")

data_hash["I"] = activity_test_data[activity_test_label == 0]

# simulation configuration
simulation_status=False
simulation_step = 2 # in seconds
last_action = "stop"
connected_clients = []
# test_data, test_label =  
def simulate(app):
    global last_action
    global simulation_status
    correct_predictions = 0
    miss_predictions = 0
    with app.app_context():
        simulation_status=True
        with open("data/commands.txt", "r") as f:
            for line in f.readlines():
                if simulation_status == False:
                    #  stop the simulation
                    if len(connected_clients):
                        for client in connected_clients:
                            emit("end-simulation", {"action":"stop","miss_predictions":miss_predictions,"correct_predictions":correct_predictions}, namespace=client['namespace'], room = client['room'])
                        last_action="stop"
                    return
                line = line.strip()
                if line != "":
                    # print(line)
                    # print(connected_clients)
                    if len(connected_clients):
                        for client in connected_clients:
                            data_to_choose_from = data_hash[line]
                            n = len(data_to_choose_from) 
                            action_random_thinking_index = randint(0,n-1)
                            predicted_label = predict(data_to_choose_from[action_random_thinking_index])
                            print()
                            if line != predicted_label:
                                miss_predictions+=1
                            else:
                                correct_predictions+=1
                            # return jsonify()
                            emit("new-action", {"true_action":line,"predicted_action":predicted_label}, namespace=client['namespace'], room = client['room'])
                    last_action=line
                sleep(simulation_step)
                
        if len(connected_clients):
                for client in connected_clients:
                    emit("end-simulation", {"action":"stop"}, namespace=client['namespace'], room = client['room'])
                last_action="stop"
        simulation_status=False
        
# simulate()
def create_app(test_config=None):
    app = Flask(__name__)
    
    # cors = CORS(app, resources={r"/api/*": {"origins": "*"}})
    cors = CORS(app, resources={"*": {"origins": "*"}})

    @app.after_request
    def after_request(response):
        response.headers.add('Access-Control-Allow-Headers',
                             'Content-Type,Authorization,true')
        response.headers.add('Access-Control-Allow-Methods',
                             'GET,PATCH,POST,DELETE,OPTIONS')
        return response

    @app.route('/', methods=['GET'])
    def index():
        return render_template('index.html')
        
    @app.route('/who', methods=['GET'])
    def who():
        hostname = socket.gethostname()
        local_ip = socket.gethostbyname(hostname)

        return jsonify(answer="simulator", hostname=hostname, ip=local_ip, port=12345)
    
    @app.route('/simulation', methods=['GET'])
    def get_simulation_status():
        return jsonify(simulation_status=simulation_status, last_action=last_action)
    
    @app.route('/simulation', methods=['PUT'])
    def toggle_simulation():
        global simulation_status
        
        if simulation_status == False:
            print('start of simulation')
            
            threading.Thread(target=simulate,args=(app,)).start()
            # simulate()
            print("end of simulation")
        else:
            simulation_status = False
        # simulation_status= not simulation_status
        return jsonify(simulation_status=simulation_status, last_action=last_action)
    
    @app.route('/dashboard', methods=['GET'])
    def dashboard():
        return render_template('dashboard.html')
    
    @app.route('/prediction', methods=['GET'])
    def predict_actions():
        action = request.args.get("action",None)
        assert action is not None

        print("action sent is ", action)
        data_to_choose_from = data_hash[action]
        n = len(data_to_choose_from) 
        action_random_thinking_index = randint(0,n-1)

        if predict_idle(data_to_choose_from[action_random_thinking_index]) == 1:
            predicted_label = predict(data_to_choose_from[action_random_thinking_index])
        else:
            predicted_label = "I"
        # print()
        return jsonify(prediction=predicted_label)


    @app.errorhandler(422)
    def unprocessable(error):
        return jsonify({
            "success": False,
            "error": 422,
        }), 422

    @app.errorhandler(404)
    def unprocessable(error):
        return jsonify({
            "message": "resource not found",
            "success": False,
            "error": 404,
        }), 404
    # the socket part
    socketio = SocketIO(app)
    @socketio.on('connect')
    def connect():
        print("connected socket", request.sid)
        print("namespace", request.namespace)
        global connected_clients
        if request.sid not in connected_clients:
            connected_clients.append({'namespace':request.namespace, 'room':request.sid})
            
        print(request.sid)

    @socketio.on('disconnect')
    def test_disconnect():
        
        global connected_clients
        print("client disconnected", request.sid)
        if request.sid in connected_clients:
            connected_clients.remove({'namespace':request.namespace, 'room':request.sid})
            
        print('Client disconnected')
    
    @socketio.on('message')
    def handle_my_custom_event(json):
        print('received json: ' + str(json))
        send("koko "+json, broadcast=True)
        
    return app
if __name__ == "__main__":
    print("hello all")
    # app.run(debug=True)
