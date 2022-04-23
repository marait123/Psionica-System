from argparse import Namespace
from distutils.core import setup
import imp
from logging import debug
import os
import threading
from urllib.robotparser import RequestRate
from debugpy import connect
from flask import Flask, json, request, jsonify, abort, render_template

from flask_cors import CORS
from flask_socketio import SocketIO, send,emit
from numpy import broadcast
from time import sleep

# simulation configuration
simulation_status=False
simulation_rate = 1 # in hz
last_action = "stop"
connected_clients = []

def simulate(app):
    global last_action
    global simulation_status
    with app.app_context():
        simulation_status=True
        with open("data/commands.txt", "r") as f:
            for line in f.readlines():
                line = line.strip()
                if line != "":
                    # print(line)
                    # print(connected_clients)
                    if len(connected_clients):
                        for client in connected_clients:
                            emit("new-action", {"action":line}, namespace=client['namespace'], room = client['room'])
                    last_action=line
                sleep(1/simulation_rate)
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
            pass
        # simulation_status= not simulation_status
        return jsonify(simulation_status=simulation_status)
    
    @app.route('/dashboard', methods=['GET'])
    def dashboard():
        return render_template('dashboard.html')


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
